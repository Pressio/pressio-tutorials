
#include "pressio_rom_lspg.hpp"
#include "pressio_apps.hpp"
//#include "utils_eigen.hpp"

template <typename T = std::size_t>
auto convertFromVVecToMultiVec(
      const std::vector<std::vector<double>> & A0,
      T nrows, T ncols)
  -> pressio::containers::MultiVector<Eigen::MatrixXd>{

  using eig_mat = Eigen::MatrixXd;
  pressio::containers::MultiVector<eig_mat> ADW(nrows, ncols);

  for (T i=0; i<nrows; i++){
    for (T j=0; j<ncols; j++)
      ADW(i,j) = A0[i][j];
  }
  return ADW;
}

template <typename T = std::size_t>
auto readBasis(std::string filename, T romSize, T numCell)
  ->pressio::containers::MultiVector<Eigen::MatrixXd>
{
  std::vector<std::vector<double>> A0;
  ::pressio::utils::readAsciiMatrixStdVecVec(filename, A0, romSize);
  // read basis into a MultiVector
  auto phi = convertFromVVecToMultiVec(A0, numCell, romSize);
  //  phi.data()->Print(std::cout);
  return phi;
}

template <typename rom_state_t, typename native_state_t>
struct observer{
  std::ofstream myfile_;
  std::ofstream myRefFile_;
  observer(const native_state_t & y0)
  : myfile_("solution.bin",  std::ios::out | std::ios::binary), 
    myRefFile_("state_ref.bin",  std::ios::out | std::ios::binary)
  {
    for (int i =0; i < y0.size(); i++){
      myRefFile_.write(reinterpret_cast<const char*>(&y0(i)),sizeof(y0(i)));
    }
    myRefFile_.close();
  }

  void operator()(size_t step,
  		  double t,
  		  const rom_state_t & y){
    auto ydata = *y.data();
    for (int i=0;i<y.extent(0);i++){
      myfile_.write(reinterpret_cast<const char*>(&ydata(i)),sizeof(ydata(i)));
    }
    std::cout << "Time = " << t << std::endl; 
  }

  void closeFile(){
    myfile_.close();
  }
};


/*
  Regression test for shallow water equation ROM
*/

int main(int argc, char *argv[])
{
  pressio::log::initialize(pressio::logto::terminal);

  std::string checkStr {"PASSED"};

  using fom_t		= pressio::apps::swe2d;
  using scalar_t	= typename fom_t::scalar_type;

  // -------------------------------------------------------
  // create FOM object
  // -------------------------------------------------------
  constexpr int nx = 128;
  constexpr int ny = 128;
  scalar_t params[3];
  std::ifstream params_file("../novel_params.txt");
  for (int i = 0; i < 3; i++){
    params_file >> params[i];
  }
  params_file.close();
  scalar_t Lx = 5;
  scalar_t Ly = 5;
  fom_t appObj(Lx,Ly,nx,ny,params);
  scalar_t t = 0;
  scalar_t et = 10.;
  scalar_t dt = 0.01;


  // -------------------------------------------------------
  // read basis
  // -------------------------------------------------------
  using decoder_jac_t	= pressio::containers::MultiVector<Eigen::MatrixXd>;
  constexpr int romSize = 30;
  decoder_jac_t phi = readBasis("basis.txt", romSize, 3*nx*ny);
  const int numBasis = phi.numVectors();
  if( numBasis != romSize ) return 0;

  // -------------------------------------------------------
  // create decoder obj
  // -------------------------------------------------------
  using native_state_t  = typename fom_t::state_type;
  using fom_state_t  = pressio::containers::Vector<native_state_t>;
  using decoder_t = pressio::rom::LinearDecoder<decoder_jac_t, fom_state_t>;
  decoder_t decoderObj(phi);

  native_state_t yRef(appObj.getGaussianIC(params[1]));
  // -------------------------------------------------------
  // create ROM problem
  // -------------------------------------------------------
  using lspg_state_t = pressio::containers::Vector<Eigen::Matrix<scalar_t,-1,1>>;

  // define ROM state
  lspg_state_t yROM(romSize);
  // initialize to zero (reference state is IC)
  pressio::ops::fill(yROM, 0.0);

  // define LSPG type
  using ode_tag  = pressio::ode::implicitmethods::Euler;
  auto lspgProblem = pressio::rom::lspg::createDefaultProblemUnsteady<ode_tag>(
    appObj, decoderObj, yROM, yRef);

  // linear solver
  using eig_dyn_mat  = Eigen::Matrix<scalar_t, -1, -1>;
  using hessian_t  = pressio::containers::DenseMatrix<eig_dyn_mat>;
  using solver_tag   = pressio::solvers::linear::iterative::LSCG;
  using linear_solver_t = pressio::solvers::linear::Solver<solver_tag, hessian_t>;
  linear_solver_t linSolverObj;

  // GaussNewton solver with normal equations
  auto solver = pressio::rom::lspg::createGaussNewtonSolver(lspgProblem, yROM, linSolverObj);
  auto Nsteps = static_cast<::pressio::ode::types::step_t>(et/dt);
  solver.setTolerance(1e-13);
  solver.setMaxIterations(10);

  // define observer
  observer<lspg_state_t,native_state_t> Obs(yRef);
  // solve
  pressio::rom::lspg::solveNSequentialMinimizations(lspgProblem, yROM, 0.0, dt, Nsteps, Obs,solver);
  auto yFomFinal = lspgProblem.fomStateReconstructorCRef()(yROM);
  Obs.closeFile();
  return 0;
}

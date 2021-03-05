#include "pressio_rom_lspg.hpp"
#include "pressio_apps.hpp"

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

int main(int argc, char *argv[])
{
  pressio::log::initialize(pressio::logto::terminal);

  using fom_t		= pressio::apps::swe2d_hyper<std::vector<int>>;
  using scalar_t	= typename fom_t::scalar_type;

  // -------------------------------------------------------
  // Read in information for ROM and hyper-reducction 
  // -------------------------------------------------------
  int romSize;
  int sampleMeshSize;
  int sampleMeshPlusStencilSize;
  //======== Read in information on ROM size and sample/stencil mesh=========
  std::ifstream info_file("info_file.txt");
  info_file >> romSize;
  std::cout << romSize << std::endl;;
  info_file >> sampleMeshSize;
  info_file >> sampleMeshPlusStencilSize;
  info_file.close();
  std::cout << "| romSize = " << romSize
            << "| |  sampleMeshSize = " << sampleMeshSize
            << "| |  sampleMeshPlusStencilSize " << sampleMeshPlusStencilSize
            << std::endl;

  // Create data structures to contain sample mesh index information
  std::vector<int> sm_gids(sampleMeshSize);
  std::vector<int> smps_gids(sampleMeshPlusStencilSize);
  Eigen::VectorXd sm_rel_lids(3*sampleMeshSize);
  
  // Required for the app
  /*
    read in the global indices of the sample mesh. Note that we could alternatively extract these 
    from the information contained in sm_rel_lids and and smps_gids 
  */
  std::ifstream sample_mesh_gids_file("sample_mesh_gids.txt");
  for (int i =0; i < sampleMeshSize ; i++){
    sample_mesh_gids_file >> sm_gids[i];
  }
  sample_mesh_gids_file.close();

  // Required for the app
  /*
    read in the global indices of the stencil mesh. The underlying app is a structured solver, 
    and these indices allow us to find the correct location on a structured grid
  */
  std::ifstream sample_mesh_gids_plus_stencil_file("sample_mesh_plus_stencil_gids.txt");
  for (int i =0; i < sampleMeshPlusStencilSize; i++){
    sample_mesh_gids_plus_stencil_file >>smps_gids[i];
  }
  sample_mesh_gids_plus_stencil_file.close();

  // Required for Pressio
  /*
    read in the indices of the sample mesh relative to the stencil mesh,
    i.e., if the 4th element of the stencil mesh is the zeroth element of the sample mesh,
    then the zeroth entry would be sm_rel_lids(0) = 4;
  */
  std::ifstream sample_mesh_rel_lids_file("sample_mesh_relative_indices.txt");
  for (int i =0; i < sampleMeshSize ; i++){
    sample_mesh_rel_lids_file >> sm_rel_lids(3*i);
    sm_rel_lids(3*i) = sm_rel_lids(3*i)*3;
    sm_rel_lids(3*i+1) = sm_rel_lids(3*i)+1; 
    sm_rel_lids(3*i+2) = sm_rel_lids(3*i)+2;
  }
  sample_mesh_rel_lids_file.close();
  pressio::containers::Vector<Eigen::VectorXd> sampleMeshRelIds(sm_rel_lids);
  //============

  // Construct the app 
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

  fom_t appObj(Lx,Ly,nx,ny,params,sm_gids,smps_gids);

  // -------------------------------------------------------
  // read basis
  // -------------------------------------------------------
  using decoder_jac_t	= pressio::containers::MultiVector<Eigen::MatrixXd>;
  decoder_jac_t phi = readBasis("PhiSamplePlusStencil.txt", romSize, sampleMeshPlusStencilSize*3);
  const int numBasis = phi.numVectors();
  if( numBasis != romSize ) return 0;

  // ------------------------------------------------------
  // construct decoder 
  // ------------------------------------------------------
  using native_state_t  = typename fom_t::state_type;
  using fom_state_t  = pressio::containers::Vector<native_state_t>;
  using decoder_t = pressio::rom::LinearDecoder<decoder_jac_t, fom_state_t>;
  decoder_t decoderObj(phi);

  // create the reference vector (on the stencil mesh).
  native_state_t yRef(appObj.getGaussianIC(params[1]));
  // For post processing, we also make a vector on the full mesh
  native_state_t yRefFull(appObj.getGaussianICFull(params[1])); 

  // -------------------------------------------------------
  // create ROM problem
  // -------------------------------------------------------

  using lspg_state_t = pressio::containers::Vector<Eigen::Matrix<scalar_t,-1,1>>;
  // define ROM state
  lspg_state_t yROM(romSize);
  // initialize to zero (reference state is the initial condition)
  pressio::ops::fill(yROM, 0.0);

  // define LSPG type
  using ode_tag  = pressio::ode::implicitmethods::CrankNicolson;
  auto lspgProblem = pressio::rom::lspg::createHyperReducedProblemUnsteady<ode_tag>(appObj, decoderObj, yROM, yRef,sampleMeshRelIds);

  // linear solver
  using eig_dyn_mat  = Eigen::Matrix<scalar_t, -1, -1>;
  using hessian_t  = pressio::containers::DenseMatrix<eig_dyn_mat>;
  using solver_tag   = pressio::solvers::linear::iterative::LSCG;
  using linear_solver_t = pressio::solvers::linear::Solver<solver_tag, hessian_t>;
  linear_solver_t linSolverObj;

  // GaussNewton solver with normal equations
  auto solver = pressio::rom::lspg::createGaussNewtonSolver(lspgProblem, yROM, linSolverObj);
  solver.setTolerance(1e-6);
  solver.setMaxIterations(10);


  scalar_t t = 0;
  scalar_t et = 10.;
  scalar_t dt = 0.02;
  auto Nsteps = static_cast<::pressio::ode::types::step_t>(et/dt);
  // define observer
  observer<lspg_state_t,native_state_t> Obs(yRefFull);
  // solve
  auto startTime = std::chrono::high_resolution_clock::now();
  pressio::rom::lspg::solveNSequentialMinimizations(lspgProblem, yROM, 0.0, dt, Nsteps, Obs,solver);
  auto yFomFinal = lspgProblem.fomStateReconstructorCRef()(yROM);
  auto solNorm = (*yFomFinal.data()).norm();
  std::cout << std::setprecision(14) << solNorm << std::endl;
  Obs.closeFile();
  auto finishTime = std::chrono::high_resolution_clock::now();
  const std::chrono::duration<double> elapsed2 = finishTime - startTime;
  std::cout << "Walltime (single ROM run) = " << elapsed2.count() << '\n';
  return 0;
}
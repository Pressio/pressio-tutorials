#include "pressio/ode_advancers.hpp"
#include "pressio/ode_steppers_implicit.hpp"
#include <Eigen/Core>

template<class ScalarType>
class Lorenz3{
  const int N_ = 3;
  const ScalarType rho_   = static_cast<ScalarType>(28);
  const ScalarType sigma_ = static_cast<ScalarType>(10);
  const ScalarType beta_  = static_cast<ScalarType>(8)/3;

  using trip_t = Eigen::Triplet<ScalarType>;
  mutable std::vector<trip_t> tripletList_;

public:
  using independent_variable_type = ScalarType;
  using state_type                = Eigen::Matrix<ScalarType,-1,1>;
  using right_hand_side_type      = state_type;
  // since the Jacobain of Lorenz3 is fairly dens,
  // we could use a dense matrix for it, but we use a sparse one
  // here for demonstration purposes
  using jacobian_type             = Eigen::SparseMatrix<ScalarType>;

  Lorenz3(){
    allocateTriplets();
  }

  state_type createState() const{
    state_type s(N_);
    s.setConstant(0);
    return s;
  };

  right_hand_side_type createRightHandSide() const{
    right_hand_side_type v(N_);
    v.setConstant(0);
    return v;
  };

  jacobian_type createJacobian() const{
    jacobian_type J(N_, N_);
    J.setFromTriplets(tripletList_.begin(), tripletList_.end());
    return J;
  };

  void operator()(const state_type & state,
		  const independent_variable_type timeIn,
		  right_hand_side_type & rhs,
		  jacobian_type & J,
		  bool computeJac) const
  {
    const auto x = state(0);
    const auto y = state(1);
    const auto z = state(2);

    rhs(0) = sigma_ * (y - x);
    rhs(1) = x * (rho_ - z) - y;
    rhs(2) = x * y - beta_ * z;

    if (computeJac)
    {
      tripletList_[0] = {0, 0, -sigma_};
      tripletList_[1] = {0, 1,  sigma_};

      tripletList_[2] = {1, 0, rho_-z};
      tripletList_[3] = {1, 1, -1.};
      tripletList_[4] = {1, 2, -x};

      tripletList_[5] = {2, 0, y};
      tripletList_[6] = {2, 1, x};
      tripletList_[7] = {2, 2, -beta_};
      J.setFromTriplets(tripletList_.begin(), tripletList_.end());
    }
  };

private:
  void allocateTriplets()
  {
    // first row
    tripletList_.push_back( trip_t( 0, 0, 0.) );
    tripletList_.push_back( trip_t( 0, 1, 0.) );
    // second and third row
    for (int i=1; i<=2; ++i){
      tripletList_.push_back( trip_t( i, 0, 0.) );
      tripletList_.push_back( trip_t( i, 1, 0.) );
      tripletList_.push_back( trip_t( i, 2, 0.) );
    }
  }
};

// template<class ScalarType>
// class StateObserver
// {
// public:
//   explicit StateObserver(int sampleFreq)
//     : myfile_(
//       sampleFreq_(sampleFreq){}

//   ~StateObserver(){  }

//   template<typename TimeType>

// private:
//   std::ofstream myfile_;
//   const int sampleFreq_ = {};
// };

int main(int argc, char *argv[])
{
  pressio::log::initialize(pressio::logto::terminal);

  using scalar_type = double;
  using problem_type = Lorenz3<scalar_type>;
  problem_type problem;

  namespace pode = pressio::ode;
  namespace plins  = pressio::linearsolvers;
  namespace pnlins = pressio::nonlinearsolvers;

  // create stepper
  constexpr auto scheme = pode::StepScheme::BDF2;
  auto stepper = pode::create_implicit_stepper(scheme, problem);

  // linear and nonlinear solver
  using fom_jacobian_t = typename problem_type::jacobian_type;
  using linear_solver_t = plins::Solver<plins::iterative::Bicgstab, fom_jacobian_t>;
  linear_solver_t linearSolver;

  auto nonLinearSolver = pnlins::create_newton_raphson(stepper, linearSolver);
  nonLinearSolver.setTolerance(1e-13);

  auto y = problem.createState();
  y(0) = 0.5; y(1) = 1.0; y(2) = 1.;
  const scalar_type startTime{0.0};
  const scalar_type finalTime{40.0};
  std::ofstream myfile("state_snapshots.bin",  std::ios::out | std::ios::binary);
  pode::advance_to_target_point(stepper, y, startTime, finalTime,
				// lambda to set the time step
				[=](const pode::StepCount & /*unused*/,
				    const pode::StepStartAt<scalar_type> & /*unused*/,
				    pode::StepSize<scalar_type> & dt)
				{ dt = 0.01; },
				// lambda to observe and write to file the state
				[&](pressio::ode::StepCount step,
				    const scalar_type /*timeIn*/,
				    const typename problem_type::state_type & state)
				{
				  std::cout << "saving state at = " << step.get() << "\n";
				  const std::size_t ext = state.size()*sizeof(scalar_type);
				  myfile.write(reinterpret_cast<const char*>(&state(0)), ext);
				},
				nonLinearSolver);
  myfile.close();

  pressio::log::finalize();
  return 0;
}

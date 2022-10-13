#include "pressio/ode_advancers.hpp"
#include "pressio/ode_steppers_explicit.hpp"
#include <Eigen/Core>

template<class ScalarType>
struct SimpleSystem{
  using independent_variable_type = ScalarType;
  using state_type                = Eigen::Matrix<ScalarType,-1,1>;
  using right_hand_side_type      = state_type;

public:
  state_type createState() const{
    auto s = state_type(3);
    s.setConstant(0);
    return s;
  };

  right_hand_side_type createRightHandSide() const{
    auto v = right_hand_side_type(3);
    v.setConstant(0);
    return v;
  };

  void operator()(const state_type & state,
		  const independent_variable_type timeIn,
		  right_hand_side_type & rhs) const
  {
    constexpr ScalarType ten{10};
    rhs(0) = ten * state(0);
    rhs(1) = ten * state(1);
    rhs(2) = ten * state(2);
  };
};

int main(int argc, char *argv[])
{
  pressio::log::initialize(pressio::logto::terminal);

  using scalar_type = double;
  SimpleSystem<scalar_type> problem;

  auto y = problem.createState();
  y(0) = 1.; y(1) = 2.; y(2) = 3.;

  namespace pode = pressio::ode;
  constexpr auto scheme = pode::StepScheme::ForwardEuler;
  auto stepperObj = pode::create_explicit_stepper(scheme, problem);

  const scalar_type dt{0.1};
  const scalar_type startTime{0.0};
  pode::advance_n_steps(stepperObj, y, startTime, dt, pode::StepCount{1});

  // check solution
  std::cout << "Computed solution: ["
            << y(0) << " " << y(1) << " " << y(2) << "] "
            << "Expected solution: [2,4,6] "
            << std::endl;

  pressio::log::finalize();
  return 0;
}

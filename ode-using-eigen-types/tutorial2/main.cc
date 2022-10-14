#include "pressio/ode_advancers.hpp"
#include "pressio/ode_steppers_explicit.hpp"
#include <Eigen/Core>

template<class ScalarType>
class Lorenz3{
  const int N_ = 3;
  const ScalarType rho_   = static_cast<ScalarType>(28);
  const ScalarType sigma_ = static_cast<ScalarType>(10);
  const ScalarType beta_  = static_cast<ScalarType>(8)/3;

public:
  using independent_variable_type = ScalarType;
  using state_type                = Eigen::Matrix<ScalarType,-1,1>;
  using right_hand_side_type      = state_type;

  state_type createState() const{
    auto s = state_type(N_);
    s.setConstant(0);
    return s;
  };

  right_hand_side_type createRightHandSide() const{
    auto v = right_hand_side_type(N_);
    v.setConstant(0);
    return v;
  };

  void operator()(const state_type & state,
		  const independent_variable_type timeIn,
		  right_hand_side_type & rhs) const
  {
    const auto x = state(0);
    const auto y = state(1);
    const auto z = state(2);

    rhs(0) = sigma_ * (y - x);
    rhs(1) = x * (rho_ - z) - y;
    rhs(2) = x * y - beta_ * z;
  };
};

template<class ScalarType>
class StateObserver
{
public:
  explicit StateObserver(int sampleFreq)
    : myfile_("state_snapshots.bin",  std::ios::out | std::ios::binary),
      sampleFreq_(sampleFreq){}

  ~StateObserver(){ myfile_.close(); }

  template<typename TimeType>
  void operator()(pressio::ode::StepCount step,
		  const TimeType /*timeIn*/,
		  const Eigen::Matrix<ScalarType,-1,1> & state)
  {
    if (step.get() % sampleFreq_ == 0){
      std::cout << "saving state at step = " << step.get() << "\n";
      const std::size_t ext = state.size()*sizeof(ScalarType);
      myfile_.write(reinterpret_cast<const char*>(&state(0)), ext);
    }
  }

private:
  std::ofstream myfile_;
  const int sampleFreq_ = {};
};

int main(int argc, char *argv[])
{
  pressio::log::initialize(pressio::logto::terminal);

  using scalar_type = double;
  Lorenz3<scalar_type> problem;

  namespace pode = pressio::ode;
  constexpr auto scheme = pode::StepScheme::RungeKutta4;
  auto stepperObj = pode::create_explicit_stepper(scheme, problem);

  auto y = problem.createState();
  y(0) = 0.5; y(1) = 1.0; y(2) = 1.;
  const scalar_type startTime{0.0};
  const scalar_type finalTime{40.0};
  const int stateSampleFreq = 2;
  pode::advance_to_target_point(stepperObj, y, startTime,
				finalTime,
				  [=](const pode::StepCount & /*unused*/,
				     const pode::StepStartAt<scalar_type> & /*unused*/,
				     pode::StepSize<scalar_type> & dt)
				  { dt = 0.01; },
				StateObserver<scalar_type>{stateSampleFreq});

  pressio::log::finalize();
  return 0;
}


#ifndef RUN_DEFAULT_GALERKIN_HPP_
#define RUN_DEFAULT_GALERKIN_HPP_

#include "pressio/rom_subspaces.hpp"
#include "pressio/rom_galerkin_unsteady.hpp"
#include "observer.hpp"
#include "rom_shared.hpp"

template<class AppObjType, class ParserType>
void run_galerkin_default(const AppObjType & appObj,
			  const ParserType & parser)
{
  namespace pode = pressio::ode;
  namespace prom = pressio::rom;
  namespace pgal = pressio::rom::galerkin;

  //
  // prepare trial subspace
  //
  using scalar_t = typename AppObjType::scalar_type;
  using reduced_state_type = Eigen::Matrix<scalar_t, Eigen::Dynamic, 1>;

  auto basis = create_basis_and_read_from_file<scalar_t>(parser.romFullMeshPodBasisFile(),
							 parser.romModeCount());

  const auto affineShiftFile = parser.romAffineShiftFile();
  auto affineShift = create_affine_shift_and_read_from_file<scalar_t>(affineShiftFile);
  auto space = prom::create_trial_subspace<reduced_state_type>(std::move(basis),
							       affineShift,
							       parser.romIsAffine());
  // create reduced state and fill from file
  auto reducedState = space.createReducedState();
  fill_rom_state_from_ascii(parser.romInitialStateFile(), reducedState);

  StateObserver observer(parser.stateSamplingFreq());
  const auto odeScheme = parser.odeScheme();
  if (pressio::ode::is_explicit_scheme(odeScheme))
  {
    // create problem and run
    auto problem = pgal::create_unsteady_explicit_problem(odeScheme, space, appObj);
    typename AppObjType::time_type startTime{0};
    pode::advance_n_steps(problem, reducedState, startTime, parser.timeStepSize(),
			  pode::StepCount{parser.numSteps()}, observer);
  }
  else{
    throw std::runtime_error("Default implicit galerkin not impl yet");
  }

}

#endif

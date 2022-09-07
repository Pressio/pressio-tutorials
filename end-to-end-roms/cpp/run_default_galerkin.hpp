
#ifndef RUN_DEFAULT_GALERKIN_HPP_
#define RUN_DEFAULT_GALERKIN_HPP_

#include "pressio/rom_subspaces.hpp"
#include "pressio/rom_galerkin_unsteady.hpp"
#include "observer.hpp"
#include "rom_shared.hpp"

template<class FomSystemType, class ParserType>
void run_galerkin_default(const FomSystemType & fomSystem,
			  const ParserType & parser)
{
  namespace pode = pressio::ode;
  namespace prom = pressio::rom;
  namespace pgal = pressio::rom::galerkin;

  using scalar_t = typename FomSystemType::scalar_type;
  using reduced_state_type = Eigen::Matrix<scalar_t, Eigen::Dynamic, 1>;

  const auto modeCount = parser.romModeCount();
  const auto & basisFile = parser.romFullMeshPodBasisFile();
  auto basis = create_basis_and_read_from_file<scalar_t>(basisFile, modeCount);

  const auto affineShiftFile = parser.romAffineShiftFile();
  auto affineShift = create_affine_shift_and_read_from_file<scalar_t>(affineShiftFile);
  auto trialSpace = prom::create_trial_column_subspace<reduced_state_type>(std::move(basis),
								    affineShift,
								    parser.romIsAffine());
  auto reducedState = trialSpace.createReducedState();
  fill_rom_state_from_ascii(parser.romInitialStateFile(), reducedState);

  StateObserver observer(parser.stateSamplingFreq());
  const auto odeScheme = parser.odeScheme();
  if (pressio::ode::is_explicit_scheme(odeScheme))
  {
    auto problem = pgal::create_unsteady_explicit_problem(odeScheme, trialSpace, fomSystem);

    typename FomSystemType::time_type startTime{0};
    const auto numSteps = pode::StepCount{parser.numSteps()};
    pode::advance_n_steps(problem, reducedState, startTime,
			  parser.timeStepSize(), numSteps, observer);
  }
  else{
    throw std::runtime_error("Default implicit galerkin not impl yet");
  }
}

#endif

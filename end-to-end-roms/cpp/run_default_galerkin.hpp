
#ifndef RUN_DEFAULT_GALERKIN_HPP_
#define RUN_DEFAULT_GALERKIN_HPP_

/* NOTE:
   1. the formatting here matters because the rst docs
      use literalincludes so if you change somehting below
      it is likely you impact the documentation

   2. the comments below are used by the rst documentaion
      in non contiguous literalinclude statetements so that
      we can make the documentation more clear

   3. do NOT erase these comments, and do NOT move them
      or you impact the rst docs

   // branch taken in this demo
   // branch not used in this demo
*/

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

  using scalar_type = typename FomSystemType::scalar_type;
  using reduced_state_type = Eigen::Matrix<scalar_type, Eigen::Dynamic, 1>;

  auto basis = create_basis_and_read_from_file<
         scalar_type>(parser.romFullMeshPodBasisFile(), parser.romModeCount());
  auto affineShift = create_affine_shift_and_read_from_file<
         scalar_type>(parser.romAffineShiftFile());
  const auto trialSpace  = prom::create_trial_column_subspace<
         reduced_state_type>(std::move(basis), std::move(affineShift), parser.romIsAffine());

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

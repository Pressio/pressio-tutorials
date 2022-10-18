
#ifndef RUN_DEFAULT_LSPG_HPP_
#define RUN_DEFAULT_LSPG_HPP_

/* NOTE:
   1. the formatting here matters because the rst docs
      use literalincludes so if you change somehting below
      it is likely you impact the documentation

   2. the comments below are used by the rst documentaion
      in non contiguous literalinclude statetements so that
      we can make the documentation more clear
*/

#include "pressio/rom_subspaces.hpp"
#include "pressio/rom_lspg_unsteady.hpp"
#include "observer.hpp"
#include "lspg_pick_solver_and_run.hpp"
#include "rom_shared.hpp"

template<class FomSystemType, class ParserType>
void run_lspg_default(const FomSystemType & fomSystem,
		      const ParserType & parser)
{
  namespace pode = pressio::ode;
  namespace prom = pressio::rom;
  namespace plspg = pressio::rom::lspg;

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

  const auto odeScheme = parser.odeScheme();
  auto problem = plspg::create_unsteady_problem(odeScheme, trialSpace, fomSystem);
  lspg_pick_solver_and_run(parser, problem.lspgStepper(), reducedState);
}

#endif


#ifndef RUN_DEFAULT_LSPG_HPP_
#define RUN_DEFAULT_LSPG_HPP_

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

  const auto odeScheme = parser.odeScheme();
  auto problem = plspg::create_unsteady_problem(odeScheme, trialSpace, fomSystem);
  lspg_pick_solver_and_run(parser, problem, reducedState);
}

#endif

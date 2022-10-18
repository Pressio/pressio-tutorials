
#ifndef RUN_HYPRED_GALERKIN_HPP_
#define RUN_HYPRED_GALERKIN_HPP_

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

template<class ScalarType, class FomRhsType>
class MyExplicitGalerkinHyperReducer
{
  static_assert(pressio::is_vector_eigen<FomRhsType>::value,
		"FomRhsType must be an Eigen vector");

  using matrix_type = Eigen::Matrix<ScalarType, -1, -1, Eigen::ColMajor>;
  matrix_type P_;

public:
  using time_type = ScalarType;
  using right_hand_side_operand_type = FomRhsType;

  MyExplicitGalerkinHyperReducer() = delete;
  MyExplicitGalerkinHyperReducer(const std::string & projFile,
				  const int numModes){
    P_ = create_colmajor_eigen_matrix_and_load_from_binary_with_extents<
      ScalarType>(projFile, numModes);
  }

  template<class ResultType>
  std::enable_if_t<pressio::is_vector_eigen<ResultType>::value>
  operator()(const right_hand_side_operand_type & operand,
	     const time_type & /*unused*/,
	     ResultType & result) const
  {
    result = P_.transpose() * operand;
  }
};

template<class FomSystemType, class ParserType>
void run_galerkin_hyperreduced(const FomSystemType & fomSystem,
			       const ParserType & parser)
{
  namespace pode = pressio::ode;
  namespace prom = pressio::rom;
  namespace pgal = pressio::rom::galerkin;

  using scalar_type = typename FomSystemType::scalar_type;
  using reduced_state_type = Eigen::Matrix<scalar_type, Eigen::Dynamic, 1>;

  const auto modeCount   = parser.romModeCount();
  const auto & basisFile = parser.romFullMeshPodBasisFile();
  auto basisFull      = create_basis_and_read_from_file<scalar_type>(basisFile, modeCount);
  auto basisOnStencil = create_basis_on_stencil_mesh(basisFull, parser);
  auto affineShift    = create_affine_shift_on_stencil_mesh<scalar_type>(parser);
  auto trialSpace     = prom::create_trial_column_subspace<
         reduced_state_type>(std::move(basisOnStencil), std::move(affineShift), parser.romIsAffine());

  auto reducedState = trialSpace.createReducedState();
  fill_rom_state_from_ascii(parser.romInitialStateFile(), reducedState);

  StateObserver observer(parser.stateSamplingFreq());
  const auto odeScheme = parser.odeScheme();
  if (pressio::ode::is_explicit_scheme(odeScheme))
  {
    using fom_rhs_type = typename FomSystemType::right_hand_side_type;
    using hr_op_t = MyExplicitGalerkinHyperReducer<scalar_type, fom_rhs_type>;
    hr_op_t H(parser.romGalerkinHypRedOperatorFile(), modeCount);

    auto problem = pgal::create_unsteady_explicit_problem(odeScheme, trialSpace, fomSystem, H);

    typename FomSystemType::time_type startTime{0};
    const auto numSteps = pode::StepCount{parser.numSteps()};
    pode::advance_n_steps(problem, reducedState, startTime,
			  parser.timeStepSize(), numSteps, observer);
  }
  else{
    throw std::runtime_error("Hyper-reduced implicit galerkin not impl yet");
  }
}

#endif

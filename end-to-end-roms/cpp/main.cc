
#include "pressiodemoapps/swe2d.hpp"

#include "pressio/ode_steppers_explicit.hpp"
#include "pressio/ode_steppers_implicit.hpp"
#include "pressio/ode_advancers.hpp"

#include "parsers.hpp"
#include "myio.hpp"
#include "run_fom_explicit.hpp"
#include "run_fom_implicit.hpp"
#include "run_default_galerkin.hpp"
#include "run_hypred_galerkin.hpp"
// #include "run_lspg.hpp"
// #include "source.hpp"
#include <chrono>

template<class AppObjType, class ParserType>
void dispatch(const AppObjType & fomSystem,
	      const ParserType & parser)
{

  if (parser.doingFom()){
    if (pressio::ode::is_explicit_scheme(parser.odeScheme())){
      run_fom_explicit(fomSystem, parser);
    }
    else{
      run_fom_implicit(fomSystem, parser);
    }
  }

  else if (parser.romAlgorithm()=="defaultGalerkin"){
    run_galerkin_default(fomSystem, parser);
  }

  else if (   parser.romAlgorithm()=="collocationGalerkin"
	   || parser.romAlgorithm()=="gappyGalerkin")
  {
    run_galerkin_hyperreduced(fomSystem, parser);
  }

  else{
    throw std::runtime_error("dispatch: invalid branching");
  }
}

std::string check_and_get_inputfile(int argc, char *argv[])
{
  if (argc != 2){
    throw std::runtime_error("Call as: ./exe <path-to-inputfile>");
  }
  const std::string inputFile = argv[1];
  std::cout << "Input file: " << inputFile << "\n";
  assert( file_exists(inputFile) );
  return inputFile;
}

int main(int argc, char *argv[])
{
  pressio::log::initialize(pressio::logto::terminal);
  pressio::log::setVerbosity({pressio::log::level::info});

  // local scope because it is helps
  {
    const auto inputFile = check_and_get_inputfile(argc, argv);
    auto node = YAML::LoadFile(inputFile);

    const auto problemNode = node["problem"];
    if (!problemNode){ throw std::runtime_error("Missing problem in yaml input!"); }
    const auto problemName = problemNode.as<std::string>();
    std::cout << problemName << std::endl;

    // start timer
    auto t1 = std::chrono::high_resolution_clock::now();

    namespace pda  = pressiodemoapps;
    if (problemName == "2d_swe")
    {
      using scalar_t = double;
      ParserTwoDimShallowWater<scalar_t> parser(node);
      const auto meshObj = pda::load_cellcentered_uniform_mesh_eigen<scalar_t>(parser.meshDir());
      const auto probId  = pda::Swe2d::SlipWall;
      const auto invScheme = string_to_inviscid_flux_scheme(parser.inviscidFluxReconstruction());
      const auto gravity   = parser.gravity();
      const auto coriolis  = parser.coriolis();
      const auto pulseMag  = parser.pulseMagnitude();
      auto fomSystem = pda::create_slip_wall_swe_2d_problem_eigen(meshObj, invScheme,
								  gravity, coriolis, pulseMag);
      dispatch(fomSystem, parser);
    }

    auto t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration< double > fs = t2 - t1;
    std::cout << "elapsed " << fs.count() << std::endl;
  }

  pressio::log::finalize();
  return 0;
}

  // if (problem == "2d_rd")
  // {
  //   namespace pda  = pressiodemoapps;
  //   ParserTwoDimReacDiff<scalar_t> parser(node);
  //   const auto meshObj   = pda::load_cellcentered_uniform_mesh_eigen<scalar_t>(parser.meshDir());
  //   const auto diffusion = parser.diffusion();
  //   const auto reaction  = parser.reaction();
  //   const auto scheme    = pda::ViscousFluxReconstruction::FirstOrder;
  //   const auto probId    = pda::DiffusionReaction2d::ProblemA;
  //   auto fomSystem = pda::create_problem_eigen(meshObj, probId, scheme,
  // 					    mySource, diffusion, reaction);
  //   dispatch(fomSystem, parser);
  // }


  // if (problem == "2d_gs")
  // {
  //   namespace pda  = pressiodemoapps;
  //   ParserTwoDimGrayScott<scalar_t> parser(node);
  //   const auto meshObj = pda::load_cellcentered_uniform_mesh_eigen<scalar_t>(parser.meshDir());
  //   const auto probId  = pda::DiffusionReaction2d::GrayScott;
  //   const auto scheme  = pda::ViscousFluxReconstruction::FirstOrder;

  //   const auto diffusionA = parser.diffusionA();
  //   const auto diffusionB = parser.diffusionB();
  //   const auto feedRate   = parser.feedRate();
  //   const auto killRate   = parser.killRate();
  //   auto fomSystem = pda::create_problem_eigen(meshObj, probId, scheme,
  // 			    diffusionA, diffusionB,
  // 			    feedRate, killRate);
  //   dispatch(fomSystem, parser);
  // }

#ifndef PRESSIO_TUTORIALS_ENDTOEND_WORKFLOW_PARSERS_HPP_
#define PRESSIO_TUTORIALS_ENDTOEND_WORKFLOW_PARSERS_HPP_

#include "yaml-cpp/parser.h"
#include "yaml-cpp/yaml.h"

template<class T = void>
pressio::ode::StepScheme
string_to_ode_scheme(const std::string & strIn)
{
  namespace pode = pressio::ode;

       if (strIn == "ForwardEuler"){ return pode::StepScheme::ForwardEuler; }
  else if (strIn == "RungeKutta4"){ return pode::StepScheme::RungeKutta4; }
  else if (strIn == "RK4"){ return pode::StepScheme::RungeKutta4; }
  else if (strIn == "SSPRungeKutta3"){ return pode::StepScheme::SSPRungeKutta3; }
  else if (strIn == "SSPRK3"){ return pode::StepScheme::SSPRungeKutta3; }
  else if (strIn == "BDF1"){ return pode::StepScheme::BDF1; }
  else if (strIn == "CrankNicolson"){ return pode::StepScheme::CrankNicolson; }
  else if (strIn == "BDF2"){ return pode::StepScheme::BDF2; }
  else{
    throw std::runtime_error("string_to_ode_scheme: Invalid odeScheme");
  }
}

template<class T = void>
pressiodemoapps::InviscidFluxReconstruction
string_to_inviscid_flux_scheme(const std::string & strIn)
{
  namespace pda  = pressiodemoapps;

       if (strIn == "FirstOrder"){ return pda::InviscidFluxReconstruction::FirstOrder; }
  else if (strIn == "Weno3"){ return pda::InviscidFluxReconstruction::Weno3; }
  else if (strIn == "Weno5"){ return pda::InviscidFluxReconstruction::Weno5; }
  else{
    throw std::runtime_error("string_to_inviscid_flux_scheme: Invalid choice");
  }
}

template <typename ScalarType>
class ParserCommon
{

  std::string meshDirPath_     = "empty";
  int numDofsPerCell_          = -1;
  ScalarType dt_	       = {};
  ScalarType finalTime_	       = {};
  int numSteps_                = -1;
  int rhsSamplingFreq_         = -1;
  int stateSamplingFreq_       = -1;
  std::string odeSchemeString_ = {};
  pressio::ode::StepScheme odeScheme_  = {};
  std::string nonlinSolverStr_ = "void";
  ScalarType nonlinSolverTol_  = std::numeric_limits<ScalarType>::max();
  bool doingFom_ = {};

public:
  ParserCommon() = delete;

  ParserCommon(YAML::Node & node){
    this->parseImpl(node);
    // if not doing ROM, then must be FOM
    doingFom_ = !node["rom"];
  }

  auto meshDir()		  const { return meshDirPath_; }
  auto numDofsPerCell()		  const { return numDofsPerCell_; }
  auto timeStepSize()	          const { return dt_; }
  auto numSteps()		  const { return numSteps_; }
  auto rhsSamplingFreq()          const { return rhsSamplingFreq_; }
  auto stateSamplingFreq()	  const { return stateSamplingFreq_; }
  auto odeScheme()		  const { return odeScheme_; }
  auto nonlinearSolver()	  const { return nonlinSolverStr_; }
  auto nonlinearSolverTolerance() const { return nonlinSolverTol_; }
  auto doingFom()                 const { return doingFom_; }

  void describe() const{
    std::cout << "\nmeshDir = "	<< meshDirPath_	  << " \n"
	      << "numDofPerCell = " << numDofsPerCell_ << " \n";
  }

private:
  void parseImpl(YAML::Node & node)
  {
    auto entry = "meshDir";
    if (node[entry]) meshDirPath_ = node[entry].as<std::string>();
    else throw std::runtime_error("Empty meshDir");

    entry = "numDofsPerCell";
    if (node[entry]) numDofsPerCell_ = node[entry].as<int>();
    else throw std::runtime_error("Empty numDofsPerCell");

    entry = "odeScheme";
    if (node[entry]) {
      odeSchemeString_ = node[entry].as<std::string>();
      odeScheme_ = string_to_ode_scheme(odeSchemeString_);
    }
    else{
      throw std::runtime_error("Input: missing odeScheme");
    }

    entry = "timeStepSize";
    if (node[entry]) dt_ = node[entry].as<ScalarType>();
    else throw std::runtime_error("Input: missing dt");

    entry = "finalTime";
    if (node[entry]) finalTime_ = node[entry].as<ScalarType>();
    else throw std::runtime_error("Input: missing final time");

    entry = "stateSamplingFreq";
    if (node[entry]) stateSamplingFreq_ = node[entry].as<int>();
    else throw std::runtime_error("Input: missing state sampling freq");

    entry = "rhsSamplingFreq";
    if (node[entry]) rhsSamplingFreq_ = node[entry].as<int>();

    entry = "nonlinearSolverName";
    if (node[entry]){
      nonlinSolverStr_ = node[entry].as<std::string>();
    }
    else if (pressio::ode::is_implicit_scheme(odeScheme_)){
      // solvers are required only when doing implicit time integration
      throw std::runtime_error("Input: missing solver name");
    }

    entry = "nonlinearSolverTolerance";
    if (node[entry]){
      nonlinSolverTol_ = node[entry].as<ScalarType>();
    }
    else if (pressio::ode::is_implicit_scheme(odeScheme_)){
      // solvers are required only when doing implicit time integration
      throw std::runtime_error("Input: missing solver tolerance");
    }

    numSteps_ = static_cast<int>(finalTime_/dt_);

    this->validate();
    //this->describe();
  }

  void validate() const{
    if (meshDirPath_=="") {
      throw std::runtime_error("meshdir is empty string");
    }

    if (numDofsPerCell_ <= 0) {
      throw std::runtime_error("numDofsPerCell must be > 0");
    }

    if (dt_ <= static_cast<ScalarType>(0)) {
      throw std::runtime_error("Cannot have dt <= 0");
    }

    if (finalTime_ <= static_cast<ScalarType>(0)) {
      throw std::runtime_error("Cannot have finalT <= 0");
    }

    if (stateSamplingFreq_<=0) {
      throw std::runtime_error("Cannot have state sampling freq <= 0");
    }
  }
};

template <typename ScalarType>
class ParserRom
{
  bool isRom_ = false;
  std::string romAlgoName_ = "empty";
  int romSize_= {};
  std::string romFullMeshPodBasisFileName_ = "empty";
  std::string romInitialStateFileName_ = "empty";
  std::string romAffineShiftFileName_ = "empty";
  std::string romGalerkinHypRedOperatorFileName_ = "empty";
  std::string romStencilMeshGidsFileName_ = "empty";
  std::string romSampleMeshGidsFileName_ = "empty";
  bool isAffine_ = false;

public:
  ParserRom() = delete;
  ParserRom(YAML::Node & node){
    this->parseImpl(node);
  }

  auto romOn()                   const{ return isRom_; }
  auto romAlgorithm()            const{ return romAlgoName_; }
  int  romModeCount()            const{ return romSize_; }
  auto romFullMeshPodBasisFile() const{ return romFullMeshPodBasisFileName_; }
  auto romInitialStateFile()     const{ return romInitialStateFileName_; }
  auto romAffineShiftFile()	 const{ return romAffineShiftFileName_; }
  auto romIsAffine()             const{ return isAffine_; }
  auto romGalerkinHypRedOperatorFile() const{ return romGalerkinHypRedOperatorFileName_; }

  auto romStencilMeshGidsFile()	 const{ return romStencilMeshGidsFileName_; }
  auto romSampleMeshGidsFile()	 const{ return romSampleMeshGidsFileName_; }

  void describe() const{
    std::cout << "\nromAlgoName_ = " << romAlgoName_ << " \n"
	      << "romSize_ = "   << romSize_ << " \n"
	      << "romFullMeshPodBasisFileName_ = " << romFullMeshPodBasisFileName_ << " \n";
  }

private:
  void parseImpl(YAML::Node & parentNode)
  {
    auto romNode = parentNode["rom"];
    if (romNode){
      isRom_ = true;

      std::string entry = "algorithm";
      if (romNode[entry]){
	romAlgoName_ = romNode[entry].as<std::string>();
	std::transform(romAlgoName_.begin(), romAlgoName_.end(), romAlgoName_.begin(),
		       [](int c){ return std::tolower(c); });
      }
      else throw std::runtime_error("Input: ROM: missing " + entry);

      entry = "numModes";
      if (romNode[entry]) romSize_ = romNode[entry].as<int>();
      else throw std::runtime_error("Input: ROM: missing " + entry);

      entry = "fullMeshPodFile";
      if (romNode[entry]) romFullMeshPodBasisFileName_ = romNode[entry].as<std::string>();
      else throw std::runtime_error("Input: ROM: missing " + entry);

      entry = "romInitialStateFile";
      if (romNode[entry]) romInitialStateFileName_ = romNode[entry].as<std::string>();
      else throw std::runtime_error("Input: ROM: missing " + entry);

      entry = "affineShiftFile";
      if (romNode[entry]) romAffineShiftFileName_ = romNode[entry].as<std::string>();
      // else affine is not used

      entry = "isAffine";
      if (romNode[entry]) isAffine_ = romNode[entry].as<bool>();

      entry = "galerkinHypRedOperatorFile";
      if (romNode[entry]) romGalerkinHypRedOperatorFileName_ = romNode[entry].as<std::string>();

      entry = "stencilMeshGidsFile";
      if (romNode[entry]) romStencilMeshGidsFileName_ = romNode[entry].as<std::string>();

      entry = "sampleMeshGidsFile";
      if (romNode[entry]) romSampleMeshGidsFileName_ = romNode[entry].as<std::string>();
    }
  }
};


template<class ScalarType>
class ParserTwoDimShallowWater
  : public ParserCommon<ScalarType>,
    public ParserRom<ScalarType>
{
  using base_common_t = ParserCommon<ScalarType>;
  using rom_common_t  = ParserRom<ScalarType>;

  std::string inviscidFluxRec_ = "";
  // gravity, coriolis, pulseMagnitude
  std::array<ScalarType, 3> params_ = {9.8, -3.0, 0.125};

public:
  ParserTwoDimShallowWater() = delete;

  ParserTwoDimShallowWater(YAML::Node & node)
    : ParserCommon<ScalarType>(node),
      ParserRom<ScalarType>(node)
  {
    this->parseImpl(node);
  }

  auto inviscidFluxReconstruction() const{ return inviscidFluxRec_; }
  auto gravity()  const{ return params_[0]; }
  auto coriolis() const{ return params_[1]; }
  auto pulseMagnitude() const{ return params_[2]; }

  void describe() const
  {
    base_common_t::describe();
    rom_common_t::describe();

    std::cout << "\ninviscidFluxReconstruction = " << inviscidFluxRec_ << " \n"
	      << "gravity = "   << params_[0] << " \n"
	      << "coriolis  = " << params_[1] << " \n"
	      << "pulse_mag = " << params_[2] << " \n";
  }

private:
  void parseImpl(YAML::Node & node)
  {
    if (node["inviscidFluxReconstruction"]){
      inviscidFluxRec_ = node["inviscidFluxReconstruction"].as<std::string>();
    }
    else{
      throw std::runtime_error("Input: SWE: missing inviscid reconstruction");
    }

    const std::array<std::string, 3> names = {"gravity", "coriolis", "pulsemag"};
    for (int i=0; i<3; ++i){
      if (node[names[i]]){
	params_[i] = node[names[i]].as<ScalarType>();
      }
      else{
	throw std::runtime_error("Cannot find " + names[i] + "in input file");
      }
    }

    this->validate();
  }

  void validate() const{
    if (inviscidFluxRec_ != "FirstOrder" &&
	inviscidFluxRec_ != "Weno3" &&
	inviscidFluxRec_ != "Weno5")
    {
      throw std::runtime_error
	("Input: SWE: inviscidFluxReconstruction must be: FirstOrder, Weno3, or Weno5");
    }

    if (params_[0]<=0.) {
      throw std::runtime_error("Input: SWE: Cannot have negative gravity");
    }
    if (params_[2]<=0.) {
      throw std::runtime_error("Input: SWE: Cannot have negative pulse magnitude");
    }
  }
};

#endif




// template<class ScalarType>
// class ParserTwoDimGrayScott
//   : public ParserCommon<ScalarType>,
//     public ParserFom<ScalarType>,
//     public ParserRom<ScalarType>
// {
//   ScalarType diffusionA_ = {};
//   ScalarType diffusionB_ = {};
//   ScalarType feedRate_   = {};
//   ScalarType killRate_   = {};

// public:
//   ParserTwoDimGrayScott() = delete;

//   ParserTwoDimGrayScott(YAML::Node & parentNode)
//     : ParserCommon<ScalarType>(parentNode),
//       ParserFom<ScalarType>(parentNode),
//       ParserRom<ScalarType>(parentNode)
//   {
//     // check if we are doing FOM or ROM
//     const auto isFom = this->doingFom();
//     const auto isRom = this->doingRom();
//     if (isFom && isRom){
//       throw std::runtime_error("Seems like both FOM and ROM are on, this is not possible");
//     }

//     // find coefficients
//     {
//       const auto node = parentNode["physicalCoefficients"];
//       if (node)
// 	{
// 	  auto entry = "diffusionA";
// 	  if (node[entry]) diffusionA_ = node[entry].as<ScalarType>();
// 	  else throw std::runtime_error("missing diffusionA");

// 	  entry = "diffusionB";
// 	  if (node[entry]) diffusionB_ = node[entry].as<ScalarType>();
// 	  else throw std::runtime_error("missing diffusionB");

// 	  entry = "feedRate";
// 	  if (node[entry]) feedRate_ = node[entry].as<ScalarType>();
// 	  else throw std::runtime_error("missing feedRate");

// 	  entry = "killRate";
// 	  if (node[entry]) killRate_ = node[entry].as<ScalarType>();
// 	  else throw std::runtime_error("missing killRate");
// 	}
//       else{
// 	throw std::runtime_error("2d GrayScott: missing coefficients section in yaml input");
//       }
//     }

//     this->describe();
//   }

//   auto diffusionA() const{ return diffusionA_; }
//   auto diffusionB() const{ return diffusionB_; }
//   auto feedRate() const{ return feedRate_; }
//   auto killRate() const{ return killRate_; }

// private:
//   void describe() const{
//     std::cout << "\ndiffusionA = " << diffusionA_ << " \n"
// 	      << "diffusionB = " << diffusionB_ << " \n"
// 	      << "killRate = " << killRate_ << " \n"
// 	      << "feedRate = " << feedRate_ << " \n";
//   }
// };


// template<class ScalarType>
// class ParserTwoDimReacDiff
//   : public ParserCommon<ScalarType>,
//     public ParserRom<ScalarType>
// {
//   ScalarType diffusion_ = 0.01;
//   ScalarType reaction_  = 0.02;

// public:
//   ParserTwoDimReacDiff() = delete;

//   ParserTwoDimReacDiff(YAML::Node & node0)
//     : ParserCommon<ScalarType>(node0),
//       ParserRom<ScalarType>(node0)
//   {
//     const auto node = node0["parameters"];
//     if (node)
//     {
//       auto entry = "diffusion";
//       if (node[entry]) diffusion_ = node[entry].as<ScalarType>();
//       else throw std::runtime_error("missing diffusion");

//       entry = "reaction";
//       if (node[entry]) reaction_ = node[entry].as<ScalarType>();
//       else throw std::runtime_error("missing reaction");
//     }
//     else{
//       throw std::runtime_error("General section in yaml input is mandatory!");
//     }
//     this->validate();
//     this->describe();
//   }

//   auto diffusion() const{ return diffusion_; }
//   auto reaction() const{ return reaction_; }

// private:
//   void validate() const{
//     if (diffusion_<=0.) {
//       throw std::runtime_error("Cannot have negative diffusion");
//     }
//     if (reaction_<=0.){
//       throw std::runtime_error("Cannot have negative reaction");
//     }
//   }

//   void describe() const{
//     std::cout << "\ndiffusion = " << diffusion_ << " \n"
// 	      << "reaction = " << reaction_ << " \n";
//   }
// };


#include "CONTAINERS_ALL"
#include "ODE_ALL"
#include "SOLVERS_NONLINEAR"

int main(int argc, char *argv[]){

  // // parse input file
  // InputParser parser;
  // int err = parser.parse(argc, argv);
  // if (err == 1) return 1;

  // // get the problem name and stepper
  // const auto problemName    = parser.problemName_;
  // const auto odeStepperName = parser.odeStepperName_;

  // if ( problemName.compare("ms") == 0 ){
  //   // types
  //   using pt = MSTypes;

  //   // functors to use
  //   typename pt::adv_fnct_t advFunctor;
  //   typename pt::src_fnct_t srcFunctor(parser.D_);

  //   if (odeStepperName.compare("RungeKutta4") == 0 ){
  //     runRungeKutta4<pt>(parser, srcFunctor, advFunctor);
  //   }
  //   if (odeStepperName.compare("bdf1") == 0 ){
  //     runBDF1<pt>(parser, srcFunctor, advFunctor);
  //   }
  // }
  // if ( problemName.compare("chemABC") == 0 ){
  //   // types
  //   using pt = ChemTypes;

  //   // functors to use
  //   typename pt::adv_fnct_t advFunctor;
  //   typename pt::src_fnct_t srcFunctor(parser.K_);

  //   if (odeStepperName.compare("RungeKutta4") == 0){
  //     runRungeKutta4<pt>(parser, srcFunctor, advFunctor);
  //   }
  //   if (odeStepperName.compare("bdf1") == 0 ){
  //     runBDF1<pt>(parser, srcFunctor, advFunctor);
  //   }
  // }
  // else{
  //   std::cout << "Invalid problemName in input file " << std::endl;
  // }

  return 0;
}

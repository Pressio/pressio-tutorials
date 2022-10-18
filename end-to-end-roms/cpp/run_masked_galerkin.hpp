
#ifndef RUN_MASKED_GALERKIN_HPP_
#define RUN_MASKED_GALERKIN_HPP_

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

// template<class ScalarType, class FomVeloType>
// struct Masker
// {
//   std::vector<int> indices_;
//   int numDofsPerCell_ = {};

//   template<class ParserType>
//   Masker(const ParserType & parser)
//     : numDofsPerCell_(parser.numDofsPerCell())
//   {
//     fill_gids_stdvector_from_ascii(parser.sampleMeshGidsFileName(), indices_);
//   }

//   FomVeloType createApplyMaskResult(const FomVeloType & /*unmasked_object*/) const{
//     return FomVeloType( numDofsPerCell_*indices_.size() );
//   }

//   template<class TimeType>
//   void operator()(const FomVeloType & unmasked_object,
//                   const TimeType time,
//                   FomVeloType & result) const
//   {
//     for (int i=0; i<indices_.size(); ++i){
//       for (int k=0; k<numDofsPerCell_; ++k){
// 	const int r = i*numDofsPerCell_ + k;
// 	const int g = indices_[i]*numDofsPerCell_ + k;
// 	result(r) = unmasked_object(g);
//       }
//     }
//   }
// };

#endif

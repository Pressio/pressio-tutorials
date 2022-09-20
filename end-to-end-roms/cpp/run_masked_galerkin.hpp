
#ifndef RUN_MASKED_GALERKIN_HPP_
#define RUN_MASKED_GALERKIN_HPP_

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

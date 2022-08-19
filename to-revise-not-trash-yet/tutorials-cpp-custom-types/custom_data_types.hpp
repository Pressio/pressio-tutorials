
#ifndef PRESSIO_TUTORIALS_CUSTOM_DATA_TYPES_EXAMPLE_HPP_
#define PRESSIO_TUTORIALS_CUSTOM_DATA_TYPES_EXAMPLE_HPP_

#include "pressio/type_traits.hpp"

template<class ScalarType>
struct CustomVector
{
  using value_type = ScalarType;
  using size_type = std::size_t;

  CustomVector(std::size_t ext) : d_(ext){}

  ScalarType & operator()(int i){ return d_[i]; }
  const ScalarType & operator()(int i)const { return d_[i]; }
  ScalarType & operator[](int i){ return d_[i]; }
  const ScalarType & operator[](int i)const { return d_[i]; }

  std::size_t extent(int k)const { return (k==0) ? d_.size() : 0; }

  void fill(ScalarType value){
    std::for_each(d_.begin(), d_.end(), [](ScalarType & v){ v= 0.; });
  }

private:
  std::vector<ScalarType> d_ = {};
};

template<class ScalarType>
struct CustomMatrix
{
  using value_type = ScalarType;
  using size_type = std::size_t;

  CustomMatrix(std::size_t nr, std::size_t nc)
    : num_rows_(nr), num_cols_(nc), d_(nr*nc){}

  std::size_t extent(int k)const { return (k==0) ? num_rows_ : num_cols_; }

  ScalarType & operator()(int i, int j){ return d_[num_cols_*i+j]; }
  const ScalarType & operator()(int i, int j) const { return d_[num_cols_*i+j]; }

  void fill(ScalarType value){
    std::for_each(d_.begin(), d_.end(), [=](ScalarType & v){ v= value; });
  }

private:
  std::size_t num_rows_ = {};
  std::size_t num_cols_ = {};
  std::vector<ScalarType> d_ = {};
};

#endif

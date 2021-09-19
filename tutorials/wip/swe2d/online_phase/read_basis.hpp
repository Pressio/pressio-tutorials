
#ifndef PRESSIO_TUT_SWE_READ_BASIS_HPP_
#define PRESSIO_TUT_SWE_READ_BASIS_HPP_

#include "pressio_rom_lspg.hpp"
#include "pressio_apps.hpp"

template <typename T = std::size_t>
auto convertFromVVecToMultiVec(
      const std::vector<std::vector<double>> & A0,
      T nrows, T ncols)
  -> pressio::containers::MultiVector<Eigen::MatrixXd>{

  using eig_mat = Eigen::MatrixXd;
  pressio::containers::MultiVector<eig_mat> ADW(nrows, ncols);

  for (T i=0; i<nrows; i++){
    for (T j=0; j<ncols; j++)
      ADW(i,j) = A0[i][j];
  }
  return ADW;
}

template <typename T = std::size_t>
auto readBasis(std::string filename, T romSize, T numCell)
  ->pressio::containers::MultiVector<Eigen::MatrixXd>
{
  std::vector<std::vector<double>> A0;
  ::pressio::utils::readAsciiMatrixStdVecVec(filename, A0, romSize);
  // read basis into a MultiVector
  auto phi = convertFromVVecToMultiVec(A0, numCell, romSize);
  //  phi.data()->Print(std::cout);
  return phi;
}

#endif

#include <fstream>
#include <iostream>
#include <vector>
#include "rom_shared.hpp"
#include "Eigen/Dense"

void test1(std::string & output)
{
  if (!file_exists("fake_affine_shift.txt")){
    output = "FAILED";
  }
  if (!file_exists("fake_basis.bin")){
    output = "FAILED";
  }
  if (file_exists("nonexistingfile.txt")){
    output = "FAILED";
  }
}

void test2(std::string & output)
{
  auto v = create_affine_shift_and_read_from_file<double>("fake_affine_shift.txt");
  Eigen::VectorXd gold(14);
  gold <<
    0.5887616795882755,
    0.8375978846318775,
    0.9156299722466359,
    0.0266658519411507,
    0.7168849214696587,
    0.5542663755800283,
    0.9345144858315367,
    0.9192516782366369,
    0.4797990840539219,
    0.4150397183277343,
    0.3581108452610327,
    0.5254241283347701,
    0.590502243075904,
    0.1993476466613442;
  if (!gold.isApprox(v)){ output = "FAILED"; }
}

void test3(std::string & output)
{
  auto M = create_basis_and_read_from_file<double>("fake_basis.bin", 3);
  Eigen::MatrixXd gold(14, 3);
  gold <<
    0.584866866852153,  0.6441937222647023, 0.5422347245405696,
    0.6738086462243329, 0.3545599789401127, 0.2292073961447653,
    0.613541196572706,  0.9130129912614364, 0.0952304434396564,
    0.2889627425441876, 0.8937351772586412, 0.7047343958137176,
    0.3887132220564247, 0.6873843380114618, 0.5640435917679026,
    0.1382712521738086, 0.9336384405594449, 0.8269981890300937,
    0.0373036838304311, 0.323591131787742,  0.4638137602005488,
    0.8881013872587351, 0.1881521004603567, 0.7191516687094873,
    0.7110619989188681, 0.471473806338619,  0.5826342470663713,
    0.208115561627496,  0.8309938609602806, 0.481739038378178,
    0.3561414595188699, 0.5170396799731559, 0.4101273764503535,
    0.9672549011289965, 0.0698963103338164, 0.2618931947323966,
    0.6570840506268659, 0.1872303737013714, 0.8837671286854955,
    0.9563943183077489, 0.9547372145317032, 0.1253634890230926;
  if (!gold.isApprox(M)){ output = "FAILED"; }
}

void test4(std::string & output)
{
  const auto stencilMeshGids = create_cell_gids_vector_and_fill_from_ascii("fake_stencil_mesh_gids.txt");
  Eigen::VectorXi gold(5);
  gold << 1,2,3,5,6;
  if (!gold.isApprox(stencilMeshGids)){ output = "FAILED"; }
}

void test5(std::string & output)
{
  const auto sampleMeshGids = create_cell_gids_vector_and_fill_from_ascii("fake_sample_mesh_gids.txt");
  Eigen::VectorXi gold(5);
  gold << 2,3,6;
  if (!gold.isApprox(sampleMeshGids)){ output = "FAILED"; }
}

void test6(std::string & output)
{
  const auto numDofsPerCell = 2;
  auto M = create_basis_and_read_from_file<double>("fake_basis.bin", 3);
  const auto stencilMeshGids = create_cell_gids_vector_and_fill_from_ascii("fake_stencil_mesh_gids.txt");
  const auto phiStencil = create_and_reduce_on_stencil_mesh(M, stencilMeshGids, numDofsPerCell);
  Eigen::MatrixXd gold(10, 3);
  gold <<
    0.613541196572706,  0.9130129912614364, 0.0952304434396564,
    0.2889627425441876, 0.8937351772586412, 0.7047343958137176,
    0.3887132220564247, 0.6873843380114618, 0.5640435917679026,
    0.1382712521738086, 0.9336384405594449, 0.8269981890300937,
    0.0373036838304311, 0.323591131787742,  0.4638137602005488,
    0.8881013872587351, 0.1881521004603567, 0.7191516687094873,
    0.3561414595188699, 0.5170396799731559, 0.4101273764503535,
    0.9672549011289965, 0.0698963103338164, 0.2618931947323966,
    0.6570840506268659, 0.1872303737013714, 0.8837671286854955,
    0.9563943183077489, 0.9547372145317032, 0.1253634890230926;
  if (!gold.isApprox(phiStencil)){ output = "FAILED"; }
}

void test7(std::string & output)
{
  const auto numDofsPerCell = 2;
  auto shift = create_affine_shift_and_read_from_file<double>("fake_affine_shift.txt");
  const auto stencilMeshGids = create_cell_gids_vector_and_fill_from_ascii("fake_stencil_mesh_gids.txt");
  const auto shiftStencil = create_and_reduce_on_stencil_mesh(shift, stencilMeshGids, numDofsPerCell);
  Eigen::VectorXd gold(10);
  gold <<
    0.9156299722466359,
    0.0266658519411507,
    0.7168849214696587,
    0.5542663755800283,
    0.9345144858315367,
    0.9192516782366369,
    0.3581108452610327,
    0.5254241283347701,
    0.590502243075904,
    0.1993476466613442;
  if (!gold.isApprox(shiftStencil)){ output = "FAILED"; }
}

int main()
{
  std::string output = "PASS";
  test1(output); std::cout << output << "\n";
  test2(output); std::cout << output << "\n";
  test3(output); std::cout << output << "\n";
  test4(output); std::cout << output << "\n";
  test5(output); std::cout << output << "\n";
  test6(output); std::cout << output << "\n";
  test7(output); std::cout << output << "\n";

  return 0;
}

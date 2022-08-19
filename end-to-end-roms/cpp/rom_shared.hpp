
#ifndef ROM_SHARED_HPP_
#define ROM_SHARED_HPP_

#include "Eigen/Dense"
#include <chrono>

bool file_exists(const std::string & fileIn){
  std::ifstream infile(fileIn);
  return (infile.good() != 0);
}

template<class ScalarType>
auto create_basis_and_read_from_file(const std::string & fileName,
				     int numModes)
{
  using matrix_type = Eigen::Matrix<ScalarType, -1, -1, Eigen::ColMajor>;

  if (!file_exists(fileName)){
    throw std::runtime_error("Cannot find file: " + fileName);
  }

  matrix_type M;
  using int_t = typename matrix_type::Index;
  using sc_t  = typename matrix_type::Scalar;
  std::ifstream fin(fileName, std::ios::in | std::ios::binary);
  fin.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  std::size_t rows={};
  std::size_t cols={};
  fin.read((char*) (&rows),sizeof(std::size_t));
  fin.read((char*) (&cols),sizeof(std::size_t));
  M.resize(rows, numModes);

  // we can do this read because file is binary and we are working
  // with colmajor matrix
  const auto nBytes = rows*numModes*sizeof(sc_t);
  fin.read( (char *) M.data(), nBytes );

  if (!fin){
    std::cout << std::strerror(errno) << std::endl;
    throw std::runtime_error("ERROR READING binary file");
  }
  else{
    std::cout << fin.gcount() << " bytes read\n";
  }
  fin.close();

  return M;
}

template<class ScalarType>
auto create_affine_shift_and_read_from_file(const std::string & fileName)
{
  using result_type = Eigen::Matrix<ScalarType, -1, 1>;

  if (!file_exists(fileName)){
    throw std::runtime_error("Cannot find file: " + fileName);
  }

  std::ifstream source;
  source.open(fileName, std::ios_base::in);
  std::string line, value;
  std::vector<ScalarType> v;
  while (std::getline(source, line) ){
    std::istringstream in(line);
    in >> value;
    v.push_back(std::atof(value.c_str()));
  }
  source.close();

  result_type result(v.size());
  for (std::size_t i=0; i<v.size(); ++i){
    result[i] = v[i];
  }
  return result;
}

template<class ScalarType>
void fill_rom_state_from_ascii(const std::string & fileName,
			       Eigen::Matrix<ScalarType, -1, 1> & romState)
{
  if (!file_exists(fileName)){
    throw std::runtime_error("Cannot find file: " + fileName);
  }

  std::ifstream source;
  source.open(fileName, std::ios_base::in);
  std::string line, value;
  std::vector<ScalarType> v;
  while (std::getline(source, line) ){
    std::istringstream in(line);
    in >> value;
    v.push_back(std::atof(value.c_str()));
  }
  source.close();

  assert(v.size() == (std::size_t) romState.size());
  for (int i=0; i<v.size(); ++i){
    romState[i] = v[i];
  }
}

// template <typename T>
// void fill_gids_stdvector_from_ascii(const std::string & fileName,
// 				    std::vector<T> & v)
// {
//   if (!file_exists(fileName)){
//     throw std::runtime_error("Cannot find file: " + fileName);
//   }

//   assert(v.size() == 0);

//   std::ifstream source;
//   source.open(fileName, std::ios_base::in);
//   std::string line, colv;
//   while (std::getline(source, line) ){
//     std::istringstream in(line);
//     in >> colv;
//     v.push_back(std::atoi(colv.c_str()));
//   }
//   source.close();
// }

// template <typename T>
// auto create_gids_stdvector_and_fill_from_ascii(const std::string & fileName)
// {
//   std::vector<T> v;
//   fill_gids_stdvector_from_ascii(fileName, v);
//   return v;
// }

// template<class ScalarType, class ParserType>
// auto create_basis_on_stencil_mesh(const Eigen::Matrix<ScalarType, -1, -1, Eigen::ColMajor> & phiFull,
// 				  const ParserType & parser)
// {
//   const auto numDofsPerCell = parser.numDofsPerCell();
//   auto stencilMeshGids = create_gids_stdvector_and_fill_from_ascii<int>(parser.stencilMeshGidsFileName());
//   const auto totStencilDofs = stencilMeshGids.size()*numDofsPerCell;

//   Eigen::Matrix<ScalarType, -1, -1, Eigen::ColMajor> phi(totStencilDofs, phiFull.cols());
//   for (int i=0; i<stencilMeshGids.size(); ++i)
//   {
//     for (int k=0; k<numDofsPerCell; ++k){
//       const int row = i*numDofsPerCell + k;
//       const int ind = stencilMeshGids[i]*numDofsPerCell + k;
//       for (int j=0; j<phi.cols(); ++j){
// 	phi(row, j) = phiFull(ind, j);
//       }
//     }
//   }
//   return phi;
// }

// template<class ScalarType, class ParserType>
// auto create_reference_state_on_stencil_mesh(const ParserType & parser)
// {
//   auto refStateFull = create_full_reference_state_and_load_from_ascii<ScalarType>(parser.referenceStateFile());

//   const auto numDofsPerCell = parser.numDofsPerCell();
//   auto stencilMeshGids = create_gids_stdvector_and_fill_from_ascii<int>(parser.stencilMeshGidsFileName());

//   Eigen::Matrix<ScalarType, -1, 1> result(stencilMeshGids.size()*numDofsPerCell);
//   for (int i=0; i<stencilMeshGids.size(); ++i)
//   {
//     for (int k=0; k<numDofsPerCell; ++k){
//       const int row = i*numDofsPerCell + k;
//       const int ind = stencilMeshGids[i]*numDofsPerCell + k;
//       result[row] = refStateFull[ind];
//     }
//   }
//   return result;
// }

#endif

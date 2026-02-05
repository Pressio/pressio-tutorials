#include <cmath>
#include <vector>
#include <algorithm>

#include <pressio/rom.hpp>

/**
 * To build a hyperreduced ROM, we need to define a hyperreduction
 * functor that will be used to project the sampled RHS to the
 * reduced RHS. In this example, we will use an explicit Galerkin
 * hyperreduction approach similar to the one described in
 *
 *    "Hyper-reduction of nonlinear operators and the discrete empirical interpolation method
 *     for large nonlinear structural dynamics models"
 *     Chaturantabut and Sorensen, 2010, SIAM J. Sci. Comput.
 *
 * The hyperreducer functor will take as input the full RHS vector,
 * sample it at selected indices, and then project it using a
 * precomputed matrix H.
 */

// Hyperreducer functor: projects sampled RHS to reduced RHS
template <typename vector_t, typename matrix_t>
class ExplicitGalerkinHyperReducer
{
    matrix_t H_;            // r x m
    std::vector<int> samp_; // size m

public:
    ExplicitGalerkinHyperReducer(matrix_t H, std::vector<int> samp)
      : H_(std::move(H)), samp_(std::move(samp)) {}

    template<class RhsType, class ResultType>
    void operator()(const RhsType& rhsFull, const double&, ResultType& result) const
    {
        const int m = static_cast<int>(samp_.size());
        vector_t sampled(m);
        for (int j = 0; j < m; ++j) sampled(j) = rhsFull(samp_[j]);
        result = H_ * sampled;
    }
};

/**
 * Typical hyperreducers would use a more refined sampling strategy,
 * such as the Discrete Empirical Interpolation Method (DEIM) or
 * Q-DEIM. Here, for simplicity, we use uniform striding to select
 * sample indices.
 */
std::vector<int> make_stride_samples(std::size_t N, std::size_t m)
{
    m = std::max<std::size_t>(1, std::min(N, m));
    std::vector<int> idx;
    idx.reserve(m);
    const double stride = static_cast<double>(N) / static_cast<double>(m);
    for (std::size_t j = 0; j < m; ++j) {
        int i = static_cast<int>(std::floor(j * stride)) % static_cast<int>(N);
        idx.push_back(i);
    }
    return idx;
}

template <typename vector_t, typename matrix_t>
auto buildHyperReducer(const matrix_t& rhsSnaps, auto& trialSpace) {
    // POD on RHS snapshots
    Eigen::JacobiSVD<matrix_t> svdRhs(rhsSnaps, Eigen::ComputeThinU | Eigen::ComputeThinV);
    const int r = static_cast<int>(trialSpace.basisOfTranslatedSpace().cols());
    int K = std::max(r+1, std::min<int>(rhsSnaps.rows(), 3*r + 1));
    matrix_t Theta = svdRhs.matrixU().leftCols(K);

    // Sample selection (via striding here for simplicity)
    const int N = static_cast<int>(rhsSnaps.rows());
    const int m = std::min(N, std::max(K, std::max(r+1, 20))); // at least r+1, K, 20
    auto samp = make_stride_samples(N, m);

    // Slice Theta at sample points -> m x K
    matrix_t ThetaS(m, K);
    for (int j = 0; j < m; ++j) ThetaS.row(j) = Theta.row(samp[j]);

    // Phi
    matrix_t Phi = trialSpace.basisOfTranslatedSpace(); // N x r

    // Compute H = (Phi^T Theta) * pinv(ThetaS)
    matrix_t cross = Phi.transpose() * Theta; // r x K
    matrix_t ThetaS_pinv = pinv<matrix_t>(ThetaS);

    PRESSIOLOG_DEBUG("hyper dims: r={} K={} m={} cross=({},{}) pinv=({},{})",
                     r, K, m, cross.rows(), cross.cols(), ThetaS_pinv.rows(), ThetaS_pinv.cols());

    matrix_t H = cross * ThetaS_pinv; // r x m

    // Hyperreducer functor
    ExplicitGalerkinHyperReducer<vector_t, matrix_t> hyperreducer(std::move(H), std::move(samp));

    return hyperreducer;
}

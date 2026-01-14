#include <pressio/rom.hpp>

/**
 * Determine the FOM initial condition
 */

template <typename FomSystem, typename vector_t>
vector_t computeInitialCondition( const FomSystem& fom )
{
    // Compuite the initial condition: e.g., u(x,0) = sin(2*pi*x/L)
    vector_t u0( fom.N() );
    for ( std::size_t i = 0; i < fom.N(); ++i ) {
        double x = i * fom.dx();
        u0( i ) = std::sin( 2.0 * M_PI * x / ( fom.dx() * fom.N() ) );
    }
    return u0;
}

/**
 * Computes the POD basis from snapshots and constructs the trial
 * subspace with the basis and affine shift.
 */
template<typename FomSystemType, typename matrix_t, typename vector_t>
auto snapshots_to_trial_space( const matrix_t& snapshots, const FomSystemType& fom )
{
    // Compute the SVD of the snapshot matrix
    Eigen::JacobiSVD< matrix_t > svd(
        snapshots,
        Eigen::ComputeThinU | Eigen::ComputeThinV
    );

    // Extract the first r left singular vectors as the POD basis
    int r = 5;
    matrix_t basis = svd.matrixU().leftCols( r ).eval();

    // Determine the affine shift (mean of snapshots)
    vector_t affineShift = snapshots.rowwise().mean().eval();

    // Construct the trial subspace
    auto trialSpace = pressio::rom::create_trial_column_subspace<vector_t>(
        basis, affineShift, false
    );

    return trialSpace;
}

/**
 * Projects the FOM initial condition onto the reduced basis
 * to initialize the reduced state.
 */
template<typename FomSystemType, typename vector_t, typename TrialSpaceType>
auto trial_space_to_reduced_state( const TrialSpaceType& trialSpace, const FomSystemType& fom )
{
    // Create the reduced state vector
    auto reducedState = trialSpace.createReducedState();

    // Reconstruct the same initial condition that the FOM used
    // For Burgers: u(x, 0) = sin(2*pi*x/L)
    auto u0 = computeInitialCondition< FomSystemType, vector_t >( fom );

    // Project the initial condition onto the reduced basis:
    // reducedState = basis^T * (u0 - affineShift)
    vector_t centered = u0 - trialSpace.translationVector();
    reducedState = trialSpace.basisOfTranslatedSpace().transpose() * centered;

    // Return the reduced state
    return reducedState;
}

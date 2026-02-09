#include <fstream>
#include <iomanip>
#include <filesystem>

#include <pressio/rom.hpp>

/**
 * Observer to capture ROM solutions at each timestep
 * Signature: void operator()(StepCount, Time, State const&) const
 */
template <typename vector_t, typename TrialSpaceType>
struct RomObserver
{
    std::vector<vector_t>& trajectory;
    const TrialSpaceType& trialSpace;
    mutable int stepCount;

    RomObserver(std::vector<vector_t>& traj, const TrialSpaceType& ts)
        : trajectory(traj), trialSpace(ts), stepCount(0) {}

    // Observer call signature required by Pressio
    template <typename StepCountType, typename TimeType, typename StateType>
    void operator()(StepCountType step, TimeType /*time*/, const StateType& reducedState) const
    {
        // Capture every timestep
        auto fullState = trialSpace.createFullStateFromReducedState(reducedState);
        trajectory.push_back(fullState);
        ++stepCount;
    }
};

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

// Simple pseudoinverse using SVD
template<typename matrix_t>
matrix_t pinv(const matrix_t& A, double tol = 1e-10)
{
    Eigen::JacobiSVD<matrix_t> svd(A, Eigen::ComputeThinU | Eigen::ComputeThinV);
    const auto& S = svd.singularValues();
    const int r = static_cast<int>(S.size());

    // Build S^{-1} as r x r
    matrix_t Sinv = matrix_t::Zero(r, r);
    for (int i = 0; i < r; ++i)
        if (S(i) > tol) Sinv(i,i) = 1.0 / S(i);

    // Using thin SVD: A = U_r S_r V_r^T => A^+ = V_r S_r^{-1} U_r^T
    matrix_t V_r = svd.matrixV().leftCols(r); // n x r
    matrix_t U_r = svd.matrixU().leftCols(r); // m x r
    return (V_r * Sinv * U_r.transpose()).eval();
}

/**
 * Write solutions to CSV file in the specified output directory
 */
template <typename vector_t>
void writeTrajectoryToCSV( const std::string& outputDir,
                           const std::string& filename,
                           const std::vector< vector_t >& trajectory )
{
    // Ensure directory exists
    std::filesystem::create_directories(outputDir);

    // Construct full file path
    std::filesystem::path filepath = std::filesystem::path(outputDir) / filename;

    std::ofstream file(filepath);
    for (int i = 0; i < trajectory[0].size(); ++i) {
        file << "x" << i;
        if (i < trajectory[0].size() - 1) file << ",";
    }
    file << "\n";

    // Write data: each row is one timestep
    for (const auto& state : trajectory) {
        for (int i = 0; i < state.size(); ++i) {
            file << std::scientific << std::setprecision(12) << state(i);
            if (i < state.size() - 1) file << ",";
        }
        file << "\n";
    }

    file.close();
}

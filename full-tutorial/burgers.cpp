/**
 * End to end demo for solving 1D Burgers' equation using reduced-order
 * modeling with Pressio.
 *
 * MATHEMATICAL FORMULATION
 * =====================
 *
 * The one-dimensional Burgers' equation is a classic nonlinear PDE
 * combining advection and diffusion:
 *
 *   ∂u/∂t + u * ∂u/∂x = ν * ∂²u/∂x²                        [1]
 *
 * where:
 *   - u(x, t) is the velocity field (scalar in 1D)
 *   - ν (nu) is the kinematic viscosity parameter
 *   - The domain is periodic: x ∈ [0, L] with u(0,t) = u(L,t)
 *   - Initial condition: u(x, 0) = u₀(x)
 *
 * PHYSICAL INTERPRETATION
 * =======================
 *
 * Burgers' equation models the balance between:
 *
 *   1. CONVECTIVE TRANSPORT (left side): u*∂u/∂x
 *      A fluid parcel carries its momentum forward. Steep gradients
 *      can steepen further (nonlinear shock formation). This term is
 *      NONLINEAR and generates fine-scale structures.
 *
 *   2. VISCOUS DIFFUSION (right side): ν*∂²u/∂x²
 *      Friction smooths velocity gradients, dissipating kinetic
 *      energy. High viscosity => smooth, diffuse solution. Low
 *      viscosity => sharp fronts, potential shock layers.
 *
 * The competing dynamics (steepening vs smoothing) produce rich
 * behavior: traveling waves, shock-like structures, and energy
 * cascade from large to small scales before dissipation.
 *
 * SPATIAL DISCRETIZATION
 * ======================
 *
 * We discretize on a uniform grid x_i = i*Δx, i = 0,...,N-1:
 *
 *   du_i/dt = -1/2 * (u²_{i+1} - u²_{i-1}) / (2Δx)
 *           + ν * (u_{i+1} - 2u_i + u_{i-1}) / Δx²        [2]
 *
 * Using centered finite differences:
 *   - Convective term: -(1/2) * d(u²)/dx via flux form
 *   - Diffusive term: ν * ∂²u/∂x² via standard 3-point stencil
 *   - Periodicity: indices wrap (i+1 mod N, i-1 mod N)
 *
 * This discretization produces a system of N ODEs:
 *   du/dt = f(u, t)                                         [3]
 * which we call the Full-Order Model (FOM).
 *
 * REDUCED-ORDER MODELING WITH PRESSIO
 * ====================================
 *
 * Motivation: The FOM [3] has N equations (thousands/millions
 * in practical 2D/3D). We want to approximate u(t) ≈ ũ(t)
 * using M << N basis vectors, reducing computational cost by
 * orders of magnitude.
 *
 * POD Basis Construction:
 *   1. Simulate the FOM for representative parameters/ICs,
 *      collecting M_snapshots state vectors: {u^(1), u^(2), ..., u^(p)}
 *   2. Compute SVD: U ≈ U_r Σ_r V_r^T
 *      Columns of U_r are the r dominant POD modes (r << N)
 *   3. Basis truncation: keep modes explaining 99.999% of energy
 *
 * Reduced State Representation:
 *   ũ(t) = U_r * a(t) + u_mean                              [4]
 *
 * where a(t) ∈ ℝ^r are reduced coordinates (r coefficients)
 * and u_mean is the snapshot mean.
 *
 * Galerkin Projection (default ROM approach):
 *   Project equation [1] onto the r-dimensional subspace:
 *
 *   da/dt = U_r^T * f(U_r * a + u_mean, t)                  [5]
 *
 * This reduces N equations to r equations. Since r << N,
 * the ROM is dramatically cheaper than the FOM for
 * evaluation and sensitivity analysis.
 *
 * ACCURACY & LIMITATIONS
 * ======================
 *
 * The ROM accuracy depends on:
 *   - Basis quality: POD basis should capture dominant dynamics
 *   - Extrapolation risk: ROM is trained on training parameter
 *     ranges; testing outside these ranges may be inaccurate
 *   - Nonlinear effects: Strong nonlinearity (Burgers convection)
 *     can cause Galerkin ROM errors to accumulate in time
 *   - Viscosity regime: Low-ν (high Reynolds number) regimes
 *     with shock-like structures are harder to capture with
 *     a fixed POD basis (manifold non-linearity)
 *
 * WORKFLOW IN THIS CODE
 * =====================
 *
 * This tutorial demonstrates:
 *   1. Instantiate the FOM (class Burgers1dFom)
 *   2. Run FOM simulations for training parameter values
 *   3. Collect snapshots and compute POD basis
 *   4. Construct Galerkin ROM [5] with Pressio
 *   5. (Optional) Implement hyper-reduction for efficiency
 *   6. Run ROM on test parameters and compare against FOM
 *
 * Expected Result: ROM should deliver ~100-1000× speedup
 * with minimal error (< 1%) on test cases within training range.
 */

////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Step 0: Pressio Setup
//////////////////////////////////////////////////////////////////////////////////

/**
 * Pressio uses macros to enable features like logging and TPLs.
 * Typically, these would be set during configuration, but we will
 * define them explicitly here for demonstration purposes.
 *
 * Importantly, these macros should be defined BEFORE including any Pressio
 * headers.
 */
#define PRESSIO_ENABLE_LOGGING
#define PRESSIO_ENABLE_TPL_EIGEN

/**
 * Due to the hierarchical structure of Pressio, including
 * pressio/rom.hpp will also pull in all necessary dependencies
 * (including Eigen, since we defined the macro above).
 * The logging macros (from pressio-log) are also included here.
 */
#include <pressio/rom.hpp>

/**
 * We'll also need some helper functions along the way.
 */
#include "helpers.h"

/**
 * And some functions for hyper-reduction.
 */
#include "hyperreduction.h"

/**
 * And we'll use std::vectors to store snapshots.
 */
#include <vector>

/**
 * Pressio supports various linear algebra backends (such as
 * Eigen, Tpetra, and Kokkos) and provides a unified interface
 * with the pressio::ops library for common operations. In this
 * tutorial, we will use Eigen.
 */
using vector_t = Eigen::VectorXd;
using matrix_t = Eigen::MatrixXd;

///////////////////////////////////////////////////////////////////////////////
// Step 1: Define the Full-Order Model (FOM)
///////////////////////////////////////////////////////////////////////////////

/**
 * Pressio requires a specific API to be used for the FOM class. This API can
 * vary depending on the type of problem. For the 1D Burgers' equation,
 * we will use the API for a semi-discrete FOM for time-dependent problems of
 * the form:
 *
 *     d/dt y(t) = f(y, t).
 *
 * The various FOM APIs can be found in pressio/rom/concepts.hpp.
 *
 * According to the semi-discrete API, our FOM class must define the following:
 *
 *     1. Three core types
 *         - time_type, state_type, rhs_type
 *
 *     2. A method to create the RHS vector
 *         - createRhs() -> rhs_type
 *
 *     3. A method to compute the RHS
 *         - rhs(const state_type & u, time_type t, rhs_type & f) -> void
 *
 * And that's it!
 */
class Burgers1dFom
{
    public:
        // API Requirement: Core types
        using time_type  = double;
        using state_type = vector_t;
        using rhs_type   = vector_t;

    private:
        std::size_t N_{ };
        double dx_{ };
        double nu_{ };

    public:
        Burgers1dFom( std::size_t N, double domainLength, double viscosity )
        : N_( N ), dx_( domainLength / ( N ) ), nu_( viscosity )
        {
            assert( N_  >= 3 );
            assert( dx_ > 0.0 );
            assert( nu_ > 0.0 );
        }

        std::size_t N() const { return N_; }
        double dx()     const { return dx_; }
        double nu()     const { return nu_; }

        // API Requirement: createRhs() method
        rhs_type createRhs() const
        {
            return rhs_type::Zero( N_ );
        }

        // API Requirement: rhs(...) method
        void rhs(
            const state_type & u,
            time_type /*t*/,
            rhs_type & f
        ) const
        {
            assert( u.size() == N_ );
            assert( f.size() == N_ );

            // periodic indexing helpers
            auto ip = [ this ]( std::size_t i ){ return ( i + 1 ) % N_; };
            auto im = [ this ]( std::size_t i ){ return ( i + N_ - 1 ) % N_; };

            const double inv2dx = 1.0 / ( 2.0 * dx_ );
            const double invdx2 = 1.0 / ( dx_ * dx_ );

            for ( std::size_t i = 0; i < N_; ++i ) {
                const std::size_t iL = im( i );
                const std::size_t iR = ip( i );

                // convective term: -(1/2 d/dx (u^2)) using centered flux difference
                // d/dx (u^2) ~ (u_{i+1}^2 - u_{i-1}^2) / (2 dx)
                const double dudt_conv =
                    -0.5 * ( ( u[ iR ] * u[ iR ] ) - ( u[ iL ] * u[ iL ] ) ) * inv2dx;

                // viscous term: nu * u_xx using second-order centered FD
                const double dudt_diff =
                    nu_ * ( u[ iR ] - 2.0 * u[ i ] + u[ iL ] ) * invdx2;
                f[ i ] = dudt_conv + dudt_diff;
            }
        }
};

///////////////////////////////////////////////////////////////////////////////
// Step 2: Run the FOM to generate snapshots (states and RHS)
////////////////////////////////////////////////////////////////////////////////

struct SnapshotSet
{
    matrix_t stateSnapshots;
    matrix_t rhsSnapshots;
};

/**
 * Building and running the FOM doesn't require any Pressio utilities.
 * For this example, we'll define a simple Forward Euler time integrator
 * to advance the FOM in time and collect the snapshots that will be used
 * to build the ROM with Pressio.
 */
template <typename FomSystem>
SnapshotSet runFOM( FomSystem& fom, typename FomSystem::time_type startTime, int numSteps, double dt )
{
    // We will store each snapshot vector in a std::vector for now
    // For a typical ROM, we would only need the state snapshots. However,
    // for hyper-reduction, we will also need snapshots of the RHS.
    std::vector< vector_t > stateSnaps;
    std::vector< vector_t > rhsSnaps;

    // Compute the initial condition
    auto u = computeInitialCondition< FomSystem, vector_t >( fom );

    // Time integration loop (using Forward Euler)
    double t = startTime;
    for ( int step = 0; step < numSteps; ++step ) {
        stateSnaps.push_back( u );   // Store state snapshot
        auto f = fom.createRhs();    // Create RHS vector
        fom.rhs( u, t, f );          // Compute RHS
        rhsSnaps.push_back( f );     // Store RHS snapshot
        u += dt * f;                 // Update solution
        t += dt;                     // Advance time
    }

    // Convert our vector of snapshot vectors into matrices
    const std::size_t numSnapshots = stateSnaps.size();
    matrix_t stateMatrix( fom.N(), numSnapshots );
    matrix_t rhsMatrix( fom.N(), numSnapshots );
    for ( std::size_t i = 0; i < numSnapshots; ++i ) {
        stateMatrix.col( i ) = stateSnaps[ i ];
        rhsMatrix.col( i )      = rhsSnaps[ i ];
    }

    // Return both snapshot matrices
    return SnapshotSet{ std::move(stateMatrix), std::move(rhsMatrix) };
}

///////////////////////////////////////////////////////////////////////////////
// Step 3: Build the ROM from the snapshot matrix
///////////////////////////////////////////////////////////////////////////////

/**
 * Now we use the Pressio ecosystem to construct a ROM representation
 * with the snapshot matrix. As before, there are various APIs that we can meet
 * to use different types of ROMs. We'll use a basic Galerkin ROM here.
 */

template <typename FomSystem>
auto buildStandardGalerkinROM( FomSystem& fom, auto& stepScheme, auto& trialSpace )
{
    return pressio::rom::galerkin::create_unsteady_explicit_problem(
        stepScheme, trialSpace, fom
    );
}

/**
 * Hyper-reduced ROMs do not evaluate the full RHS at every time step.
 * Instead, they sample the RHS at selected indices and use a
 * projection to approximate the reduced RHS. This can greatly
 * reduce the computational cost of evaluating the ROM, especially
 * when the FOM is large and the RHS evaluation is expensive.
 *
 * However, there are trade-offs in accuracy, as you'll see.
 */
template <typename FomSystem>
auto buildHyperReducedGalerkinROM( FomSystem& fom, auto& stepScheme,
                                   auto& trialSpace, auto& hyperReducer )
{
    // Build the hyperreduced Galerkin ROM by passing the hyper-reducer
    // functor to the ROM factory function.
    return pressio::rom::galerkin::create_unsteady_explicit_problem(
        stepScheme, trialSpace, fom, hyperReducer
    );
}

///////////////////////////////////////////////////////////////////////////////
// Step 4: Run the ROM with trajectory capture
///////////////////////////////////////////////////////////////////////////////
/**
 * To run the ROM, we will define a simple observer that captures
 * the reduced state at each time step and reconstructs the full-order
 * state using the trial space. This allows us to capture the full
 * trajectory of the ROM solution for later analysis.
 */
template <typename FomSystem>
auto runROM( auto& rom, auto& trialSpace, auto& reducedState,
             typename FomSystem::time_type startTime, int numSteps, double dt,
             std::vector<vector_t>& trajectory
) {
    // Define the ODE time stepping policy
    auto policy = pressio::ode::steps_fixed_dt(
        startTime,
        pressio::ode::StepCount( numSteps ),
        dt
    );

    /**
     * In Pressio, observers are functors that are called at each time step
     * during time integration. Here, we define an observer that captures
     * the full-order state at each time step by reconstructing it from
     * the reduced state using the trial space.
     *
     * Observers must meet a specific API defined by Pressio. Namely, they
     * must implement the operator() with the signature shown in helpers.h.
     */
    auto observer = RomObserver< vector_t, decltype(trialSpace) >( trajectory, trialSpace );

    // Run the ROM time integration with observer
    pressio::ode::advance( rom, reducedState, policy, observer );

    /**
     * At this point, reducedState contains the ROM solution at the
     * final timestep. We just have to reconstruct the full-order
     * solution from the time reduced coordinates via the trial subspace.
     */
    auto romSolution = trialSpace.createFullStateFromReducedState( reducedState );

    return romSolution;
}

///////////////////////////////////////////////////////////////////////////////
// Step 5: Compare FOM and ROM solutions
///////////////////////////////////////////////////////////////////////////////

/**
 * To compare the FOM and ROM solutions, we can compute the relative error
 * between the two solution vectors at the final timestep.
 */
double compareFomAndRom( const matrix_t& fomSolution, const matrix_t& romSolution )
{
    double error = ( fomSolution - romSolution ).norm() / fomSolution.norm();
    return error;
}

///////////////////////////////////////////////////////////////////////////////
// Main driver
///////////////////////////////////////////////////////////////////////////////

int main() {
    // Initialize the logger so we can see Pressio output
    // Change the log level to "debug" for more information
    PRESSIOLOG_INITIALIZE(pressiolog::LogLevel::info);

    using FomSystem = Burgers1dFom;
    using time_t = typename FomSystem::time_type;

    // 1. Create the FOM
    const std::size_t N = 100;          // Number of spatial points
    const double domainLength = 1.0;    // Domain length
    const double viscosity = 0.01;      // Viscosity parameter
    FomSystem fom( N, domainLength, viscosity );
    PRESSIOLOG_INFO( "1. Created FOM with N = {}, dx = {}, nu = {}", fom.N(), fom.dx(), fom.nu() );

    // 2. Run the FOM to generate a snapshot matrix
    const double dt    = 0.001;
    const int numSteps = 1000;
    time_t startTime   = 0.0;
    SnapshotSet snapshots = runFOM< FomSystem >( fom, startTime, numSteps, dt );
    PRESSIOLOG_INFO( "2. Generated snapshot matrix with {} snapshots", snapshots.stateSnapshots.cols() );

    // 3 and 4. Build and run the ROM(s)

    /**
     * This tutorial covers both standard and hyper-reduced ROMs.
     * These variables below will hold the trajectories and solutions,
     * which we can analyze at the end.
     */
    std::vector<vector_t> standardRomTrajectory;
    std::vector<vector_t> hypredRomTrajectory;
    matrix_t standardSolution;
    matrix_t hypredSolution;

    // Start with the standard ROM
    PRESSIOLOG_INFO( "Standard ROM:" );
    {
        // 3. Build the ROM

        // Build the state trial space (Phi) from state snapshots
        auto standardTrialSpace = snapshots_to_trial_space< FomSystem, matrix_t, vector_t >( snapshots.stateSnapshots, fom );
        // Create the initial reduced state by projecting the FOM initial condition
        auto standardReducedState = trial_space_to_reduced_state< FomSystem, vector_t >( standardTrialSpace, fom );
        // Select the time integration scheme with Pressio
        auto stepScheme = pressio::ode::StepScheme::ForwardEuler;
        // Build the standard ROM
        auto standardRom = buildStandardGalerkinROM< FomSystem >( fom, stepScheme, standardTrialSpace );
        PRESSIOLOG_INFO( "  3. Built standard Galerkin ROM" );

        // 4. Run the ROM and capture the trajectory
        standardSolution = runROM< FomSystem >( standardRom, standardTrialSpace, standardReducedState, startTime, numSteps, dt, standardRomTrajectory );
        PRESSIOLOG_INFO( "  4. Ran standard ROM and captured trajectory" );
    }

    // Follow a similar process for hyper-reduced ROM
    PRESSIOLOG_INFO( "Hyper-reduced ROM:" );
    {
        // 3. Build the ROM
        auto hypredTrialSpace   = snapshots_to_trial_space< FomSystem, matrix_t, vector_t >( snapshots.stateSnapshots, fom );
        auto hypredReducedState = trial_space_to_reduced_state< FomSystem, vector_t >( hypredTrialSpace, fom );
        auto stepScheme = pressio::ode::StepScheme::ForwardEuler;
        /**
         * Here, we build the hyper-reducer using the RHS snapshot matrix
         * and the trial space. The hyper-reducer is a functor that will
         * be used by the ROM to compute the reduced RHS from sampled FOM RHSs.
         */
        auto hyperReducer = buildHyperReducer< vector_t, matrix_t >( snapshots.rhsSnapshots, hypredTrialSpace );
        auto hypredRom    = buildHyperReducedGalerkinROM< FomSystem >( fom, stepScheme, hypredTrialSpace, hyperReducer );
        PRESSIOLOG_INFO( "  3. Built hyper-reduced Galerkin ROM" );

        // 4. Run the ROM and capture the trajectory
        hypredSolution = runROM< FomSystem >( hypredRom, hypredTrialSpace, hypredReducedState, startTime, numSteps, dt, hypredRomTrajectory );
        PRESSIOLOG_INFO( "  4. Ran hyper-reduced ROM and captured trajectory" );
    }

    // 5. Compare ROM solution against FOM solution (the last snapshot)
    auto fomSolution = snapshots.stateSnapshots.col( snapshots.stateSnapshots.cols() - 1 );
    auto romError = compareFomAndRom( fomSolution, standardSolution );
    PRESSIOLOG_INFO( "5. Relative error between FOM and standard ROM: {}", romError );
    auto hypredError = compareFomAndRom( fomSolution, hypredSolution );
    PRESSIOLOG_INFO( "   Relative error between FOM and hyper-reduced ROM: {}", hypredError );

    // 6. Write trajectories to CSV files in output directory
    // Get FOM trajectory from snapshots
    std::vector<vector_t> fomTrajectory;
    for (int i = 0; i < snapshots.stateSnapshots.cols(); ++i) {
        fomTrajectory.push_back(snapshots.stateSnapshots.col(i));
    }
    writeTrajectoryToCSV< vector_t >("output", "fom_trajectory.csv",         fomTrajectory);
    writeTrajectoryToCSV< vector_t >("output", "standard_rom_trajectory.csv", standardRomTrajectory);
    writeTrajectoryToCSV< vector_t >("output", "hypred_rom_trajectory.csv",  hypredRomTrajectory);
    PRESSIOLOG_INFO( "6. Wrote trajectories to output/{fom,default_rom,hypred_rom}_trajectory.csv" );

    // Finalize the logger
    PRESSIOLOG_FINALIZE();
    return 0;
}

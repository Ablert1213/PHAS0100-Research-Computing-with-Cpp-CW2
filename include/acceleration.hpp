#include <Eigen/Dense>
#include <vector>

#include "particle.hpp"

using Eigen::Vector3d;

namespace n_body 
{
// The particleAcceleration class is derived from the Particle class.
// It contains additional functionality to calculate and store accelerations.
class particleAcceleration : public n_body::Particle { // inherit all function and definition from Particle class
    public:
        using Particle::Particle;

        // Add a particle to the particle list.
        void addParticle (n_body::particleAcceleration& add_particle);

        // Get the list of particles.
        std::vector <n_body::particleAcceleration*> getParticle ();

        // Calculate the acceleration of particle p_i due to particle p_j.
        // Epsilon is a softening factor to avoid numerical instability when particles are too close.
        static Vector3d calcAcceleration (const particleAcceleration* p_i, const particleAcceleration* p_j, const double& epsilon = 0.0);
        
        // Calculate the net acceleration of the current particle due to all other particles in the list.
        void sumAcceleration (std::vector<n_body::particleAcceleration*> particles_list, const double& epsilon = 0.0);
        
        // Release memory allocated to the particle list.
        void releaseMemory();
        
    protected:
        std::vector <n_body::particleAcceleration*> particle_list_;
};
}
#include <Eigen/Dense>
#include <vector>

#include "particle.hpp"

using Eigen::Vector3d;

namespace n_body 
{
class particleAcceleration : public n_body::Particle {
    public:
        using Particle::Particle;
        void addParticle (n_body::particleAcceleration add_particle);
        std::vector <n_body::particleAcceleration> getParticle ();
        static Vector3d calcAcceleration (const Particle &p_i, const Particle &p_j, double epsilon);
        void sumAcceleration (std::vector<n_body::particleAcceleration> particles_list, double epsilon);
    protected:
        std::vector <n_body::particleAcceleration> particle_list_;
        // Eigen::Vector3d acceleration;
};
}
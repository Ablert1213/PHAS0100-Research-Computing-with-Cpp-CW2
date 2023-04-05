#include <Eigen/Dense>
#include <vector>

using Eigen::Vector3d;

namespace n_body 
{

// The Particle class represents a single particle in an n-body system.
// It contains information about the particle's position, velocity, acceleration, and mass.
class Particle{
    public:
        // Constructs a particle with the given initial position, velocity, and mass.
        Particle(const Eigen::Vector3d& position, const Eigen::Vector3d& velocity, double& mass);
        
        // Accessor methods
        Eigen::Vector3d getPosition() const;
        Eigen::Vector3d getVelocity() const;
        Eigen::Vector3d getAcceleration() const;
        double getMass() const;

        // Sets the initial acceleration and can be used to update acceleration for the particle.
        void initialAcceleration (const Eigen::Vector3d& acceleration);
        // Updates the particle's position.
        void uploadPosition(const Eigen::Vector3d& position);
        // Updates the particle's velocity.
        void uploadVelocity(const Eigen::Vector3d& velocity);
        // Updates the particle's position and velocity based on the current acceleration and a time step (dt).
        void update(double& dt);

    protected:
        // Particle info variables
        Eigen::Vector3d position_;
        Eigen::Vector3d velocity_;
        Eigen::Vector3d acceleration_;
        double mass_;

};
}
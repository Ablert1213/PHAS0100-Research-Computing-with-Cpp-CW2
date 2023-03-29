#include <Eigen/Dense>
#include <vector>

using Eigen::Vector3d;

namespace n_body 
{

class Particle{
    public:
        Particle(const Eigen::Vector3d& position, const Eigen::Vector3d& velocity, double mass);
        
        Eigen::Vector3d getPosition() const;
        Eigen::Vector3d getVelocity() const;
        Eigen::Vector3d getAcceleration() const;
        double getMass() const;

        void initialAcceleration (const Eigen::Vector3d& acceleration);
        void uploadPosition(const Eigen::Vector3d& position);
        void uploadVelocity(const Eigen::Vector3d& velocity);
        void update(double dt);

    protected:
        Eigen::Vector3d position_;
        Eigen::Vector3d velocity_;
        Eigen::Vector3d acceleration_;

        double mass_;

};
}
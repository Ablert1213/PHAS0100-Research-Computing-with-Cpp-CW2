#include <Eigen/Dense>

// class Particle {
//     public:
//         Particle(double in_mass);
//         double getMass() const;

//     private:
//         double mass;
// };

namespace n_body 
{
class Particle{
    public:
        Particle(const Eigen::Vector3d& position, const Eigen::Vector3d& velocity, double mass);
        
        Eigen::Vector3d getPosition() const;
        Eigen::Vector3d getVelocity() const;
        Eigen::Vector3d getAcceleration() const;

        double getMass() const;
        void update(double dt, const Eigen::Vector3d& acceleration);

    private:
        Eigen::Vector3d position_;
        Eigen::Vector3d velocity_;
        Eigen::Vector3d acceleration_;

        double mass_;

};
}
#include "particle.hpp"
#include <Eigen/Core>

#include <iostream>

// Particle::Particle(double in_mass) :
//     mass{in_mass}
//     {}

// double Particle::getMass() const {
//     return mass;
// }

namespace n_body 
{
Particle::Particle(const Eigen::Vector3d& position, const Eigen::Vector3d& velocity, double mass)
    : position_ (position), velocity_ (velocity), mass_ (mass)
    {}

Eigen::Vector3d Particle::getPosition() const {
    return position_;
}

Eigen::Vector3d Particle::getVelocity() const {
    return velocity_;
}

Eigen::Vector3d Particle::getAcceleration() const {
    return acceleration_;
}

double Particle::getMass() const {
    return mass_;
}

void Particle::update(double dt, const Eigen::Vector3d& acceleration) {
    velocity_ += dt * acceleration_;
    position_ += dt * velocity_;
}

}
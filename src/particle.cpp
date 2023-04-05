#include "particle.hpp"
#include <Eigen/Core>
#include <iostream>

namespace n_body 
{
// Particle constructor implementation
Particle::Particle(const Eigen::Vector3d& position, const Eigen::Vector3d& velocity, double& mass)
    : position_ (position), velocity_ (velocity), mass_ (mass)
    {}
// Accessor methods implementation
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

// Set the initial acceleration or update for the particle
void Particle::initialAcceleration(const Eigen::Vector3d &acceleration) {
    acceleration_ = acceleration;
}
// Update the particle's position
void Particle::uploadPosition(const Eigen::Vector3d &position) {
    position_ = position;
}
// Update the particle's velocity
void Particle::uploadVelocity(const Eigen::Vector3d &velocity) {
    velocity_ = velocity;
}
// Update the particle's position and velocity based on the current acceleration and a time step (dt)
void Particle::update(double& dt) {
    position_ += dt * velocity_;
    velocity_ += dt * acceleration_;
}
}

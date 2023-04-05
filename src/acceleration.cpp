#include <Eigen/Dense>
#include <vector>
#include <iostream>
#include "acceleration.hpp"
#include <omp.h>

using Eigen::Vector3d;

namespace n_body 
{

// Add a particle to the particle list.
void particleAcceleration::addParticle (n_body::particleAcceleration& add_particle){
    particle_list_.push_back(&add_particle);
};

// Get the list of particles.
std::vector <n_body::particleAcceleration*> particleAcceleration::getParticle (){
    return particle_list_;
};

// Calculate the acceleration of particle p_i due to particle p_j.
Vector3d particleAcceleration::calcAcceleration (const particleAcceleration* p_i, const particleAcceleration* p_j,const double& epsilon){

    Vector3d acceleration_j_i;

    Vector3d r = p_j->getPosition() - p_i->getPosition();
    double d_j_i = (p_i->getPosition() - p_j->getPosition()).norm();
    double denominator = std::pow ((d_j_i * d_j_i + epsilon * epsilon), 3.0/2.0);

    acceleration_j_i = (p_j->getMass() * r) / denominator;

    return acceleration_j_i;
};

// Calculate the net acceleration of the current particle due to all other particles in the list.
void particleAcceleration::sumAcceleration (std::vector<n_body::particleAcceleration*> particles_list, const double& epsilon){

    Vector3d sumAcceleration_i = Vector3d::Zero();

    // developer can uncomment the codes blow to parallelise the calculation. 
    // #pragma omp declare reduction (+: Eigen::Vector3d: omp_out += omp_in) initializer(omp_priv=Eigen::Vector3d::Zero())
    // #pragma omp parallel for schedule(dynamic) reduction(+:sumAcceleration_i)
    
    for (particleAcceleration* p_i : particles_list) {
        if (p_i != this) {
            Vector3d acceleration_i = calcAcceleration(this, p_i, epsilon);
            sumAcceleration_i += acceleration_i;
        }
    }

    initialAcceleration(sumAcceleration_i);
};

// Release memory allocated to the particle list.
void particleAcceleration::releaseMemory() {
    particle_list_.shrink_to_fit();
}
}
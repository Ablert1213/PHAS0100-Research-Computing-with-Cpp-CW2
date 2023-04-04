#include <Eigen/Dense>
#include <vector>
#include <iostream>
#include "acceleration.hpp"
#include <omp.h>

using Eigen::Vector3d;

namespace n_body 
{

void particleAcceleration::addParticle (n_body::particleAcceleration& add_particle){
    particle_list_.push_back(&add_particle);
};

std::vector <n_body::particleAcceleration*> particleAcceleration::getParticle (){
    return particle_list_;
};

Vector3d particleAcceleration::calcAcceleration (const particleAcceleration* p_i, const particleAcceleration* p_j,const double& epsilon){

    Vector3d acceleration_j_i;

    Vector3d r = p_j->getPosition() - p_i->getPosition();
    double d_j_i = (p_i->getPosition() - p_j->getPosition()).norm();
    double denominator = std::pow ((d_j_i * d_j_i + epsilon * epsilon), 3.0/2.0);

    acceleration_j_i = (p_j->getMass() * r) / denominator;

    return acceleration_j_i;
};

void particleAcceleration::sumAcceleration (std::vector<n_body::particleAcceleration*> particles_list, const double& epsilon){

    Vector3d sumAcceleration_i = Vector3d::Zero();
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

void particleAcceleration::releaseMemory() {
    particle_list_.shrink_to_fit();
}
}
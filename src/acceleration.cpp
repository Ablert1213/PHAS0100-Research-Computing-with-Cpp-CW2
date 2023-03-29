#include <Eigen/Dense>
#include <vector>
#include "acceleration.hpp"

using Eigen::Vector3d;

namespace n_body 
{

void particleAcceleration::addParticle (n_body::particleAcceleration add_particle){
    particle_list_.push_back(add_particle);
};

std::vector <n_body::particleAcceleration> particleAcceleration::getParticle (){
    return particle_list_;
};

Vector3d particleAcceleration::calcAcceleration (const Particle& p_i, const Particle& p_j, double epsilon){
    
    Vector3d r = p_j.getPosition() - p_i.getPosition();
    double d_j_i = r.norm();
    double denominator = std::pow ((d_j_i * d_j_i + epsilon * epsilon), 3.0/2.0);
    Vector3d acceleration_j_i = (p_j.getMass () * r) / denominator;

    return acceleration_j_i;
};

void particleAcceleration::sumAcceleration (std::vector<n_body::particleAcceleration> particles_list, double epsilon){
    for (Particle &p_i : particles_list) {
        Vector3d sumAcceleration_i = Vector3d::Zero();
        for (const Particle &p_j : particles_list) {
            if (&p_i != &p_j) {
                sumAcceleration_i += calcAcceleration(p_i, p_j, epsilon);
            }
        }
        p_i.initialAcceleration(sumAcceleration_i);
    }
};


}
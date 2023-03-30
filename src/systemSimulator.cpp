#include <Eigen/Dense>
#include <vector>
#include <iostream>
#include <random>
#include <cmath>


#include "systemSimulator.hpp"

using Eigen::Vector3d;

namespace n_body
{
sysSimulator::sysSimulator (){
    std::vector<particleAcceleration> particle_list = particleListGenerator();
    addSysInput (particle_list);
}

std::vector<particleAcceleration> sysSimulator::particleListGenerator (){
    // inital data
    std::vector<double> masses = {1., 1./6023600, 1./408524, 1./332946.038, 1./3098710, 1./1047.55, 1./3499, 1./22962, 1./19352};
    std::vector<double> distances = {0.0, 0.4, 0.7, 1, 1.5, 5.2, 9.5, 19.2, 30.1};
    // generates random angle sigma for each of the planets 
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> randomly_theta (0, 2 * M_PI);
    
    std::vector<particleAcceleration> solar_system;

    for (int i = 0; i < masses.size(); ++i) {
        // initial the first sun input
        if (i == 0) {
            Vector3d position = Vector3d::Zero();
            Vector3d velocity = Vector3d::Zero();
            particleAcceleration planet_i = particleAcceleration (position, velocity, 1.0);
            solar_system.push_back(planet_i);
        }
        else{
            double theta_i = randomly_theta(gen);
            double mass_i = masses[i];
            double r_i = distances[i];

            Vector3d position ((r_i * sin(theta_i)), (r_i * cos(theta_i)), 0.0);
            Vector3d velocity ((-(1 / std::sqrt(r_i)) * cos(theta_i)), ((1 / std::sqrt(r_i)) * sin(theta_i)), 0.0);
            particleAcceleration planet_i = particleAcceleration (position, velocity, mass_i);
            solar_system.push_back(planet_i);

        }
    }
    return solar_system;

}

void sysSimulator::addSysInput (std::vector<particleAcceleration>& particle_list) {
    // for (int i = 0; i < particle_list.size(); i++){
    //     for (int j = 0; j < particle_list.size(); j++){
    //         if (i != j){
    //             particle_list[i].addParticle(particle_list[j]);
    //         }
    //         else{
    //             continue;
    //         }
    //     }
    // }
    for (particleAcceleration &p_i : particle_list) {
        for (const particleAcceleration &p_j : particle_list) {
            if (&p_i != &p_j) {
                p_i.addParticle(p_j);
            }
            else{
                continue;
            }
        }
    }
}

void sysSimulator::printPosition (std::vector<particleAcceleration>& particle_list, const std::string& label){
    std::cout << label << " positions:" << std::endl;
    std::vector<std::string> planet {"sun", "Mercury", "Venus", "Earth", "Mars", "Jupiter", "Saturn", "Uranus", "Neptune"};
    int i = 0;
    for (const n_body::particleAcceleration& particle : particle_list) {
        std::cout << planet[i] << ": "<< particle.getPosition().transpose() << std::endl;
        ++i;
    }
}
}
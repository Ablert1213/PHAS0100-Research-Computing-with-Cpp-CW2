#include <Eigen/Dense>
#include <vector>
#include <iostream>
#include <random>
#include <algorithm>
#include <memory>
#include <string>
#include <omp.h>


#include "systemSimulator.hpp"

using Eigen::Vector3d;

namespace n_body
{

std::vector<particleAcceleration> SolarSystemGenerator::generateInitialConditions() {
    // inital data
    std::vector<double> masses = {1., 1./6023600, 1./408524, 1./332946.038, 1./3098710, 1./1047.55, 1./3499, 1./22962, 1./19352};
    std::vector<double> distances = {0.0, 0.4, 0.7, 1, 1.5, 5.2, 9.5, 19.2, 30.1};
    // generates random angle sigma for each of the planets 
    // std::random_device rd;
    int seed = 42; // have a fixed seed value that allows to reproduce results consistently
    std::mt19937 gen(seed);
    std::uniform_real_distribution<double> randomly_theta (0, 2 * M_PI);
    
    std::vector<particleAcceleration> solar_system;

    for (int i = 0; i < masses.size(); ++i) {
        // initial the first sun input
        if (i == 0) {
            Vector3d position = Vector3d::Zero();
            Vector3d velocity = Vector3d::Zero();
            double mass_sun = 1.0;
            particleAcceleration planet_i = particleAcceleration (position, velocity, mass_sun);
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

RandomSystemGenerator::RandomSystemGenerator(int seed, int num_particles): seed(seed), num_particles(num_particles) {}

std::vector<particleAcceleration> RandomSystemGenerator::generateInitialConditions() {
    // Implement random initial conditions generation
    std::mt19937 gen(seed);
    std::uniform_real_distribution<double> mass_distribution(1.0 / 6000000, 1.0 / 1000);
    std::uniform_real_distribution<double> distance_distribution(0.4, 30);
    std::uniform_real_distribution<double> angle_distribution(0, 2 * M_PI);

    std::vector<particleAcceleration> particles;

    // Create the central star
    double mass_central = 1.0;
    particleAcceleration central_star(Vector3d::Zero(), Vector3d::Zero(), mass_central);
    particles.push_back(central_star);

    // Generate random particles
    for (int i = 1; i <= num_particles; ++i) {
        double mass_i = mass_distribution(gen);
        double r_i = distance_distribution(gen);
        double theta_i = angle_distribution(gen);

        Vector3d position ((r_i * sin(theta_i)), (r_i * cos(theta_i)), 0.0);
        Vector3d velocity ((-(1 / std::sqrt(r_i)) * cos(theta_i)), ((1 / std::sqrt(r_i)) * sin(theta_i)), 0.0);
        particleAcceleration particle(position, velocity, mass_i);
        particles.push_back(particle);

    }

    return particles;
}

sysSimulator::sysSimulator (std::shared_ptr<InitialConditionGenerator> gen) : generator(gen){
    std::vector<particleAcceleration> particle_list = generator->generateInitialConditions();
    addSysInput(particle_list);
    particle_list_ = particle_list;
}

std::vector<particleAcceleration> sysSimulator::particleListGenerator (){
    
    // return generator->generateInitialConditions();
    return particle_list_;
}

void sysSimulator::addSysInput (std::vector<particleAcceleration>& particle_list) {

    #pragma omp parallel for schedule(dynamic) collapse(1)
    for (particleAcceleration& p_i : particle_list) {
        for (particleAcceleration& p_j : particle_list) {
            if (&p_i != &p_j) {
                p_i.addParticle(p_j);
            }
        }
    }
    releaseMemoryFromParticles(particle_list);

}

void sysSimulator::releaseMemoryFromParticles(std::vector<particleAcceleration>& particle_list) {
    for (particleAcceleration& p_i : particle_list) {
        p_i.releaseMemory();
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
std::vector<double> sysSimulator::kineticEnergy (std::vector<particleAcceleration>& particle_list) {
    std::vector<double> kinetic_energy_list;
    for (const n_body::particleAcceleration particle : particle_list){
        double kin_energy = 0.0;
        double mass = particle.getMass();
        Eigen::Vector3d velocity = particle.getVelocity();
        kin_energy = 0.5 * mass * velocity.squaredNorm();
        kinetic_energy_list.push_back(kin_energy);
    }
    kinetic_energy_list_ = kinetic_energy_list;
    return kinetic_energy_list_;
}

std::vector<double> sysSimulator::potentialEnergy (std::vector<particleAcceleration>& particle_list) {
    std::vector<double> potential_energy_list;
    for (particleAcceleration &p_i : particle_list) {
        // initial potential energy for each particle
        double pot_energy = 0.0;
        for (const particleAcceleration &p_j : particle_list) {
            if (&p_i != &p_j) {
                double mass_i = p_i.getMass();
                double mass_j = p_j.getMass();
                Eigen::Vector3d position_i = p_i.getPosition();
                Eigen::Vector3d position_j = p_j.getPosition();
                double dis_i_j = (position_i - position_j).norm();
                pot_energy += -0.5 * (mass_i * mass_j) / dis_i_j;
            }
            else{
                continue;
            }
        }
        potential_energy_list.push_back(pot_energy);
        
    }
    potential_energy_list_ = potential_energy_list;
    return potential_energy_list_;
}

std::vector<double> sysSimulator::potentialEnergyPara(std::vector<particleAcceleration>& particle_list) {
    std::vector<double> potential_energy_list(particle_list.size(), 0.0);

    #pragma omp parallel
    {
        std::vector<double> local_potential_energy_list(particle_list.size(), 0.0);

        #pragma omp for
        for (int i = 0; i < particle_list.size(); ++i) {
            for (int j = 0; j < particle_list.size(); ++j) {
                if (i != j) {
                    double mass_i = particle_list[i].getMass();
                    double mass_j = particle_list[j].getMass();
                    Eigen::Vector3d position_i = particle_list[i].getPosition();
                    Eigen::Vector3d position_j = particle_list[j].getPosition();
                    double dis_i_j = (position_i - position_j).norm();
                    local_potential_energy_list[i] += -0.5 * (mass_i * mass_j) / dis_i_j;
                }
            }
        }

        #pragma omp critical
        for (int i = 0; i < particle_list.size(); ++i) {
            potential_energy_list[i] += local_potential_energy_list[i];
        }
    }

    potential_energy_list_ = potential_energy_list;
    return potential_energy_list_;
}


std::vector<double> sysSimulator::totalEnergy (){
    std::vector<double> total_energy_list;
    for (int i = 0; i < kinetic_energy_list_.size(); ++i){
        double tot_energy = 0.0;
        double kin_energy = kinetic_energy_list_[i];
        double pot_energy = potential_energy_list_[i];
        tot_energy = kin_energy + pot_energy;
        total_energy_list.push_back(tot_energy);
    }
    total_energy_list_ = total_energy_list;
    return total_energy_list_;
}

double sysSimulator::sumTotalEnergy (){
    double sum_tot_energy = 0.0;
    for (int i = 0; i < total_energy_list_.size(); ++i){
        sum_tot_energy += total_energy_list_[i];
    } 
    sum_tot_energy_ = sum_tot_energy;
    return sum_tot_energy_;
}

double sysSimulator::sumTotalEnergyPara (){
    double sum_tot_energy = 0.0;
    #pragma omp parallel for reduction(+:sum_tot_energy)
    for (int i = 0; i < total_energy_list_.size(); ++i){
        sum_tot_energy += total_energy_list_[i];
    } 
    sum_tot_energy_ = sum_tot_energy;
    return sum_tot_energy_;
}
}

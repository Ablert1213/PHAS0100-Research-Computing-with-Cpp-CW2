#include <Eigen/Dense>
#include <vector>
#include <iostream>
#include <random>
#include <algorithm>
#include <memory>
#include <string>
#include "acceleration.hpp"
#pragma once

using Eigen::Vector3d;

namespace n_body 
{

class InitialConditionGenerator {
public:
    virtual std::vector<particleAcceleration> generateInitialConditions() = 0;
};

class SolarSystemGenerator : public InitialConditionGenerator {
public:
    std::vector<particleAcceleration> generateInitialConditions() override;
};

class RandomSystemGenerator : public InitialConditionGenerator {
private:
    int seed;
public:
    RandomSystemGenerator(int seed = 42) : seed(seed) {} // Default seed is 42, but it can be fixed by developer to a particular value when appropriate
    std::vector<particleAcceleration> generateInitialConditions() override;
};

class sysSimulator{
    private:
        std::shared_ptr<InitialConditionGenerator> generator;
        // double epsilon;
    public:
        // construct inputs 
        // sysSimulator ();
        sysSimulator (std::shared_ptr<InitialConditionGenerator> gen);
        
        // create a list of particles corresponding to the major bodies within solar system with given initial conditions
        std::vector<particleAcceleration> particleListGenerator ();

        // add input data to the particle vector to calculate
        void addSysInput (std::vector<particleAcceleration>& particle_list);

        // calculate the kinetic energy
        std::vector<double> kineticEnergy (std::vector<particleAcceleration> particle_list);

        // calculate the potential energy
        std::vector<double> potentialEnergy (std::vector<particleAcceleration> particle_list);

        // calculate the total energy of each particle
        std::vector<double> totalEnergy ();

        // calculate the sum of all individual particle energies
        double sumTotalEnergy ();

        // print position value of particles in the list
        static void printPosition (std::vector<particleAcceleration>& particle_list, const std::string& label);

    protected:
        std::vector<double> kinetic_energy_list_;
        std::vector<double> potential_energy_list_;
        std::vector<double> total_energy_list_;
        double sum_tot_energy_;
};



}
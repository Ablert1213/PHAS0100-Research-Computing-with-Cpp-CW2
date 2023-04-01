#include <Eigen/Dense>
#include <vector>
#include <iostream>
#include "acceleration.hpp"
#pragma once

using Eigen::Vector3d;

namespace n_body 
{
class sysSimulator{
    public:
        // construct inputs 
        sysSimulator ();

        // create a list of particles corresponding to the major bodies within solar system with given initial conditions
        static std::vector<particleAcceleration> particleListGenerator ();

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

class InitialConditionGenerator {
public:
    virtual std::vector<particleAcceleration> generateInitialConditions() = 0;
};



}
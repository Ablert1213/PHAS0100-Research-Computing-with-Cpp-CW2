#include <Eigen/Dense>
#include <vector>
#include <iostream>
#include <random>
#include <algorithm>
#include <memory>
#include <string>
#include "acceleration.hpp"

using Eigen::Vector3d;

namespace n_body 
{

// Abstract base class for generating initial conditions
class InitialConditionGenerator {
public:
    virtual std::vector<particleAcceleration> generateInitialConditions() = 0;
};

// Solar system initial condition generator
class SolarSystemGenerator : public InitialConditionGenerator {
public:
    std::vector<particleAcceleration> generateInitialConditions() override;
};

// Random system initial condition generator
class RandomSystemGenerator : public InitialConditionGenerator {
private:
    int seed;
    int num_particles;
public:
    // Constructor with default seed and number of particles
    RandomSystemGenerator(int seed = 42, int num_particles = 8); // Default seed is 42, number of particles are 8, but it can be fixed by developer to a particular value when appropriate
    std::vector<particleAcceleration> generateInitialConditions() override;
};

// System simulator class
class sysSimulator{
    private:
        std::shared_ptr<InitialConditionGenerator> generator;

    public:

        // Constructor taking an initial condition generator
        sysSimulator (std::shared_ptr<InitialConditionGenerator> gen);
        
        // Generate particle list using initial condition generator
        std::vector<particleAcceleration> particleListGenerator ();

        // Add input data to particle list for calculations
        void addSysInput (std::vector<particleAcceleration>& particle_list);

        // Calculate kinetic energy for all particles
        std::vector<double> kineticEnergy (std::vector<particleAcceleration>& particle_list);

        // calculate the potential energy and parallelise the calculation using OpenMp
        std::vector<double> kineticEnergyPara (std::vector<particleAcceleration>& particle_list);

        // Calculate potential energy for all particles
        std::vector<double> potentialEnergy (std::vector<particleAcceleration>& particle_list);

        // Calculate potential energy in parallel using OpenMP
        std::vector<double> potentialEnergyPara(std::vector<particleAcceleration>& particle_list); 

        // Calculate total energy for all particles
        std::vector<double> totalEnergy ();

        // Calculate sum of all individual particle energies
        double sumTotalEnergy ();

        // Calculate sum of all individual particle energies in parallel using OpenMP
        double sumTotalEnergyPara ();    

        // Print particle positions
        static void printPosition (std::vector<particleAcceleration>& particle_list, const std::string& label);

        // Release memory after particles have been added to particleAcceleration objects
        void releaseMemoryFromParticles(std::vector<particleAcceleration>& particle_list);
    
    protected:
        std::vector<particleAcceleration> particle_list_;
        std::vector<double> kinetic_energy_list_;
        std::vector<double> potential_energy_list_;
        std::vector<double> total_energy_list_;
        double sum_tot_energy_;
};
}
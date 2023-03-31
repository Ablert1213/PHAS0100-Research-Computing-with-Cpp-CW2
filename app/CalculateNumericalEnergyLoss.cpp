#include <iostream>
#include <Eigen/Core>
#include "systemSimulator.hpp"

int main(int argc, char* argv[]) {
    if (argc == 1 || std::string(argv[1]) == "-h" || std::string(argv[1]) == "--help"){
        std::cout << "Usage: solarSystemSimulator [options]" << "\n";
        std::cout << "Options:" << "\n";
        std::cout << "  -h, --help      Display this help message" << "\n";
        std::cout << "  -dt <value>     Set the timestep for the simulation" << "\n";
        std::cout << "  -len_time <years>  Set the total length of time to simulate" << "\n";
        std::cout << "  For example: solarSystemSimulator 0.01 100" << "\n";
        std::cout << "  This mean 100 years of 0.01 each timestep to simulate" << std::endl;
        return 0;
    }

    else{
        double dt = std::atof(argv[1]);
        double len_time = std::atof(argv[2]);
        double tot_timestpes = len_time * ((2 * M_PI)/dt);

        n_body::sysSimulator simulator = n_body::sysSimulator();
        std::vector<n_body::particleAcceleration> particle_list = simulator.particleListGenerator();
        std::vector<double> kinetic_energy_list = simulator.kineticEnergy(particle_list);
        std::vector<double> potential_energy_list = simulator.potentialEnergy(particle_list);
        std::vector<double> total_energy_list = simulator.totalEnergy();
        double sum_total_energy = simulator.sumTotalEnergy();

        std::cout << "Inital Energy: " << std::endl;
        std::vector<std::string> planet {"sun", "Mercury", "Venus", "Earth", "Mars", "Jupiter", "Saturn", "Uranus", "Neptune"};
        for (int i = 0; i < total_energy_list.size(); ++i) {
          std::cout << planet[i] << ": " << "kinetic energy: "<< kinetic_energy_list[i] << " potential energy: "<< potential_energy_list[i] << " total energy: "<< total_energy_list[i] << std::endl;
        }

        std::cout << "sum of total energy: " << sum_total_energy << std::endl;

        for (int timestep = 0; timestep < tot_timestpes; ++timestep){
            // Update gravitational acceleration for all bodies
            for (n_body::particleAcceleration& p_i : particle_list){
                p_i.sumAcceleration(particle_list);
            } 

            // Update position and velocity of each body
            for (n_body::particleAcceleration& p_i : particle_list){
                p_i.update(dt);
            }
        }

        std::vector<double> kinetic_energy_list_final = simulator.kineticEnergy(particle_list);
        std::vector<double> potential_energy_list_final = simulator.potentialEnergy(particle_list);
        std::vector<double> total_energy_list_final = simulator.totalEnergy();
        double sum_total_energy_final = simulator.sumTotalEnergy();

        std::cout << std::endl;
        std::cout << "Final Energy: " << std::endl;
        // std::vector<std::string> planet {"sun", "Mercury", "Venus", "Earth", "Mars", "Jupiter", "Saturn", "Uranus", "Neptune"};
        for (int i = 0; i < total_energy_list.size(); ++i) {
          std::cout << planet[i] << ": " << "kinetic energy: "<< kinetic_energy_list_final[i] - kinetic_energy_list[i] << " potential energy: "<< potential_energy_list_final[i] << " total energy: "<< total_energy_list_final[i] << std::endl;
        }

        std::cout << "sum of total energy: " << sum_total_energy_final << " total energy drop: " << 100 * (sum_total_energy_final - sum_total_energy)/sum_total_energy << "%" << std::endl;

    }

}
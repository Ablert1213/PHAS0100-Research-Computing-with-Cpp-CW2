#include <iostream>
#include <Eigen/Core>
#include <chrono>
#include "systemSimulator.hpp"

int main(int argc, char* argv[]) {
    if (argc == 1 || std::string(argv[1]) == "-h" || std::string(argv[1]) == "--help"){
        std::cout << "Usage: solarSystemSimulator [options]" << "\n";
        std::cout << "Options:" << "\n";
        std::cout << "  -h, --help      Display this help message" << "\n";
        std::cout << "  -dt <float><value>     Set the timestep for the simulation" << "\n";
        std::cout << "  -len_time <float><years>  Set the total length of time to simulate" << "\n";
        std::cout << "  -epsilon <float><softening factor>     Set the epsilon for the simulation" << "\n";
        std::cout << "  -num_particles <integer><number of inital particles>     Set the number of inital particles for the simulation" << "\n";
        std::cout << "  For example: solarSystemSimulator 0.01 100 0.001" << "\n";
        std::cout << "  This mean 100 years of 0.01 each timestep to simulate at epsilon equal to 0.001" << std::endl;
        return 0;
    }

    else{
        double dt = std::atof(argv[1]);
        double len_time = std::atof(argv[2]);
        double tot_timestpes = len_time * ((2 * M_PI)/dt);
        double epsilon = std::atof(argv[3]);
        int seed = 42; // developer can modify seed value here, set as default number 42
        int num_particles = epsilon = std::stoi(argv[4]);

        n_body::sysSimulator simulator = n_body::sysSimulator(std::make_shared<n_body::RandomSystemGenerator>(seed, num_particles));
        // n_body::sysSimulator simulator = n_body::sysSimulator();
        std::vector<n_body::particleAcceleration> particle_list = simulator.particleListGenerator();
        std::vector<double> kinetic_energy_list = simulator.kineticEnergy(particle_list);
        std::vector<double> potential_energy_list = simulator.potentialEnergy(particle_list);
        std::vector<double> total_energy_list = simulator.totalEnergy();
        double sum_total_energy = simulator.sumTotalEnergy();

        std::cout << "Inital Energy: " << std::endl;
        std::vector<std::string> planet {"sun", "Mercury", "Venus", "Earth", "Mars", "Jupiter", "Saturn", "Uranus", "Neptune"};
        // for (int i = 0; i < total_energy_list.size(); ++i) {
        //   std::cout << planet[i] << ": " << "kinetic energy: "<< kinetic_energy_list[i] << " potential energy: "<< potential_energy_list[i] << " total energy: "<< total_energy_list[i] << std::endl;
        // }
        std::cout << "sum of total energy: " << sum_total_energy << std::endl;

        // Start the timer
        auto start_time = std::chrono::high_resolution_clock::now();

        for (int timestep = 0; timestep < tot_timestpes; ++timestep){
            // Update gravitational acceleration for all bodies
            for (n_body::particleAcceleration& p_i : particle_list){
                p_i.sumAcceleration(particle_list, epsilon);
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

        // End the timer
        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_time = end_time - start_time;

        // Calculate and print the total time and average time per timestep
        double total_time = elapsed_time.count();
        double avg_time_per_timestep = total_time / tot_timestpes;
        std::cout << "\n" <<"Total time: " << total_time/60 << " mins" << std::endl;
        std::cout << "Average time per timestep: " << avg_time_per_timestep << " seconds" << std::endl;

        std::cout << std::endl;
        std::cout << "Final Energy: " << std::endl;
        // std::vector<std::string> planet {"sun", "Mercury", "Venus", "Earth", "Mars", "Jupiter", "Saturn", "Uranus", "Neptune"};
        // for (int i = 0; i < total_energy_list.size(); ++i) {
        //   std::cout << planet[i] << ": " << "kinetic energy: "<< kinetic_energy_list_final[i] - kinetic_energy_list[i] << " potential energy: "<< potential_energy_list_final[i] << " total energy: "<< total_energy_list_final[i] << std::endl;
        // }

        std::cout << "sum of total energy: " << sum_total_energy_final << " total energy drop: " << 100 * (sum_total_energy_final - sum_total_energy)/sum_total_energy << "%" << std::endl;

    }

}
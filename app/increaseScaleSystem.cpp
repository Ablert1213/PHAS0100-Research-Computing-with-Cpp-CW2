#include <iostream>
#include <Eigen/Core>
#include <chrono>
#include <vector>
#include <omp.h>
#include "systemSimulator.hpp"

// This program simulates an n-body solar system using parallel programming techniques.
// It accepts command line arguments for time step size, total simulation time, softening factor epsilon value, and the number of initial particles.
int main(int argc, char* argv[]) {

    // If no arguments are provided or the user requests help, display usage instructions.
    if (argc == 1 || std::string(argv[1]) == "-h" || std::string(argv[1]) == "--help"){
        std::cout << "Usage: solarSystemSimulator [options]" << "\n";
        std::cout << "Options:" << "\n";
        std::cout << "  -h, --help      Display this help message" << "\n";
        std::cout << "  -dt <float><value>     Set the timestep for the simulation" << "\n";
        std::cout << "  -len_time <float><years>  Set the total length of time to simulate" << "\n";
        std::cout << "  -epsilon <float><softening factor>     Set the epsilon for the simulation" << "\n";
        std::cout << "  -num_particles <integer><number of inital particles>     Set the number of inital particles for the simulation" << "\n";
        std::cout << "  For example_1: solarSystemSimulator 0.01 100 0.001" << "\n";
        std::cout << "  This mean 100 years of 0.01 each timestep to simulate at epsilon equal to 0.001" << "\n";
        std::cout << "  For example_2: solarSystemSimulator 0.01 100 0.001 2048" << "\n";
        std::cout << "  This mean 100 years of 0.01 each timestep to simulate 2048 particles at epsilon equal to 0.001" << std::endl;
        return 0;
    }

    else{

        // Create a list of default particle numbers for benchmarking.
        std::vector<int> num_particles_list = {8, 64, 256, 1024, 2048};
        double dt = std::atof(argv[1]);
        double len_time = std::atof(argv[2]);
        double tot_timestpes = len_time * ((2 * M_PI)/dt);
        double epsilon = std::atof(argv[3]);
        int seed = 42; // Developer can modify seed value here, set as default number 42

        // If the user provides the number of particles as an argument,
        // run the simulation with the specified number of particles.
        if (argc == 5) {
            
            // Start the timer
            auto start_time = std::chrono::high_resolution_clock::now();

            // Calculates energy values by using initial particle state
            int num_particles = std::stoi(argv[4]);
            n_body::sysSimulator simulator = n_body::sysSimulator(std::make_shared<n_body::RandomSystemGenerator>(seed, num_particles));
            std::vector<n_body::particleAcceleration> particle_list = simulator.particleListGenerator();
            std::vector<double> kinetic_energy_list = simulator.kineticEnergy(particle_list);
            std::vector<double> potential_energy_list = simulator.potentialEnergy(particle_list);
            std::vector<double> total_energy_list = simulator.totalEnergy();
            double sum_total_energy = simulator.sumTotalEnergy();
            
            // Create a vector of pointers to particleAcceleration objects
            std::vector<n_body::particleAcceleration*> particle_ptr_list;
            for (auto& p : particle_list) {
                particle_ptr_list.push_back(&p);
            }

            for (int timestep = 0; timestep < tot_timestpes; ++timestep){

                // developer can uncomment the codes blow to parallelise the loop. 
                // Update gravitational acceleration for all body
                #pragma omp parallel for
                for (n_body::particleAcceleration* p_i : particle_ptr_list){
                    p_i->sumAcceleration(particle_ptr_list);
                } 

                // developer can uncomment the codes blow to parallelise the loop. 
                // Update position and velocity of each body
                #pragma omp parallel for
                for (n_body::particleAcceleration* p_i : particle_ptr_list){
                    p_i->update(dt);
                }
            }

            // Calculates energy values by using updated particle state
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
            std::cout << "\n" << num_particles << " number of initial particles "<< "Inital Energy: "<<std::endl;
            std::cout <<"Total time: " << total_time/60 << " mins" << std::endl;
            std::cout << "Average time per timestep: " << avg_time_per_timestep << " seconds" << std::endl;
            std::cout << std::endl;
            std::cout << "Final Energy: " << std::endl;
            std::cout << "sum of total energy: " << sum_total_energy_final << " total energy drop: " << 100 * (sum_total_energy_final - sum_total_energy)/sum_total_energy << "%" << std::endl;
            
        }

        // If the user doesn't provide the number of particles as an argument,
        // run the simulation for a range of particle numbers to benchmark performance.
        else{
            for (int num_particles : num_particles_list){

                // Initialize the system simulator with the specified number of particles.
                n_body::sysSimulator simulator = n_body::sysSimulator(std::make_shared<n_body::RandomSystemGenerator>(seed, num_particles));
                
                // Start the timer
                auto start_time = std::chrono::high_resolution_clock::now();
                
                // Calculates energy values by using initial particle state
                std::vector<n_body::particleAcceleration> particle_list = simulator.particleListGenerator();
                std::vector<double> kinetic_energy_list = simulator.kineticEnergy(particle_list);
                std::vector<double> potential_energy_list = simulator.potentialEnergy(particle_list);
                std::vector<double> total_energy_list = simulator.totalEnergy();
                double sum_total_energy = simulator.sumTotalEnergy();

                std::vector<n_body::particleAcceleration*> particle_ptr_list;
                for (auto& p : particle_list) {
                    particle_ptr_list.push_back(&p);
                }

                for (int timestep = 0; timestep < tot_timestpes; ++timestep){
                    // Update gravitational acceleration for all bodies
                    for (n_body::particleAcceleration* p_i : particle_ptr_list){
                        p_i->sumAcceleration(particle_ptr_list);
                    } 

                    // Update position and velocity of each body
                    for (n_body::particleAcceleration* p_i : particle_ptr_list){
                        p_i->update(dt);
                    }
                }

                // Calculates energy values by using updated particle state
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
                std::cout << "\n" << num_particles << " number of initial particles "<< "Inital Energy: "<<std::endl;
                std::cout <<"Total time: " << total_time/60 << " mins" << std::endl;
                std::cout << "Average time per timestep: " << avg_time_per_timestep << " seconds" << std::endl;
                std::cout << std::endl;
                std::cout << "Final Energy: " << std::endl;
                std::cout << "sum of total energy: " << sum_total_energy_final << " total energy drop: " << 100 * (sum_total_energy_final - sum_total_energy)/sum_total_energy << "%" << std::endl;
            }
        }
    }
    return 0;

}

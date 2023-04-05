#include <iostream>
#include <Eigen/Core>
#include "systemSimulator.hpp"

// Main function for simulating the solar system
int main(int argc, char* argv[]) {

    // Display help message if no arguments are provided or help flag is used
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

        // Parse command line arguments for timestep and simulation length
        double dt = std::atof(argv[1]);
        double len_time = std::atof(argv[2]);
        double tot_timestpes = len_time * (2 * M_PI / dt);

        // Initialize the simulator with the solar system generator
        n_body::sysSimulator simulator = n_body::sysSimulator(std::make_shared<n_body::SolarSystemGenerator>());
        std::vector<n_body::particleAcceleration> particle_list = simulator.particleListGenerator();

        // Print initial positions
        simulator.printPosition (particle_list, "Initial");

        // Convert particle_list to particle_ptr_list for easier manipulation
        std::vector<n_body::particleAcceleration*> particle_ptr_list;
        for (auto& p : particle_list) {
            particle_ptr_list.push_back(&p);
        }

        // Main simulation loop
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

        // Print final positions
        std::cout << "\n" << std::endl;
        simulator.printPosition (particle_list, "Final");
    }
}

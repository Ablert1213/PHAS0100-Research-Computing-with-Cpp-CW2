#include <iostream>
#include <Eigen/Core>
#include "acceleration.hpp"

int main(int argc, char* argv[]) {
    if (argc == 1 || std::string(argv[1]) == "-h" || std::string(argv[1]) == "--help"){
        std::cout << "Usage: solarSystemSimulator [options]" << "\n";
        std::cout << "Options:" << "\n";
        std::cout << "  -h, --help      Display this help message" << "\n";
        std::cout << "  -dt <value>     Set the timestep for the simulation" << "\n";
        std::cout << "  -timesteps <n>  Set the total number of timesteps to simulate" << "\n";
        std::cout << "  For example: solarSystemSimulator 0.01 100" << "\n";
        std::cout << "  This mean 100 total number of 0.01 timestep to simulate" << std::endl;
        return 0;
    }

}
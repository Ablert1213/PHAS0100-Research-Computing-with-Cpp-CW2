#include <Eigen/Dense>
#include <vector>
#include <iostream>
#include "acceleration.hpp"

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

        // print position value of particles in the list
        static void printPosition (std::vector<particleAcceleration>& particle_list, const std::string& label);
};
}
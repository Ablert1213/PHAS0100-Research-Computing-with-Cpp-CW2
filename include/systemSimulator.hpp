#include <Eigen/Dense>
#include <vector>
#include <iostream>

using Eigen::Vector3d;

namespace n_body 
{
class sysSimulator{
    public:
        // construct inputs 
        sysSimulator initialSysInput ();

        // create a list of particles corresponding to the major bodies within solar system with given initial conditions
        std::vector<particleAcceleration> particle_list_generate ();

        // add input data to the particle vector to calculate
        void addSysInput ();
}
}
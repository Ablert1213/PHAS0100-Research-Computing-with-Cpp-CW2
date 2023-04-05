#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "systemSimulator.hpp"
#include <Eigen/Dense>
#include <vector>
#include <iostream>

using Catch::Matchers::WithinRel;
using Eigen::Vector3d;
using Catch::Approx;

TEST_CASE("Particle with no acceleration should move at constant velocity", "[particle]") {
    // Set initial conditions
    Vector3d position(1, 2, 3);
    Vector3d velocity(4, 5, 6);
    double mass = 10.0;

    // Create particle instance
    n_body::Particle particle(position, velocity, mass);

    // Apply zero acceleration
    Vector3d zero_acceleration(0, 0, 0);
    particle.initialAcceleration(zero_acceleration);    

    // Perform simulation step
    double dt = 0.1;
    particle.update(dt);

    // Check if the expected values match the calculated values
    REQUIRE(particle.getPosition().isApprox(Vector3d(1.4, 2.5, 3.6), 1e-6));
    REQUIRE(particle.getVelocity().isApprox(Vector3d(4, 5, 6), 1e-6));
    REQUIRE(particle.getMass() == 10.0);
}

TEST_CASE("Particle with constant acceleration should move with constant acceleration", "[particle]") {
    // Set initial conditions
    Vector3d position(1, 2, 3);
    Vector3d velocity(4, 5, 6);
    double mass = 10.0;

    // Create particle instance
    n_body::Particle particle(position, velocity, mass);

    // Apply constant acceleration
    Vector3d constant_acceleration(0, 0, 2);
    particle.initialAcceleration(constant_acceleration);

    // Perform simulation step
    double dt = 0.1;
    particle.update(dt);

    // Check if the expected values match the calculated values
    REQUIRE(particle.getPosition().isApprox(Vector3d(1.4, 2.5, 3.6)));
    REQUIRE(particle.getVelocity().isApprox(Vector3d(4, 5, 6.2)));
    REQUIRE(particle.getMass() == 10.0);
}

TEST_CASE("Particle with artificial acceleration should orbit in a circle", "[particle]") {

    // Set initial conditions
    Vector3d position(10, 0, 0);
    Vector3d velocity(0, 10, 0);
    double mass = 10.0;

    // Create particle instance
    n_body::Particle particle(position, velocity, mass);
    double dt = 0.001;
    double tot_timesteps = (2 * M_PI)/dt;

    // Perform the simulation for a complete orbit
    for (double t = 0; t < tot_timesteps; ++t) {
        Vector3d artificial_acceleration = -1 * particle.getPosition();
        particle.initialAcceleration(artificial_acceleration);
        particle.update(dt);
    }

    // Check if the expected values match the calculated values
    REQUIRE(particle.getPosition().isApprox(Vector3d(10, 0, 0), 1e-2));
    REQUIRE(particle.getVelocity().isApprox(Vector3d(0, 10, 0), 1e-2));
    REQUIRE(particle.getMass() == 10.0);
}

TEST_CASE("Gravitational force between two particles", "[acceleration]") {

    // Set initial conditions
    double mass = 0.1;
    n_body::particleAcceleration p_i (Vector3d(0, 0, 0), Vector3d::Zero(), mass);
    n_body::particleAcceleration p_j (Vector3d(1, 0, 0), Vector3d::Zero(), mass);
    double epsilon = 0.0;

    // Calculate the expected acceleration by hand
    Vector3d expectAcc (0.1, 0, 0); 
    Vector3d calAcc = n_body::particleAcceleration::calcAcceleration(&p_i, &p_j, epsilon);
    
    // Check if the expected acceleration matches the calculated acceleration
    REQUIRE(calAcc.isApprox(expectAcc));
}

TEST_CASE("No acceleration when particle interacts with itself", "[acceleration]") {
    
    // Set initial conditions
    double mass = 1.0;
    n_body::particleAcceleration p1 = n_body::particleAcceleration(Eigen::Vector3d(1, 0, 0), Eigen::Vector3d::Zero(), mass);
    n_body::particleAcceleration p2 = n_body::particleAcceleration(Eigen::Vector3d(1, 0, 0), Eigen::Vector3d::Zero(), mass);
    p1.addParticle(p2);
    
    // Set expected acceleration to zero
    Eigen::Vector3d expectedAcceleration = Eigen::Vector3d::Zero();
    std::vector <n_body::particleAcceleration*> particle_list = p1.getParticle();

    // Calculate acceleration for each particle
    p1.sumAcceleration(particle_list, 1.0);
    Eigen::Vector3d calculatedAcceleration = n_body::particleAcceleration::calcAcceleration(&p1, &p1, 1.0);

    // Check if the calculated accelerations match the expected accelerations
    for (n_body::particleAcceleration* p_i : particle_list){
        REQUIRE(p_i->getAcceleration().isApprox(expectedAcceleration));
    }
    REQUIRE(calculatedAcceleration.isApprox(expectedAcceleration));

}

TEST_CASE("Zero acceleration with equal particles on either side", "[acceleration]") {

    // Set initial conditions
    double mass = 1.0;
    n_body::particleAcceleration p1 = n_body::particleAcceleration(Eigen::Vector3d(1, 0, 0), Eigen::Vector3d::Zero(), mass);
    n_body::particleAcceleration p2 = n_body::particleAcceleration(Eigen::Vector3d(-1, 0, 0), Eigen::Vector3d::Zero(), mass);
    p1.addParticle(p2);
    
    // Set expected acceleration to zero
    Eigen::Vector3d expectedAcceleration = Eigen::Vector3d::Zero();
    std::vector <n_body::particleAcceleration*> particle_list = p1.getParticle();

    // Calculate acceleration for each particle
    p1.sumAcceleration(particle_list, 1.0);
    Eigen::Vector3d calculatedAcceleration = n_body::particleAcceleration::calcAcceleration(&p1, &p1, 1.0);
    
    // Check if the calculated accelerations match the expected accelerations
    for (n_body::particleAcceleration* p_i : particle_list){
        REQUIRE(p_i->getAcceleration().isApprox(expectedAcceleration));
    }
}

TEST_CASE("Solar System particles list Generator", "[generator]"){

    // Create a simulator with a Solar System generator
    n_body::sysSimulator simulator = n_body::sysSimulator(std::make_shared<n_body::SolarSystemGenerator>());
    std::vector<n_body::particleAcceleration> particle_list = simulator.particleListGenerator();
    
    // Masses in order: Sun, Mercury, Venus, etc.
    std::vector<double> masses = {1., 1. / 6023600, 1. / 408524, 1. / 332946.038, 1. / 3098710, 1. / 1047.55, 1. / 3499, 1. / 22962, 1. / 19352};
    
    // Distances from Sun
    std::vector<double> distances = {0.0, 0.4, 0.7, 1, 1.5, 5.2, 9.5, 19.2, 30.1};
    
    // Check if the number of particles is correct
    REQUIRE(particle_list.size() == masses.size());

    for (int i = 0; i < particle_list.size(); ++i) {
        // Check if the mass is correct
        REQUIRE(particle_list[i].getMass() == Approx(masses[i]));

        // Check if the distance from the Sun is correct (ignoring the Sun itself)
        if (i > 0) {
            double distance = (particle_list[i].getPosition() - particle_list[0].getPosition()).norm();
            REQUIRE(distance == Approx(distances[i]));
        }
    }
}

TEST_CASE("Testing total energy calculation with OpenMP parallelization", "[parallelization]") {

    // Set initial conditions
    int num_particles = 8;
    int seed = 42;
    n_body::sysSimulator simulator = n_body::sysSimulator(std::make_shared<n_body::RandomSystemGenerator>(seed, num_particles));

    // Generate the particle list
    std::vector<n_body::particleAcceleration> particle_list = simulator.particleListGenerator();
    std::vector<double> kinetic_energy_list_final = simulator.kineticEnergy(particle_list);
    std::vector<double> potential_energy_list_final = simulator.potentialEnergy(particle_list);
    std::vector<double> total_energy_list_final = simulator.totalEnergy();
    double total_energy_final = simulator.sumTotalEnergy();
    double total_energy_final_para = simulator.sumTotalEnergyPara(); // with parallelisition

    // Check if the total energy calculated with and without parallelization match
    REQUIRE(total_energy_final == Approx(total_energy_final_para));

}

TEST_CASE("Testing kinetic and potential energy calculation with OpenMP parallelization", "[parallelization]") {
    
    // Set initial conditions
    int num_particles = 8;
    int seed = 42;
    n_body::sysSimulator simulator = n_body::sysSimulator(std::make_shared<n_body::RandomSystemGenerator>(seed, num_particles));
    
    // Generate the particle list
    std::vector<n_body::particleAcceleration> particle_list = simulator.particleListGenerator();

    // Calculate energy values with and without parallelization
    std::vector<double> kinetic_energy_list_final = simulator.kineticEnergy(particle_list);
    std::vector<double> kinetic_energy_list_final_para = simulator.kineticEnergyPara(particle_list); // with parallelisition
    std::vector<double> potential_energy_list_final = simulator.potentialEnergy(particle_list);
    std::vector<double> potential_energy_list_final_para = simulator.potentialEnergyPara(particle_list); // with parallelisition
    
    // Check if the energy calculated with and without parallelization match
    for (int i = 0; i < potential_energy_list_final.size(); ++i){
        REQUIRE(potential_energy_list_final[i] == Approx(potential_energy_list_final_para[i]));
        REQUIRE(kinetic_energy_list_final[i] == Approx(kinetic_energy_list_final_para[i]));
    }
}
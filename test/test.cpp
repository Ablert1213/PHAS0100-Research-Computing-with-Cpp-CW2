#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
// #include "particle.hpp"
#include "acceleration.hpp"
#include <iostream>

using Catch::Matchers::WithinRel;
using Eigen::Vector3d;

TEST_CASE("Particle with no acceleration should move at constant velocity", "[particle]") {
    Vector3d position(1, 2, 3);
    Vector3d velocity(4, 5, 6);
    double mass = 10.0;

    n_body::Particle particle(position, velocity, mass);

    Vector3d zero_acceleration(0, 0, 0);

    particle.initialAcceleration(zero_acceleration);    

    particle.update(0.1);

    REQUIRE(particle.getPosition().isApprox(Vector3d(1.4, 2.5, 3.6), 1e-6));
    REQUIRE(particle.getVelocity().isApprox(Vector3d(4, 5, 6), 1e-6));
    REQUIRE(particle.getMass() == 10.0);
}

TEST_CASE("Particle with constant acceleration should move with constant acceleration", "[particle]") {
    Vector3d position(1, 2, 3);
    Vector3d velocity(4, 5, 6);
    double mass = 10.0;

    n_body::Particle particle(position, velocity, mass);

    Vector3d constant_acceleration(0, 0, 2);

    particle.initialAcceleration(constant_acceleration);

    particle.update(0.1);

    REQUIRE(particle.getPosition().isApprox(Vector3d(1.4, 2.5, 3.6)));
    REQUIRE(particle.getVelocity().isApprox(Vector3d(4, 5, 6.2)));
    REQUIRE(particle.getMass() == 10.0);
}

TEST_CASE("Particle with artificial acceleration should orbit in a circle", "[particle]") {
    Vector3d position(1, 0, 0);
    Vector3d velocity(0, 1, 0);
    double mass = 10.0;

    n_body::Particle particle(position, velocity, mass);

    for (double t = 0; t < 2 * 3.141; t += 0.001) {

        Vector3d artificial_acceleration = -1 * particle.getPosition();
        particle.initialAcceleration(artificial_acceleration);
        particle.update(0.001);
    
    }

    REQUIRE(particle.getPosition().isApprox(Vector3d(1, 0, 0), 1e-1));
    REQUIRE(particle.getVelocity().isApprox(Vector3d(0, 1, 0), 1e-1));
    REQUIRE(particle.getMass() == 10.0);
}

TEST_CASE("Gravitational force between two particles", "[acceleration]") {
    n_body::Particle p_i (Vector3d(0, 0, 0), Vector3d::Zero(), 1.0);
    n_body::Particle p_j (Vector3d(1, 0, 0), Vector3d::Zero(), 1.0);
    double epsilon = 0.0;

    Vector3d expectAcc (1, 0, 0); // Calculate the expected acceleration by hand
    Vector3d calAcc = n_body::particleAcceleration::calcAcceleration(p_i, p_j, epsilon);
    REQUIRE(calAcc.isApprox(expectAcc));
}

TEST_CASE("No acceleration when particle interacts with itself", "[acceleration]") {
    n_body::particleAcceleration p1 = n_body::particleAcceleration(Eigen::Vector3d(1, 0, 0), Eigen::Vector3d::Zero(), 1.0);
    n_body::particleAcceleration p2 = n_body::particleAcceleration(Eigen::Vector3d(1, 0, 0), Eigen::Vector3d::Zero(), 1.0);
    p1.addParticle(p2);
    
    Eigen::Vector3d expectedAcceleration = Eigen::Vector3d::Zero();
    p1.sumAcceleration(p1.getParticle(), 1.0);

    std::vector <n_body::particleAcceleration> particle_list = p1.getParticle();
    Eigen::Vector3d calculatedAcceleration = n_body::particleAcceleration::calcAcceleration(p1, p1, 1.0);

    for (n_body::Particle &p_i : particle_list){
        REQUIRE(p_i.getAcceleration().isApprox(expectedAcceleration));
    }
    REQUIRE(calculatedAcceleration.isApprox(expectedAcceleration));

}

TEST_CASE("Zero acceleration with equal particles on either side", "[acceleration]") {
    n_body::particleAcceleration p1 = n_body::particleAcceleration(Eigen::Vector3d(1, 0, 0), Eigen::Vector3d::Zero(), 1.0);
    n_body::particleAcceleration p2 = n_body::particleAcceleration(Eigen::Vector3d(-1, 0, 0), Eigen::Vector3d::Zero(), 1.0);
    p1.addParticle(p2);
    
    Eigen::Vector3d expectedAcceleration = Eigen::Vector3d::Zero();
    p1.sumAcceleration(p1.getParticle(), 1.0);

    std::vector <n_body::particleAcceleration> particle_list = p1.getParticle();
    Eigen::Vector3d calculatedAcceleration = n_body::particleAcceleration::calcAcceleration(p1, p1, 1.0);

    for (n_body::Particle &p_i : particle_list){
        REQUIRE(p_i.getAcceleration().isApprox(expectedAcceleration));
    }
}

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "particle.hpp"
#include <iostream>

using Catch::Matchers::WithinRel;
using Eigen::Vector3d;

TEST_CASE("Particle with no acceleration should move at constant velocity", "[particle]") {
    Vector3d position(1, 2, 3);
    Vector3d velocity(4, 5, 6);
    double mass = 10.0;

    Particle particle(position, velocity, mass);

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

    Particle particle(position, velocity, mass);

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

    Particle particle(position, velocity, mass);

    for (double t = 0; t < 2 * 3.141; t += 0.001) {

        Vector3d artificial_acceleration = -1 * particle.getPosition();
        particle.initialAcceleration(artificial_acceleration);
        particle.update(0.001);
    
    }

    REQUIRE(particle.getPosition().isApprox(Vector3d(1, 0, 0), 1e-1));
    REQUIRE(particle.getVelocity().isApprox(Vector3d(0, 1, 0), 1e-1));
    REQUIRE(particle.getMass() == 10.0);
}
# A Virtual Solar System

This is the starting repository for assignment 2 of PHAS0100: Research Computing with C++. You may add or remove C++ files in any directory. You should organise the files as you see fit but do read the Folder Structure section below to understand the intended use of the existing folders.

## Installing dependencies

We are using the package manager Conan to install the dependencies Catch2 and Eigen. In order to use CMake's `Release` target for performance and `Debug` for debugging, the libraries must be installed twice with:

```
conan install . --output-folder=build --build=missing -s build_type=Debug
conan install . --output-folder=build --build=missing -s build_type=Release
```

If you delete the `build` directory to clean your build, you may have to install the dependencies again.

## Building

To build from the project root directory you should run:

```
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

If you wish to debug your code, you should replace `Release` with `Debug`. For performance measurements, ensure you have built with the `Release` target.

## Testing

Once the project has been built, it can be tested by running:

```
cd build
ctest
```

## Folder structure

The project is split into four main parts aligning with the folder structure described in [the relevant section in Modern CMake](https://cliutils.gitlab.io/modern-cmake/chapters/basics/structure.html):

- `app/` contains all code implementing the command-line application.
- `lib/` contains all non-app code. Only code in this directory can be accessed by the unit tests.
- `include/` contains all `.hpp` files.
- `test/` contains all unit tests.

You are expected to edit the `CMakeLists.txt` file in each folder to add or remove sources as necessary. For example, if you create a new file `test/particle_test.cpp`, you must add `particle_test.cpp` to the line `add_executable(tests test.cpp)` in `test/CMakeLists.txt`. Please ensure you are comfortable editing these files well before the submission deadline. If you feel you are struggling with the CMake files, please see the Getting Help section of the assignment instructions.

## Usage Instructions

You should fill in the instructions for using the app here.

## Credits

This project is maintained by Dr. Jamie Quinn as part of UCL ARC's course, Research Computing in C++.

## 2.1 Calculating numerical energy loss results summarizes.

In summary, the total energy drop over a single simulation run varies depending on the time step (dt) used in the simulation. As the time step increases, the simulation becomes less accurate in general, and more energy is lost.

Here are the summarized results (run for 100 years) for each run:

dt = 0.1, Initial total energy: -0.000112417, Final total energy: -7.48918e-05, Energy drop: -33.3802%;
dt = 0.01, Initial total energy: -0.000112417, Final total energy: -0.000102496, Energy drop: -8.82516%;
dt = 0.008, Initial total energy: -0.000112417, Final total energy: -0.000103841, Energy drop: -7.62855%;
dt = 0.004, Initial total energy: -0.000112417, Final total energy: -0.000106866, Energy drop: -4.93741%;
dt = 0.001, Initial total energy: -0.000112417, Final total energy: -0.000109813, Energy drop: -2.31664%;
dt = 0.0008, Initial total energy: -0.000112417, Final total energy: -0.00011009, Energy drop: -2.06997%;
dt = 0.0004, Initial total energy: -0.000112417, Final total energy: -0.000110784, Energy drop: -1.45247%;
dt = 0.0001, Initial total energy: -0.000112417, Final total energy: -0.000111695, Energy drop: -0.641777%;

As seen from the results, the energy drop becomes larger when dt is larger, indicating that the simulation is less accurate with a larger time step.

## 2.2 Benchmarking the situation

Run the simulation with compiler optimizations:
```
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-O2"
cmake --build build
```

Run the simulation without compiler optimizations:
```
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS="-O0"
cmake --build build
```
Based on the performance, the difference with and without complier optimizations is quite significant. With compiler optimizations enabled (using -O2), the average time per timestep remains relatively constant which are ranged from 1.46544e-06 to 1.63359e-06 seconds. However, without compiler optimizations (using -O0), the average time per timestep is much higher, at 5.74133e-05 seconds. So, in this case with same '0.001 100' input, the simulation without complier optimizations ran 0.60123 mins in total which is way more longer than the simulation using -O2 (0.0153516 mins in total).
In terms of accuracy, the total energy drop decreases as the timestep size decreases. To strike a balance between simulation run time and accuracy, a timestep size of 0.001 could be a good choice. With this timestep, the total time is 0.0153516 minutes, and the total energy drop is around -2.31664%. Compared with timestep size of 0.004 the energy drop decreased 53%, and compared with timestep size of 0.0008 the energy drop only less 10%. Which indicates this provides a reasonable simulation run time while maintaining an acceptable level of accuracy. However, under 'a few minutes is reasonable' condition, with timestep 0.00001, it gets -0.103798% of total energy drop after 1.688 minute.

## 2.3 Increasing the scale of the system

With timestep(dt) equal to 0.001 and softening acceleration calculation equal to 0.001 after 1 year (2π time).

The performance of the solar system simulator for each case is as follows:

1, 8 initial particles:
Initial energy: -0.0013983
Total time: 0.000178207 mins
Average time per timestep: 1.70175e-06 seconds
Final energy: -0.00125763
Total energy drop: -10.0603%

2, 64 initial particles:
Initial energy: -0.00290945
Total time: 0.00830786 mins
Average time per timestep: 7.93342e-05 seconds
Final energy: -0.00247646
Total energy drop: -14.8822%

3, 256 initial particles:
Initial energy: -0.010686
Total time: 0.129788 mins
Average time per timestep: 0.00123939 seconds
Final energy: 0.2627
Total energy drop: -2558.34%

4, 1024 initial particles:
Initial energy: -0.0474887
Total time: 2.22621 mins
Average time per timestep: 0.0212588 seconds
Final energy: 2.20298
Total energy drop: -4738.95%

5, 2048 initial particles:
Initial energy: -0.112992
Total time: 9.33237 mins
Average time per timestep: 0.0891176 seconds
Final energy: 74.2086
Total energy drop: -65775%

Generally speaking. From the performance results of the solar system simulator, it can be observed that as the number of initial particles increases, the system's initial energy also increases, which should be expected, as more particles contribute to the gravitational potential energy of the system. 
The total energy drops percentage increases significantly as the number of particles increases. This could indicate that the simulation becomes less accurate and stable for larger numbers of particles. Energy conservation might be affected by the increasing complexity of the interactions, numerical errors, or the choice of the timestep.
And with the increase in the number of particles, the total time and average time per timestep required to complete the simulation also increase because the computational complexity increases as more particles are introduced, leading to more calculations required for each timestep.

## 2.4 Parallelising with OpenMP

# a.
At the beginning, I parallelise the two for-loops in the 'solarSystemSimulator.cpp' for updating accelerations and position/velocities. Simply use '#pragma omp parallel for' directive.
The simulator with timestep 0.003, softening factor 0.001 and 2048 number of particles, the total time drop from 3.16945 mins (Average time per timestep: 0.0907981 seconds) to 0.914399 mins (Average time per timestep: 0.0261956 seconds), it gets 71% quicker (significant improvement) than the one without parllelising (I take this as benchmark for below experiments). 

# Experiment with the collapse and schedule clauses and comment on the performance differences.
1. I parallelise the loops that involve adding particles 'void sysSimulator::addSysInput' function by using '#pragma omp parallel for schedule(dynamic) collapse(1)' directive. The simulator with timestep 0.003, softening factor 0.001 and 2048 number of particles, the total time drop from 0.914399 mins (Average time per timestep: 0.0261956 seconds) to 0.79011 mins (Average time per timestep: 0.022635 seconds), it gets 14% quicker than the experiment before without parllelising. 
2. The next experiment, I parallelize the loops that involve computing the sum of total energy 'double sysSimulator::sumTotalEnergy' function by using '#pragma omp parallel for reduction(+:sum_tot_energy)' directive. The simulator with timestep 0.003, softening factor 0.001 and 2048 number of particles, the total time increases from 0.79011 mins (Average time per timestep: 0.022635 seconds) to 0.932914 mins (Average time per timestep: 0.026726 seconds).  
3. Moreover, I parallelise the loop that involves the calculation of the sum of accelerations 'void particleAcceleration::sumAcceleration' function by using '#pragma omp parallel for schedule(dynamic) reduction(+:sumAcceleration_i)' directive.The simulator with timestep 0.003, softening factor 0.001 and 2048 number of particles, the total time increases from 0.932914 mins (Average time per timestep: 0.026726 seconds) to 1.45148 mins (Average time per timestep: 0.0415817 seconds).
Both experiment 2 and 3 get worse performance, which could be these following reasons: It may because the problem size is not large enough the overhead might overshadow the potential speedup obtained from parallelization. And it could be the parallelisation strategy does not efficiently utilize the available resources and does not effectively reduce the problem size, therefore the performance might get worse.
Therefore, in this part I keep the code only with the change made in experiment 1.

# b.
In the part, I compiled the code with the optimization level set to -O2 as benchmark.

For the Strong Scaling Experiment, I run the simulations with different thread counts, starting form 1 and increasing up to the number 10. The runtime with a single thread is larger than 30 seconds.
  | 'OMP_NUM_THREADS' | Time(<units>) | Speedup |
  |---|---|---|
  | 1 | 2.92736 mins |-------|
  | 2 | 1.64597 mins | 1.78x |
  | 3 | 1.33830 mins | 2.19x |
  | 4 | 1.14702 mins | 2.55x |
  | 5 | 1.13136 mins | 2.59x |
  | 6 | 1.06746 mins | 2.74x |
  | 7 | 1.05932 mins | 2.76x |
  | 8 | 1.05127 mins | 2.78x |
  | 9 | 1.04113 mins | 2.81x |
  | 10 | 0.98229 mins | 2.98x |

The strong scaling experiment shows that as the number of threads increases, the overall execution time decreases, and the speedup factor increases. This is expected, as the workload is divided among more threads, allowing the computation to be completed more quickly. However, the speedup does not scale linearly with the number of threads (the idea scaling or perfect scaling in a strong scaling experiment would be a linear improvement). For instance, going from 1 to 2 threads results in a 1.78x speedup, and going from 1 to 10 threads results in a 2.98x speedup. This indicates that there are diminishing returns as more threads are added.

 For the weak scaling experiment, I run the simulations with the same thread counts as in the strong scaling experiment, but increase the number of particles proportionally to the number of threads. (In this case, I choose to start at 256 and increase 256 particles each time)
  | 'OMP_NUM_THREADS' | Num Particles | Time(<units>) | Speedup |
  |---|---|---|---|
  | 1  | 256  | 0.0443601 mins |--------|
  | 2  | 512  | 0.0922988 mins | 0.480x |
  | 3  | 768  | 0.1423420 mins | 0.310x |
  | 4  | 1024 | 0.2346730 mins | 0.190x |
  | 5  | 1280 | 0.4532300 mins | 0.098x |
  | 6  | 1536 | 0.6100640 mins | 0.072x |
  | 7  | 1792 | 0.8772010 mins | 0.050x |
  | 8  | 2048 | 1.1778400 mins | 0.038x |
  | 9  | 2304 | 1.2501600 mins | 0.035x |
  | 10 | 2560 | 1.6581000 mins | 0.027x |

The weak scaling experiment demonstrates that as the number of threads and the problem size (number of particles) both increase, the execution time increases, and the speedup factor decreases. This result is expected, as the workload grows proportionally with the number of threads, and there is a constant amount of work per thread. However, the speedup factor decreases more quickly than expected (In a perfectly weak scaling code, the speedup should be exactly 1 with any number of threads), suggesting that the performance might not scale well with the problem size.







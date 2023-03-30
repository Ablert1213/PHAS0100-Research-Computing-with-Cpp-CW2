add_executable(solarSystemSimulator solarSystemSimulator.cpp)
target_compile_features(solarSystemSimulator PUBLIC cxx_std_17)
target_include_directories(solarSystemSimulator PUBLIC ../include)

add_executable(solarSystemSimulator2 main.cpp)
target_compile_features(solarSystemSimulator PUBLIC cxx_std_17)
target_include_directories(solarSystemSimulator PUBLIC ../include)

find_package(Eigen3 3.4 REQUIRED)
find_package(OpenMP REQUIRED)

target_link_libraries(solarSystemSimulator PUBLIC Eigen3::Eigen OpenMP::OpenMP_CXX nbody_lib)
target_link_libraries(solarSystemSimulator2 PUBLIC Eigen3::Eigen OpenMP::OpenMP_CXX nbody_lib)
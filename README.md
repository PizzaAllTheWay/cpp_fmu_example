# Compiles cpp to FMU

## Strucure
All cpp FMU code is contained in [src](./src). 
The name of the folder in src is the same as the name of the FMU. 
To make more FMUs make a new folder with the FMU name, Then add the FMU folder to the CMakeList.txt file. you can copy the formating for the MySimpleFMU folder.

## Build and testing
For building use the "(operating system)-build_my_fmu" wich will build all the fmus
to test your fmus you can execute "(operating system)-verify_my_fmu" which you can edit to verify specific individual or multiple FMUS, as well as seting their parameters and initial conditions for the simulation and plot their response.

## Dependencies
Code dependencies that are taken from other repositories (mostly Open Simulation Platform):
- [cmake](./cmake), [fmi](./fmi) and [CMakeLists.txt](./CMakeLists.txt) is from [this repo](https://github.com/open-simulation-platform/cpp-fmus)
- [cppfmu](./cppfmu/) is from [this repo](https://github.com/viproma/cppfmu)

Dependencies to build and test FMUs
- CMake (v3.0)
- FMPy (v0.3.20) (Python library)

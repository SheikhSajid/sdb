# Build Instructions

## Prerequisites

- **CMake**
- **C++17 compiler** (e.g., g++, clang++)
- **vcpkg**

## Install Prerequisites

1. **Install build tools:**
   ```sh
   apt update
   apt install -y build-essential cmake ninja-build
   ```

2. **Clone and build vcpkg:**
   ```sh
   pushd ..
   git clone https://github.com/microsoft/vcpkg.git
   VCPKG_FORCE_SYSTEM_BINARIES=1 ./vcpkg/bootstrap-vcpkg.sh
   popd
   ```

## Build Steps

1. **Create a build directory:**
   ```sh
   mkdir build
   cd build
   ```

2. **Run CMake to configure the project:**
   ```sh
   cmake .. -DCMAKE_TOOLCHAIN_FILE=/root/vcpkg/scripts/buildsystems/vcpkg.cmake
   ```

3. **Build the library:**
   ```sh
   cmake --build .
   ```

4. **Run the program:**
   ```sh
   ./tools/sdb
   ```

This will compile the `libsdb` library as defined in `src/CMakeLists.txt`. The output library will be named `libsdb` (or `sdb`).

## Installing Packages using vcpkg

```sh
pushd ~/vcpkg
VCPKG_FORCE_SYSTEM_BINARIES=1 ./vcpkg install <pkg_name>
popd
```

# Project File Tree

This project should be placed at `/root/sdb` in an iMX 6 powered device and contains the following main directories and files:

```
sdb/
├── CMakeLists.txt
├── include/
│   └── libsdb/            # public headers used by consumers
├── src/
│   ├── include/           # private headers
│   ├── libsdb.cpp         # library
│   └── CMakeLists.txt
├── test/                
│   └── CMakeLists.txt
└── tools/
    ├── CMakeLists.txt
    └── sdb.cpp            # program that uses libsdb
```

## Directory Descriptions
- **CMakeLists.txt**: Main build configuration file for CMake.
- **include/**: Stores header files, including the `libsdb/` subdirectory for additional headers.
- **src/**: Contains the main source code and its own CMake configuration.
- **test/**: Contains test code and its own CMake configuration.
- **tools/**: Contains utility scripts or tools, with its own CMake configuration.

This README provides a high-level overview of the project structure to help you navigate and understand the organization of the codebase.

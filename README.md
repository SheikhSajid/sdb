# Build Instructions

## Prerequisites

- **CMake** (install with `apt install cmake` on Debian)
- **C++17 compiler** (e.g., g++, clang++)

## Steps

1. **Create a build directory:**
   ```sh
   mkdir build
   cd build
   ```

2. **Run CMake to configure the project:**
   ```sh
   cmake ..
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


# Project File Tree

This project should be placed at `/root/sdb` in an iMX 6 powered device and contains the following main directories and files:

```
my_debugger/
├── CMakeLists.txt
├── include/
│   └── libsdb/          # public headers used by consumers
├── src/
│   └── include          # private headers
│   └── CMakeLists.txt
├── test/                
│   └── CMakeLists.txt
└── tools/
    └── CMakeLists.txt
```

## Directory Descriptions
- **CMakeLists.txt**: Main build configuration file for CMake.
- **include/**: Stores header files, including the `libsdb/` subdirectory for additional headers.
- **src/**: Contains the main source code and its own CMake configuration.
- **test/**: Contains test code and its own CMake configuration.
- **tools/**: Contains utility scripts or tools, with its own CMake configuration.

This README provides a high-level overview of the project structure to help you navigate and understand the organization of the codebase.

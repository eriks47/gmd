# Generate Method Definitions

A regex based C++ method and function implementation
generator.

### Building

##### Requirements

cmake (version 3.20 or above)   
vcpkg
c++23

##### Build proccess

```shell
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/script
cmake --build Build
```
### Running

After the build has been performed the 
program can be ran the following
way:

```shell
./Build/main
```

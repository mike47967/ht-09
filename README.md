# PrintRandom

This repository contains the implementation of a program `printrandom` that generates random numbers using different random number generator (RNG) algorithms. The program is modular, with implementations split across multiple source files, and is compiled using a `Makefile` to ensure portability and maintainability.

## Features

- **RNG Algorithms**:
  - **Linear Congruential Generator (LCG)**: Implements a generator with multiplier `1103515245`, increment `12345`, and modulus `2^31`.
  - **Mersenne Twister (MT)**: Implements the 32-bit Mersenne Twister algorithm using the MTwister project.
- **Factory Pattern**: Provides a flexible API for creating RNG objects with initialization parameters.
- **Dynamic Linking**: Uses `dlopen` and `dlsym` for resolving function addresses, supporting multiple RNG implementations in a single binary.
- **Command-line Interface**: Allows users to specify the generator type, the number of random numbers to generate, and initialization parameters.

## Requirements

- **Dependencies**:
  - A C compiler (e.g., GCC or Clang).
  - Standard C library and POSIX-compliant dynamic linking support.

- **Build System**:
  - The program uses a `Makefile` with the following customizable variables:
    - `CC`: Compiler (default: `gcc`).
    - `LD`: Linker (default: `gcc`).
    - `CFLAGS`: Compiler flags.
    - `LDFLAGS`: Linker flags.
  - Compilation and linking are designed to work with `-rdynamic`.

## Project Structure

- `main.c`: Implements the main program logic and command-line argument parsing.
- `random_source.h`: Defines the API for RNG objects and their operations.
- `linear.c`: Implements the Linear Congruential Generator and its factory function `random_linear_factory`.
- `mt.c`: Implements the Mersenne Twister generator using the MTwister project and its factory function `random_mt_factory`.
- `Makefile`: Automates the compilation and linking process.

### Usage

## Build the Program

Run the following command to build the program:

```bash
make
```

This generates the executable printrandom.

##Run the Program

#The program takes the following command-line arguments:

- `Generator Type`: linear or mt.
- `Count`: Number of random numbers to generate (0 to 100,000).
- `Seed`: Initialization parameter for the chosen RNG.

#Example usage:

```bash
./printrandom linear 10 12345
```

This generates 10 random numbers using the Linear Congruential Generator with a seed of 12345.

#Example Output

For the above command, the output might look like:

```bash
0.3451234567
0.8901234567
...
```

##Clean Up

To remove generated files:

```bash
make clean
```

###Implementation Details
##Random Source API

The random source objects are defined using the following structure:

- ***RandomSourceOperations***:
    - `destroy`: Frees resources and returns NULL.
    - `next`: Returns the next random number in the range [0, 1).
- ***RandomSource***:
    - Contains a pointer to RandomSourceOperations for polymorphic behavior.

##Factory Functions

- `random_linear_factory`: Creates an LCG-based RNG object. The seed is interpreted as a positive integer using the lower 31 bits.
- `random_mt_factory`: Creates an MT-based RNG object. The seed is interpreted as a 32-bit unsigned integer.

##Main Program Logic

- Parses command-line arguments.
- Dynamically loads the appropriate factory function using dlsym.
- Creates the RNG object and generates the requested numbers.
- Cleans up resources by calling the destroy method.

##Notes

- Dynamic Linking: The program is linked with -rdynamic and uses dlopen for dynamic symbol resolution.
- Portability: The Makefile avoids hardcoding flags in CFLAGS and LDFLAGS, allowing them to be overridden.

##License

This project is licensed under the MIT License.

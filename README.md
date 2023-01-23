# Parallel version of Tarjan's algorithm

Project description: This project is a distributed application using MPI and OpenMP to find strongly connected components using the Tarjan algorithm.

## Requirements
- C compiler supporting MPI and OpenMP
- Math.h library

## Compilation
To compile the project, use the provided makefile. Open a terminal window and navigate to the project directory, then run the `make` command

cd /path/to/project
make

## Execution
To execute the project, use the mpirun or mpiexec command, specifying the number of processes to be used.

mpirun -np 4 ./main

## License
This project is released under the GNU General Public Licence. Please read the LICENSE file for more details.
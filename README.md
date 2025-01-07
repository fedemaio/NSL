# NSL

## BASIC FILE ORGANISATION OF EACH FOLDER

- Jupyter Notebook containing an explanation of the exercises and representations of the results (eventually some –commented–Python scripts to manage the input/output files and run the executable quickier): exe*.ipynb
- source code: main*.cpp 
- files for the Random Number Generator: random.h, random.cpp, Primes, Primes32001.in, seed.in
- output files with simulation results: .out or .output or .dat (eventually contained in proper folders)
- Makefile
- executable: main.exe 
- text of the exercises LSN_Exercise_*.ipynb
- Zone.Identifier are some default files created by Windows as some files were copied in their proper folder from another environment

## SPECIFIC NOTES FOR EACH EXERCISE

### Exercise1
- source code is divided in main1.1.cpp, main1.2.cpp and main1.3.cpp and I manually change the Makefile to compile

### Exercise2
- source code name: main2.cpp

### Exercise3
- source code is divided in main3.1.cpp and main3.2.cpp and I manually change the Makefile to compile

### Exercise4
- input files in INPUT folder
- source code name: NSL_SIMULATOR.cpp (in /NSL_SIMULATOR/SOURCE folder)
- class particle.cpp and its header file: contains methods to work with particles of matter
- class system.cpp and its header file system.h: contains methods to perform a melocular dynamic simulation
- exe4.ipynb is in /NSL_SIMULATOR/OUTPUT folder

### Exercise5
- is not present because it wasn't supposed to be done by bachelor's students

### Exercise6
- input file like exercise 4
- source code as exercise 4
- classes like exercise 4
- exe6tentativi.ipynb is a neglectable Jupyter Notebook used for tests

### Exercise7
- input file like exercise 4
- source code as exercise 4
- classes like exercise 4

### Exercise8
- class groundstate_SA.cpp and its header file groundstate_SA.h: contains methods to calculate the gound state of an hamiltonian with simulated annealing

### Exercise9
- class tour.cpp and its makefile tour.h. contains methods to manage a vector of cities (tour), their positions and index
- class population.cpp and its header file population.h: contains methods to apply genetic algorithms on a TSP

### Exercise10
- classes tour and population like in exercise 9
- main*.cpp tests source files
- the real source code is main.cpp for communicating processors and mainind.cpp for non communicating processors
- to compile: mpic++ -o tsp_parallel(l) main(ind).cpp population.cpp random.cpp tour.cpp
- to execute: mpirun --use-hwthread-cpus -np 4(6) ./tsp_parallel(l)

### Exercise11
- this exercise is performed only on Jupyter Notebook
- exe11.*_mock.ipynb contains python boxes to perform the imulations
- *.png of plots made in exe11. *_mock.ipynb

### Exercise12
- this exercise is performed only on Jupyter Notebook
- number.png singole pictures of my own hanwriting to test in 12.3

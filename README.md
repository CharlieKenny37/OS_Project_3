# Project 3 - by Charlie Kenny and Ryan Donald 4/10/2023

This repository contains a file which simulates 5 different scheduling algorithms for a given input data set. These are First Come First Serve, Shortest Job First, Shortest Time to Completion First, and Round Robin. The data set is input in a text file, with each process existing on a single line in the format "<CPU Burst Time> <Priority> <Arrival Time>". 

## Compiling the Programs
To compile the program, unzip the program using an archive manager of your choosing, and then place the unzipped folder in a 
directory of your choosing. Note before running the program, the g++ compiler and make need to be installed on the linux system. Then open a terminal that has the folder directory of the unzipped folder open. Finally, run the following commands:

    In the src/ directory:
    make

This will create the project3 executable file.

## Running the Programs
While still in the src/ directory, run the following command to run the program:

    ./project3 <input file> <output file> <Snapshot Cycle Interval>

Replace <input file> with an input .dat file, in the correct format, of your choosing. There are two files included, "test1.dat" and "test2.dat"
Replace <output file> with a .txt file of your choosing.
Replace <Snapshot Cycle Interval> with an integer representing the number of cycles of your choosing between each scheduler snapshot.

## Output

Here is an example output for the program based on the shown command:



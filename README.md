# Individual Assignment 2 - by Charlie Kenny 3/15/2023

This repository contains three seperate homework solution programs from the textbook "Operating Systems: Three Easy Pieces": fork-join.c, rendezvous.c, and barrier.c. Each program shows off how semaphores can be utilized to perform different thread syncronization tasks. Here are the details for each program:
- join-fork.c: In this program, a parent and child thread both print a message to the console, and the semaphores are used to ensure that the message created by the child always appears before the parent's end message.
- rendezvous.c: In this program, a parent and child thread both print a message to the console, and the semaphores are used to ensure that the message created by the child thread always appears before the parent's end message.
- barrier.c: In this particular example, an arbitrarty number of threads (specified by command line args) will print a message that includes the word "before" before all the the threads continue to print a message with the word "after".

## Compiling the Programs
To compile the program, unzip the program using an archive manager of your choosing, and then place the unzipped folder in a 
directory of your choosing. Note before running the program, the gcc compiler and make need to be installed on the linux system. Then open a terminal that has the folder directory of the unzipped folder open. Finally, run the following commands:

    In the src/ directory:
    make

This will create the fork_join, rendezvous, and barrier executable files.

## Running the Programs
While still in the src/ directory, run the following commands to run any of the three programs:

    1. join-fork.c: ./join_fork
    2. rendezvous.c: ./rendezvous
    3. barrier.c: ./barrier <number of threads to be created>

Replace <number of threads to be created> with the desired number of child threads to be created in the barrier program.

## Output

Here are some example outputs for all three programs based on the shown commands

./join_fork

parent: begin
child
parent: end

./rendezvous

parent: begin
child 1: before
child 2: before
child 2: after
child 1: after
parent: end


./barrier 10

parent: begin
child 1: before
child 0: before
child 3: before
child 2: before
child 4: before
child 5: before
child 6: before
child 7: before
child 8: before
child 9: before
child 1: after
child 0: after
child 3: after
child 5: after
child 8: after
child 6: after
child 7: after
child 9: after
child 4: after
child 2: after
parent: end

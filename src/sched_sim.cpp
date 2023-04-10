#include <stdlib.h>
#include <time.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <sched_sim.h>

using namespace std;



// Program Constructor
Program::Program(int cpu_burst, int priority)
{
    this->pid = this->program_counter;
    this->program_counter++;
    this->priority = priority;
    this->cpu_burst = cpu_burst;
    this->wait_time = 0;
    this->turnaround_time = 0;
}


// Program functions
void Program::wait()
{
    this->wait_time++;
    this->turnaround_time++;
}
    
void Program::run_cycle()
{
    if(this->turnaround_time - this->wait_time < cpu_burst)
        this->turnaround_time++;
}

int Program::get_burst_time() {
    return this->cpu_burst;
}
    
int Program::get_priority() {
    return this->priority;
}

int Program::get_turnaround_time() {
    return this->turnaround_time;
}
    
bool Program::finished()
{
    return turnaround_time == cpu_burst;
}




//FCFS
void FCFS_Scheduler::update_queue(Program input) {
    this->queue.push_back(input);

}

//SJF
void SJF_Scheduler::update_queue(Program input) {
    // burst time of input
    int time = input.get_burst_time();
    //iterates accross list in reverse order
    for (std::list<Program>::iterator it = this->queue.end(); it != this->queue.begin(); --it) {
        //if input burst time >= current index burst_time, add behind current index
        if(time >= (*it).get_burst_time()) {
            this->queue.insert(it++, input);
        }
    }

}

//STCF  THIS SHOULD PREEMPT IF TIME < CURRENT PROGRAM TIME SOMEHOW
void STCF_Scheduler::update_queue(Program input) {
    // burst time of input
    int time = input.get_burst_time();
    //iterates accross list in reverse order
    for (std::list<Program>::iterator it = this->queue.end(); it != this->queue.begin(); --it) {
        //if input burst time >= current index burst_time, add behind current index
        if(time >= (*it).get_burst_time()) {
            this->queue.insert(it++, input);
        }
    }

}

//Round Robin
void RR_Scheduler::update_queue(Program input) {
    this->queue.push_back(input);

}

void Program_Spawner::read_program_file(std::string file_name)
{
    string line;
    ifstream program_input_file(file_name);
    
    if (program_input_file.is_open())
    {
        while ( getline (program_input_file,line) )
        {
            std::vector<int> space_locations;
            std::size_t found_index = -1; 

            // Find the indexes of all the space charaters in the string
            while(found_index != std::string::npos)
            {
                found_index = line.find(' ', found_index + 1);

                if(found_index != std::string::npos)
                    space_locations.push_back(found_index);
            }

            // Now that all the locations of the spaces have been found, perform
            // substring operations and parse the separated numbers

            std::string program_args[3];
            for(int i = 0; i < sizeof(program_args); i++)
            {
                if(i == 0)
                    program_args[i] = line.substr(0, space_locations[0]);
                else
                    program_args[i] = line.substr(space_locations[i-1] + 1, space_locations[i] - space_locations[i-1]);
            }

            // The number arguments for a program have been split. Now parse the string numbers into integers and create the program object
            int cpu_burst = stoi(program_args[0]);
            int priority = stoi(program_args[1]);
            int arrival_time = stoi(program_args[2]);

            Program new_program = Program(cpu_burst);

            // Add the program to the queue list
            this->queue.push_back(new_program);
        }

        program_input_file.close();
    }

    else cout << "Unable to open file"; 
}

//?
std::list<Program> Program_Spawner::run_spawner()
{
    std::list<Program> spawned_programs;

    for (std::vector<Program>::iterator it = this->queue.; it != this->queue.begin(); --it) {
        //if input burst time >= current index burst_time, add behind current index
        if(time >= (*it).get_burst_time()) {
            this->queue.insert(it++, input);
        }
    }
}


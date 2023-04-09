#include <stdlib.h>
#include <time.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <sched_sim.h>

using namespace std;

// Program Constructor
Program::Program(int pid, int cpu_burst)
{
    this->pid = pid;
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
    
bool Program::finished()
{
    return turnaround_time == cpu_burst;
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

            for(int i = 0; i < space_locations.size; i++)
            {
                
            }
        }
        program_input_file.close();
    }

    else cout << "Unable to open file"; 
}



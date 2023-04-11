#include <stdlib.h>
#include <time.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <sched_sim.h>

using namespace std;


// Program Constructor
Program::Program(int cpu_burst, int priority, int arrival_time)
{
    this->pid = this->program_counter;
    this->program_counter++;
    this->cpu_burst = cpu_burst;
    this->priority = priority;
    this->arrival_time = arrival_time;
    this->wait_time = 0;
    this->turnaround_time = 0;
}

// Program functions
void Program::wait()
{
    wait_time++;
    turnaround_time++;
}
    
void Program::run_cycle()
{
    if(turnaround_time - wait_time < cpu_burst)
        turnaround_time++;
}

bool Program::finished()
{
    return turnaround_time == cpu_burst;
}

int Program::get_pid() { return pid; }

int Program::get_burst_time() { return cpu_burst; }

int Program::get_priority() { return priority; }

int Program::get_arrival_time() { return arrival_time; }

int Program::get_wait_time() { return wait_time; }

int Program::get_turnaround_time() { return turnaround_time; }
    



// Scheduler base functions
bool Scheduler::is_empty() { return queue.empty(); }

Program* Scheduler::load_program()
{
    in_loading_state = true;

    // save a pointer to the finished or null program that was previously running
    Program* previous_program = running_program;

    // Take the program from queue and load it into running_program
    running_program = &(queue.front());
    queue.pop_front();

    // return previous program
    return previous_program;
}

void Scheduler::run()
{
    // Start by clearing loading state to false
    in_loading_state = false;

    // Check if there is a program running to start a load or manage a running program
    if(running_program == NULL)
    {
        // There is nothing happening in the scheduler, so prep to load a program
        if( !queue.empty() )
        {
            // Don't need return value since we know running program is null
            load_program();

            // DO NOT CALL WAIT OR RUN ON LOADING PROGRAM
        }
    }
    else
    {
        // Check if the current running program has finished
        if(running_program->finished())
        {
            finished_programs.finished_programs.push_back( &(load_program()) );
        }
    }
}

void Scheduler::document_status()
{
    // First add the time to the printout
    cout << "t = " << time << std::endl;

    // Next, there are 4 states the scheduler can be in: loading the first program, running a program,
    // finishing a program and loading the next program, and finishing the final program. 
}




//FCFS
FCFS_Scheduler::FCFS_Scheduler()
{
    cout << "***** FCFS Scheduling *****" << std::endl;
}

void FCFS_Scheduler::add_program(Program program)
{
    queue.push_back(program);
}

//SJF
SJF_Scheduler::SJF_Scheduler()
{
    cout << "***** SJF Scheduling *****" << std::endl;
}

void SJF_Scheduler::add_program(Program program)
{
    // burst time of input
    int time = program.get_burst_time();

    //iterates accross list in reverse order
    for (std::list<Program>::iterator it = this->queue.end(); it != this->queue.begin(); --it) {
        //if input burst time >= current index burst_time, add behind current index
        if(time >= (*it).get_burst_time()) {
            this->queue.insert(it++, program);
        }
    }
}

STCF_Scheduler::STCF_Scheduler()
{
    cout << "***** STCF Scheduling *****" << std::endl;
}

//STCF  THIS SHOULD PREEMPT IF TIME < CURRENT PROGRAM TIME SOMEHOW
void STCF_Scheduler::add_program(Program program) {
    // burst time of input
    int time = program.get_burst_time();
    //iterates accross list in reverse order
    for (std::list<Program>::iterator it = this->queue.end(); it != this->queue.begin(); --it) 
    {
        //if input burst time >= current index burst_time, add behind current index
        if(time >= (*it).get_burst_time())
            this->queue.insert(it++, program);
    }

}

//Round Robin
void RR_Scheduler::add_program(Program program)
{
    this->queue.push_back(program);

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

            Program new_program = Program(cpu_burst, priority, arrival_time);

            // Add the program to the queue list
            queue.push_back(new_program);

            // Set new latest arrival time
            last_program_arrival = arrival_time;
        }

        program_input_file.close();
    }

    else cout << "Unable to open file"; 
}

std::vector<Program> Program_Spawner::run_spawner()
{
    std::vector<Program> spawned_programs;

    for (int i = 0; i < queue.size(); i++)
    {
        // Add programs that have the same arrival time as the current spawner time
        if(queue[i].get_arrival_time() == time)
            spawned_programs.push_back(queue[i]);
    }

    return spawned_programs;
}

bool Program_Spawner::finish_spawning() 
{
    return time > last_program_arrival;
}

void Program_Spawner::set_time(int t) { time = t; }




double Scheduler_Report::calculate_avg_wait()
{
    double total = 0;
    for (std::list<Program>::iterator it = this->finished_programs.begin(); it != this->finished_programs.end(); ++it) 
    {
        total += it->get_wait_time();
    }

    return total / finished_programs.size();
}

double Scheduler_Report::calculate_avg_turn()
{
    double total = 0;
    for (std::list<Program>::iterator it = this->finished_programs.begin(); it != this->finished_programs.end(); ++it) 
    {
        total += it->get_turnaround_time();
    }

    return total / finished_programs.size();
}

void Scheduler_Report::print_program_summary()
{
    cout << "PID" << "     " << "WT" << "     " << "TT" << std::endl;

    // Sort the finished_programs list based on pid
    finished_programs.sort([](Program * lhs, Program * rhs) {return lhs->get_pid() < rhs->get_pid();});
    
    for (std::list<Program>::iterator it = this->finished_programs.begin(); it != this->finished_programs.end(); ++it) 
    {
        // Output the program id
        cout << " " << it->get_pid();
        if(it->get_pid() / 10 <= 0)
            cout << " ";


        cout << "   ";


        // Output the wait time
        if(it->get_wait_time() / 100 <= 0)
            cout << " ";
        cout << it->get_wait_time();
        if(it->get_wait_time() / 10 <= 0)
            cout << " ";

        
        cout << "   ";


        // Output the turnaround time
        if(it->get_wait_time() / 100 <= 0)
            cout << " ";
        cout << it->get_wait_time();
        if(it->get_wait_time()in_loading_state = true;

            // Take the first program from queue and load it into running_program
            running_program = &(queue.front());
            queue.pop_front(); / 10 <= 0)
            cout << " ";

        cout << std::endl;
    }

    // Output the averages of the wait and turnaround times
    cout << "AVG: " << 
}


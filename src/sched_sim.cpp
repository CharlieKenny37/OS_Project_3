#include <stdlib.h>
#include <time.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include "sched_sim.h"
#include <iomanip>
#include <memory>

using namespace std;


// Program Constructor
Program::Program(int pid, int cpu_burst, int priority, int arrival_time)
{
    this->pid = pid;
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
    if(cpu_burst > 0)
    {
        cpu_burst--;
        turnaround_time++;
    }
}

bool Program::finished()
{
    return cpu_burst == 0;
}

int Program::get_pid() { return pid; }

int Program::get_burst_time() { return cpu_burst; }

int Program::get_priority() { return priority; }

int Program::get_arrival_time() { return arrival_time; }

int Program::get_wait_time() { return wait_time; }

int Program::get_turnaround_time() { return turnaround_time; }
    


Scheduler::Scheduler(){};

// Scheduler base functions
bool Scheduler::is_stagnant()
{ 
    return queue.empty() && running_program == NULL;
}

std::shared_ptr<Program> Scheduler::load_program()
{
    in_loading_state = true;

    // save the finished or null program that was previously running
    std::shared_ptr<Program> previous_program = running_program;

    // Take the program from queue and load it into running_program
    if(!queue.empty())
    {
        running_program = std::shared_ptr<Program>(new Program(queue.front()));
        queue.pop_front();
    }
    else
    {
        running_program = NULL;
    }

    // Save the process order and context switches
    if(previous_program != NULL)
    {
        // Save process order
        finished_programs.process_order.push_back(previous_program->get_pid());
        // Increment context switches
        finished_programs.context_switches++;
    }

    // return previous program
    return previous_program;
}

void Scheduler::run()
{   
    int count = 0;
    // Check if there is a program running to start a load or manage a running program
    if(running_program == NULL)
    {
        // There is nothing happening in the scheduler, so prep to load a program
        if( !queue.empty() )
        {
            // Don't need return value since we know running program is null
            load_program();

            // Run current program
            running_program->run_cycle();
            this->currentProgTime++;
        }
    }
    else
    {
        // Check if the current running program has finished
        if(running_program->finished())
        {
            // Loads the next program and stores the finished program (CAN LOAD NULL PROGRAM)
            finished_programs.finished_programs.push_back( *(load_program()) );

            // Run current program
            if(running_program != NULL)
                running_program->run_cycle();
        }
        else if(this->finished_programs.sched_type.compare("STCF          ") == 0 && (this->running_program->get_burst_time() > queue.front().get_burst_time()) && !this->queue.empty()) {

            // this->queue.push_back(*(this->running_program));
            this->add_program(*(this->running_program));

            load_program();

            running_program->run_cycle();
        }
        else if(this->finished_programs.sched_type.compare("Round Robin   ") == 0) {
            if (this->currentProgTime < this->quantum) {
                // this->queue.push_back(*(this->running_program));
                running_program->run_cycle();
                this->currentProgTime++;
            }
            else {
                this->add_program(*(this->running_program));

                load_program();

                running_program->run_cycle();
                this->currentProgTime = 1;
            }
        }
        else
        {
            // run the running program
            running_program->run_cycle();
        }
    }

    // Call wait on all waiting processes
    for (std::list<Program>::iterator it = queue.begin(); it != queue.end(); it++) 
    {
        it -> wait();
    }
}

void Scheduler::document_status()
{
    // First add the time to the printout
    cout << "t = " << time << std::endl;


    // Next, there are 4 states the scheduler can be in: loading the first program, running a program,
    // finishing a program and loading the next program, and finishing the final program. 
    if( running_program == NULL && !queue.empty())
    {
        // loading the first program state
        cout << "CPU: Loading process " << queue.front().get_pid() << " (CPU burst = " << queue.front().get_burst_time() << ")" << endl;
    }

    if( running_program != NULL && running_program->finished() && !queue.empty())
    {
        // finishing a program and loading the next program state
        cout << "CPU: Finishing process " << running_program->get_pid() << "; loading process " << queue.front().get_pid() << " (CPU burst = " 
        << queue.front().get_burst_time() << ")" << endl;
    }
    else if( running_program != NULL && this->currentProgTime == this->quantum) {
        cout << "CPU: Preempting process " << running_program->get_pid() << " (remaining CPU burst = " << running_program->get_burst_time() << "); Loading process " << queue.front().get_pid() << " (CPU burst = " << queue.front().get_burst_time() << ")" << endl;
    }
    else if( running_program != NULL && this->running_program->get_burst_time() > queue.front().get_burst_time() && !this->queue.empty()) {
        cout << "CPU: Preempting process " << running_program->get_pid() << " (remaining CPU burst = " << running_program->get_burst_time() << "); Loading process " << queue.front().get_pid() << " (CPU burst = " << queue.front().get_burst_time() << ")" << endl;
    }
    else if( running_program != NULL && !running_program->finished())
    {
        // running a program state
        cout << "CPU: Running process " << running_program->get_pid() << " (remaining CPU burst = " << running_program->get_burst_time() << ")" << endl;
    }


    if( running_program != NULL && running_program->finished() && queue.empty())
    {
        // finishing the final program state
        cout << "CPU: Finishing process " << running_program->get_pid() << endl;
    }

    if( running_program == NULL && queue.empty())
    {
        // nothing state
        cout << "No Tasks to Run on CPU" << endl;
    }


    // Finally print out the ready queue
    cout << "Ready queue: ";

    if(queue.empty())
    {
        cout << "empty" << endl;
    }
    else
    {
        for (std::list<Program>::iterator it = queue.begin(); it != queue.end(); it++) 
        {
            cout << it->get_pid();
            if( std::distance(it, queue.end()) != 1)
                cout << "-";
        }

        cout << endl << endl;
    }
}

void Scheduler::add_program(Program program)
{
    queue.push_back(program);
}


Scheduler_Report Scheduler::get_scheduler_report()
{
    return finished_programs;
}

void Scheduler::set_time(int t) { time = t; }


//FCFS
FCFS_Scheduler::FCFS_Scheduler()
{
    this->quantum = 100;
    this->currentProgTime = 0;
    finished_programs = Scheduler_Report("FCFS          ");
}

void FCFS_Scheduler::add_program(Program program)
{
    
    queue.push_back(program);
}



//SJF
SJF_Scheduler::SJF_Scheduler()
{
    this->quantum = 100;
    this->currentProgTime = 0;
    finished_programs = Scheduler_Report("SJF           ");
}

void SJF_Scheduler::add_program(Program program)
{
    if(queue.empty())
    {
        queue.push_back(program);
    }
    else if(program.get_burst_time() < queue.front().get_burst_time()) {
            queue.push_front(program);
    }
    else if(program.get_burst_time() >= queue.back().get_burst_time()) {
            queue.push_back(program);
    }
    else
    {
        //iterates across list
        for (std::list<Program>::iterator it = queue.begin(); it != queue.end(); ++it) 
        {
            //if input burst time < current index burst_time, insert

            if(program.get_burst_time() < it->get_burst_time())
            {
                queue.insert(it, program);
                break;
            }
        }
    }
}

STCF_Scheduler::STCF_Scheduler()
{
    this->quantum = 100;
    this->currentProgTime = 0;
    this->preemting = false;
    finished_programs = Scheduler_Report("STCF          ");
}

//STCF  THIS SHOULD PREEMPT IF TIME < CURRENT PROGRAM TIME SOMEHOW
void STCF_Scheduler::add_program(Program program)
{
    if(queue.empty())
    {
        queue.push_back(program);
    }
    else if(program.get_burst_time() < queue.front().get_burst_time()) {
            queue.push_front(program);
    }
    else if(program.get_burst_time() >= queue.back().get_burst_time()) {
            queue.push_back(program);
    }
    else
    {
        //iterates across list
        for (std::list<Program>::iterator it = queue.begin(); it != queue.end(); ++it) 
        {
            //if input burst time < current index burst_time, insert

            if(program.get_burst_time() < it->get_burst_time())
            {
                queue.insert(it, program);
                break;
            }
        }
    }

}

//Round Robin
RR_Scheduler::RR_Scheduler(int quantum)
{
    this->quantum = quantum;
    this->currentProgTime = 0;
    this->preemting = false;

    finished_programs = Scheduler_Report("Round Robin   ");
}

void RR_Scheduler::add_program(Program program)
{
    this->queue.push_back(program);

}


// Priority
NPP_Scheduler::NPP_Scheduler()
{

    this->quantum = 100;
    this->currentProgTime = 0;
    finished_programs = Scheduler_Report("Priority      ");
}


void NPP_Scheduler::add_program(Program program)
{

    if(queue.empty())
    {
        queue.push_back(program);
    }
    else {
        //iterates accross list in reverse order
        for (std::list<Program>::iterator it = queue.begin(); it != queue.end(); ++it) 
            {
            //if input burst time >= current index burst_time, add behind current index
            if(program.get_priority() < it->get_priority())
            {
                queue.insert(it, program);
                break;
            }
            else if (it == --queue.end()) {
                queue.push_back(program);
                break;
            }
        }
    }
}


void Program_Spawner::read_program_file(std::string file_name)
{
    string line;
    ifstream program_input_file(file_name);
    int pid_def = 0;

    if (program_input_file.is_open())
    {
        while ( getline (program_input_file,line) )
        {
            std::vector<int> lineData;
            std::stringstream lineStream(line);

            int value;

            // Read an integer at a time from the line
            while(lineStream >> value)
            {
                // Add the integers from a line to a 1D array (vector)
                lineData.push_back(value);
            }

            Program new_program = Program(pid_def, lineData[0], lineData[1], lineData[2]);

            // Add the program to the queue list
            queue.push_back(new_program);

            // Set new latest arrival time
            last_program_arrival = lineData[0];

            pid_def++;
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
            spawned_programs.push_back(Program(queue[i]));
    }

    return spawned_programs;
}

bool Program_Spawner::finish_spawning() 
{
    return time > last_program_arrival;
}

void Program_Spawner::set_time(int t) { time = t; }



Scheduler_Report::Scheduler_Report(std::string type)
{
    sched_type = type;
    context_switches = 0;
}


double Scheduler_Report::calculate_avg_wait()
{
    double total = 0;
    for (std::list<Program>::iterator it = this->finished_programs.begin(); it != this->finished_programs.end(); ++it) 
    {
        total += it->get_wait_time();
    }

    this->avg_wait = total / finished_programs.size();
    return this->avg_wait;
}

double Scheduler_Report::calculate_avg_turn()
{
    double total = 0;
    for (std::list<Program>::iterator it = this->finished_programs.begin(); it != this->finished_programs.end(); ++it) 
    {
        total += it->get_turnaround_time();
    }

    this->avg_turn = total / finished_programs.size();
    return total / finished_programs.size();
}

void Scheduler_Report::print_program_summary()
{
    cout << "PID" << "     " << "WT" << "     " << "TT" << std::endl;

    // Sort the finished_programs list based on pid
    finished_programs.sort([](const Program &f, const Program &s) { return f.pid < s.pid; });
    
    for (std::list<Program>::iterator it = this->finished_programs.begin(); it != this->finished_programs.end(); ++it) 
    {
        // Output the program id
        cout << " " << it->get_pid();
        if(it->get_pid() / 10 <= 0)
            cout << " ";


        cout << "    ";


        // Output the wait time
        if(it->get_wait_time() / 100 <= 0)
            cout << " ";
        cout << it->get_wait_time();
        if(it->get_wait_time() / 10 <= 0)
            cout << " ";

        
        cout << "    ";


        // Output the turnaround time
        if(it->get_turnaround_time() / 100 <= 0)
            cout << " ";
        cout << it->get_turnaround_time();
        if(it->get_turnaround_time() / 10 <= 0)
            cout << " ";

        cout << std::endl;

    }
    // Output the averages of the wait and turnaround times
    cout << "AVG:  "; 
    
    if (this->calculate_avg_wait() / 10 <= 0)
        cout << " ";
    cout << std::fixed << std::setprecision(2) << this->calculate_avg_wait();

    cout << "  ";

    if (this->calculate_avg_turn() / 10 <= 0)
        cout << " ";
    cout << std::fixed << std::setprecision(2) << this->calculate_avg_turn() << std::endl << std::endl;


    cout << "Process sequence: ";
    for (std::list<int>::iterator it = this->process_order.begin(); it != this->process_order.end(); ++it) {
        cout << *(it);
        if( std::distance( it, process_order.end() ) != 1 )
            cout << "-";
    }

    cout << std::endl;

    cout << "Context switches: " << context_switches << endl << endl;
}
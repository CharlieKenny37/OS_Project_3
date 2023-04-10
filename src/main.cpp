
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sched_sim.h>

using namespace std;

int main(int argc, char *argv[])
{
    int report_spacing = 1;
    string input_file;
    string output_file;

    if(argc == 4) {
		input_file = argv[1];
        output_file = argv[2];
        report_spacing = atoi(argv[3]);
    }
    
    Program_Spawner program_spawner = Program_Spawner();
    program_spawner.read_program_file(input_file);
    
    FCFS_Scheduler fcfs_scheduler = FCFS_Scheduler();
    SJF_Scheduler sjf_scheduler = SJF_Scheduler();
    STCF_Scheduler stcf_scheduler = STCF_Scheduler();
    RR_Scheduler rr_scheduler = RR_Scheduler(2);
    NPP_Scheduler npp_scheduler = NPP_Scheduler();

    int time = 0;
    while(!program_spawner.finish_spawning() && !fcfs_scheduler.is_empty())
    {
        program_spawner.set_time(time);
        fcfs_scheduler.set_time(time);
    }

    time = 0;
    while(!program_spawner.finish_spawning() && !sjf_scheduler.is_empty())
    {
        program_spawner.set_time(time);
        fcfs_scheduler.set_time(time);
    }

    time = 0;
    while(!program_spawner.finish_spawning() && !stcf_scheduler.is_empty())
    {
        program_spawner.set_time(time);
        fcfs_scheduler.set_time(time);
    }

    time = 0;
    while(!program_spawner.finish_spawning() && !rr_scheduler.is_empty())
    {
        program_spawner.set_time(time);
        fcfs_scheduler.set_time(time);
    }

    time = 0;
    while(!program_spawner.finish_spawning() && !npp_scheduler.is_empty())
    {
        program_spawner.set_time(time);
        fcfs_scheduler.set_time(time);
    }
    
}




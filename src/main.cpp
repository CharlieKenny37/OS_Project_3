#include <iostream>
#include <fstream>
#include <string>
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

    // Redirect cout to the output file
    std::ofstream out(output_file);
    std::cout.rdbuf(out.rdbuf()); //redirect std::cout to output file
    
    Program_Spawner program_spawner = Program_Spawner();
    program_spawner.read_program_file(input_file);
    
    FCFS_Scheduler fcfs_scheduler = FCFS_Scheduler();
    SJF_Scheduler sjf_scheduler = SJF_Scheduler();
    STCF_Scheduler stcf_scheduler = STCF_Scheduler();
    RR_Scheduler rr_scheduler = RR_Scheduler(2);
    NPP_Scheduler npp_scheduler = NPP_Scheduler();

    std::vector<Scheduler_Report> finished_reports;

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




void final_output(std::vector<Scheduler_Report> reports) {

    cout << "***** OVERALL SUMMARY *****" << endl << "Wait Time Comparison" << endl;

    reports.sort([](Scheduler_Report * lhs, Scheduler_Report * rhs) {return lhs->calculate_avg_turn() < rhs->calculate_avg_turn();});

    int i = 1;
    for (std::list<Scheduler_Report>::iterator it = reports.begin(); it != reports.end(); ++it) {
        cout << i << " " << it->sched_type;
        if (it->calculate_avg_turn() / 10 <= 0)
            cout << " ";
        cout << std::fixed << std::setprecision(2) << it->calculate_avg_turn() << std::endl;
        i++;
    }
    cout << endl;


    cout << "Turnaround Time Comparison" << endl;

    reports.sort([](Scheduler_Report * lhs, Scheduler_Report * rhs) {return lhs->calculate_average_wait() < rhs->calculate_average_wait();});

    i = 1;
    for (std::list<Scheduler_Report>::iterator it = reports.begin(); it != reports.end(); ++it) {
        cout << i << " " << it->sched_type;
        if (it->calculate_avg_wait() / 10 <= 0)
            cout << " ";
        cout << std::fixed << std::setprecision(2) << it->calculate_avg_wait() << std::endl <<;
        i++;
    }
    cout << endl;

    
    cout << "Context Switch Comparison" << endl;

    reports.sort([](Scheduler_Report * lhs, Scheduler_Report * rhs) {return lhs->get_context_switch() < rhs->get_context_switch();});

    i = 1;
    for (std::list<Scheduler_Report>::iterator it = reports.begin(); it != reports.end(); ++it) {
        cout << i << " " << it->sched_type;
        if (it->calculate_avg_wait() / 10 <= 0)
            cout << " ";
        cout << std::fixed << std::setprecision(2) << it->get_context_switch() << std::endl;
        i++;
    }
    cout << endl;

}




#include <iostream>
#include <fstream>
#include <string>
#include "sched_sim.h"
#include <iomanip>
#include <list>

using namespace std;

void final_output(std::list<Scheduler_Report> reports) {

    for (std::list<Scheduler_Report>::iterator it = reports.begin(); it != reports.end(); ++it) {
        it->calculate_avg_turn();
        it->calculate_avg_wait();
    }

    cout << "***** OVERALL SUMMARY *****" << endl << "Wait Time Comparison" << endl;

    reports.sort([](const Scheduler_Report & f, const Scheduler_Report & s) { return f.avg_wait < s.avg_wait; });

    int i = 1;
    for (std::list<Scheduler_Report>::iterator it = reports.begin(); it != reports.end(); ++it) {
        cout << i << " " << it->sched_type;
        if (it->avg_wait / 10 <= 0)
            cout << " ";
        cout << std::fixed << std::setprecision(2) << it->avg_wait << std::endl;
        i++;
    }
    cout << endl;


    cout << "Turnaround Time Comparison" << endl;

    reports.sort([](const Scheduler_Report & f, const Scheduler_Report & s) { return f.avg_turn < s.avg_turn; });

    i = 1;
    for (std::list<Scheduler_Report>::iterator it = reports.begin(); it != reports.end(); ++it) {
        cout << i << " " << it->sched_type;
        if (it->avg_turn / 10 <= 0)
            cout << " ";
        cout << std::fixed << std::setprecision(2) << it->avg_turn << std::endl;
        i++;
    }
    cout << endl;

    
    cout << "Context Switch Comparison" << endl;

    reports.sort([](const Scheduler_Report & f, const Scheduler_Report & s) { return f.context_switches < s.context_switches; });

    i = 1;
    for (std::list<Scheduler_Report>::iterator it = reports.begin(); it != reports.end(); ++it) {
        cout << i << " " << it->sched_type;
        if (it->context_switches / 10 <= 0)
            cout << " ";
        cout << std::fixed << std::setprecision(2) << it->context_switches << std::endl;
        i++;
    }
    cout << endl;

}


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
    std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
    std::cout.rdbuf(out.rdbuf()); //redirect std::cout to output file
    
    Program_Spawner program_spawner = Program_Spawner();
    program_spawner.read_program_file(input_file);
    
    FCFS_Scheduler fcfs_scheduler = FCFS_Scheduler();
    SJF_Scheduler sjf_scheduler = SJF_Scheduler();
    STCF_Scheduler stcf_scheduler = STCF_Scheduler();
    RR_Scheduler rr_scheduler = RR_Scheduler(2);
    NPP_Scheduler npp_scheduler = NPP_Scheduler();

    std::list<Scheduler_Report> finished_reports;

    int time = 0;
    // Synchronize timing between objects
    fcfs_scheduler.set_time(time);
    program_spawner.set_time(time);

    cout << endl << "***** FCFS Scheduling *****" << std::endl;
    while(!program_spawner.finish_spawning() || !fcfs_scheduler.is_stagnant() )
    {
        // Enqueue any spawned program
        std::vector<Program> spawned_programs = program_spawner.run_spawner();
        for(int i = 0; i < spawned_programs.size(); i++)
            fcfs_scheduler.add_program(spawned_programs[i]);

        //Run printout on scheduler
        fcfs_scheduler.document_status();

        // Run scheduler transition
        fcfs_scheduler.run();

        // Increment time
        time++;

        // Synchronize timing between objects
        fcfs_scheduler.set_time(time);
        program_spawner.set_time(time);
    }

    cout << "*********************************************************" << endl;
    cout << "FCFS Summary (WT = wait time, TT = turnaround time):" << endl << endl;
    fcfs_scheduler.get_scheduler_report().print_program_summary();
    
    finished_reports.push_back(fcfs_scheduler.get_scheduler_report());

    time = 0;
    sjf_scheduler.set_time(time);
    program_spawner.set_time(time);

    cout << endl << "***** SJF Scheduling *****" << std::endl;
    while(!program_spawner.finish_spawning() || !sjf_scheduler.is_stagnant())
    {
        // Enqueue any spawned program
        std::vector<Program> spawned_programs = program_spawner.run_spawner();
        for(int i = 0; i < spawned_programs.size(); i++)
            sjf_scheduler.add_program(spawned_programs[i]);

        // Run printout on scheduler
        sjf_scheduler.document_status();

        // Run scheduler transition
        sjf_scheduler.run();

        // Increment time
        time++;

        // Synchronize timing between objects
        sjf_scheduler.set_time(time);
        program_spawner.set_time(time);
    }
    cout << "*********************************************************" << endl;
    cout << "SJF Summary (WT = wait time, TT = turnaround time):" << endl << endl;
    sjf_scheduler.get_scheduler_report().print_program_summary();
    
    finished_reports.push_back(sjf_scheduler.get_scheduler_report());


    time = 0;
    // Synchronize timing between objects
    stcf_scheduler.set_time(time);
    program_spawner.set_time(time);

    cout << endl << "***** STCF Scheduling *****" << std::endl;
    while(!program_spawner.finish_spawning() || !stcf_scheduler.is_stagnant())
    {
        // Enqueue any spawned program
        std::vector<Program> spawned_programs = program_spawner.run_spawner();
        for(int i = 0; i < spawned_programs.size(); i++)
            stcf_scheduler.add_program(spawned_programs[i]);

        // Run printout on scheduler
        stcf_scheduler.document_status();

        // Run scheduler transition
        stcf_scheduler.run();

        // Increment time
        time++;
        stcf_scheduler.set_time(time);
        program_spawner.set_time(time);

    }
    finished_reports.push_back(stcf_scheduler.get_scheduler_report());

    cout << "*********************************************************" << endl;
    cout << "STCF Summary (WT = wait time, TT = turnaround time):" << endl << endl;
    stcf_scheduler.get_scheduler_report().print_program_summary();

    time = 0;
    // Synchronize timing between objects
    rr_scheduler.set_time(time);
    program_spawner.set_time(time);
    cout << endl << "***** Round robin *****" << std::endl;
    while(!program_spawner.finish_spawning() || !rr_scheduler.is_stagnant())
    {


        // Enqueue any spawned program
        std::vector<Program> spawned_programs = program_spawner.run_spawner();
        for(int i = 0; i < spawned_programs.size(); i++)
            rr_scheduler.add_program(spawned_programs[i]);

        // Run printout on scheduler
        rr_scheduler.document_status();

        // Run scheduler transition
        rr_scheduler.run();

        // Increment time
        time++;

        // Synchronize timing between objects
        rr_scheduler.set_time(time);
        program_spawner.set_time(time);
    }
    finished_reports.push_back(rr_scheduler.get_scheduler_report());

    cout << "*********************************************************" << endl;
    cout << "RR Summary (WT = wait time, TT = turnaround time):" << endl << endl;
    rr_scheduler.get_scheduler_report().print_program_summary();



    time = 0;
    // Synchronize timing between objects
    npp_scheduler.set_time(time);
    program_spawner.set_time(time);
    cout << endl << "***** Priority Scheduling *****" << std::endl;
    while(!program_spawner.finish_spawning() || !npp_scheduler.is_stagnant())
    {


        // Enqueue any spawned program
        std::vector<Program> spawned_programs = program_spawner.run_spawner();
        for(int i = 0; i < spawned_programs.size(); i++)
            npp_scheduler.add_program(spawned_programs[i]);

        // Run printout on scheduler
        npp_scheduler.document_status();

        // Run scheduler transition
        npp_scheduler.run();

        // Increment time
        time++;

        // Synchronize timing between objects
        npp_scheduler.set_time(time);
        program_spawner.set_time(time);
    }
    finished_reports.push_back(npp_scheduler.get_scheduler_report());

    cout << "*********************************************************" << endl;
    cout << "NPP Summary (WT = wait time, TT = turnaround time):" << endl << endl;
    npp_scheduler.get_scheduler_report().print_program_summary();

    cout << endl;

    final_output(finished_reports);

    std::cout.rdbuf(coutbuf);   //reset to standard input again
}


#ifndef __sched_sim_h__
#define __sched_sim_h__

#include <list>
#include <vector>

class Program
{
    private:
        int cpu_burst;
        int priority;
        int arrival_time;
        int wait_time;
        int turnaround_time;

        // Used to create pids by assigning new programs sequential IDs starting at 0.
        static int program_counter;

    public:
        int pid;
        Program(int cpu_burst, int priority, int arrival_time);
        void wait();
        void run_cycle();
        int get_pid();
        int get_burst_time();
        int get_priority();
        int get_arrival_time();
        int get_wait_time();
        int get_turnaround_time();
        bool finished();
};

// Initialize the Program pid values to start at 0;
int Program::program_counter = 0;

class Scheduler_Report
{
    public:
        Scheduler_Report(std::string = "");
        std::list<Program> finished_programs;
        std::list<int> process_order;
        int context_switches;
        std::string sched_type;
        void print_program_summary();
        double avg_wait;
        double avg_turn;
        double calculate_avg_wait();
        double calculate_avg_turn();
};

class Scheduler
{
    protected:
        std::list<Program> queue;
        Program* running_program;
        Scheduler_Report finished_programs;

        int time;
        bool in_loading_state;

        Program* load_program();

    public:
        Scheduler(int time = 0);
        bool is_empty();
        void run();
        virtual void add_program(Program program);
        void set_time(int t);
        void document_status();
        Scheduler_Report get_scheduler_report(); 
        
};

class FCFS_Scheduler : public Scheduler
{
    public:
        FCFS_Scheduler();
        void add_program(Program program);
};

class SJF_Scheduler : public Scheduler
{
    public:
        SJF_Scheduler();
        void add_program(Program program);
};

class STCF_Scheduler : public Scheduler
{
    public:
        STCF_Scheduler();
        void add_program(Program program);
};

class RR_Scheduler : public Scheduler
{
    private:    
        int currentProgTime;
        int quantum;

    public:
        RR_Scheduler(int quantum);
        void add_program(Program program);
};

class NPP_Scheduler : public Scheduler
{
    public:
        NPP_Scheduler();
        void add_program(Program input);
};

class Program_Spawner
{
    private:
        std::vector<Program> queue;
        int time;
        int last_program_arrival;
    
    public:
        void read_program_file(std::string file_name);
        std::vector<Program> run_spawner();
        void set_time(int t);
        bool finish_spawning();
};

#endif // __sched_sim_h__

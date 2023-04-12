#ifndef __sched_sim_h__
#define __sched_sim_h__

#include <list>
#include <vector>
#include <memory>

class Program
{
    private:
        int cpu_burst;
        int priority;
        int arrival_time;
        int wait_time;
        int turnaround_time;

    public:
        int pid;
        Program(int pid, int cpu_burst, int priority, int arrival_time);
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
        std::shared_ptr<Program> running_program;
        Scheduler_Report finished_programs;

        int time;
        bool in_loading_state;
        bool preemting;

        std::shared_ptr<Program> load_program();

    public:
        int currentProgTime;
        int quantum;
        Scheduler();
        bool is_stagnant();
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

#ifndef __sched_sim_h__
#define __sched_sim_h__

#include <list>
#include <vector>

class Program
{
    private:
        int pid;
        int cpu_burst;
        int priority;
        int arrival_time;
        int wait_time;
        int turnaround_time;

        // Used to create pids by assigning new programs sequential IDs starting at 0.
        static int program_counter;

    public:
        Program(int cpu_burst, int priority, int arrival_time);
        void wait();
        void run_cycle();
        int get_burst_time();
        int get_priority();
        int get_arrival_time();
        int get_wait_time();
        int get_turnaround_time();
        bool finished();
};

// Initialize the Program pid values to start at 0;
int Program::program_counter = 0;


class Scheduler
{
    protected:
        std::list<Program> queue;
        Program running_program;

        int context_switches;
        int time;

        virtual void update_queue(Program input);
        void load_program();

    public:
        Scheduler(int time = 0);
        bool is_empty();
        Program run();
        void add_programs(std::list<Program> programs);
        void set_time(int t);
};

class FCFS_Scheduler : public Scheduler
{
    public:
        void update_queue(Program input);
};

class SJF_Scheduler : public Scheduler
{
    private:
        void update_queue(Program input);
};

class STCF_Scheduler : public Scheduler
{
    private:
        void update_queue(Program input);
};

class RR_Scheduler : public Scheduler
{
    private:    
        int currentProgTime;
        void update_queue(Program input);

    public:
        RR_Scheduler(int quantum);
};

class NPP_Scheduler : public Scheduler
{
    private:
        virtual void update_queue(Program input);
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

class Scheduler_Report
{
    public:
        std::list<Program> finished_programs;
};

#endif // __sched_sim_h__

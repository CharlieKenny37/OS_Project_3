#ifndef __sched_sim_h__
#define __sched_sim_h__

#include <list>

class Program
{
    private:
        int pid;
        int cpu_burst;
        int wait_time;
        int turnaround_time;

    public:
        Program(int pid, int cpu_burst = 0);
        void wait();
        void run_cycle();
        int get_burst_time();
        bool finished();
};

class Scheduler
{
    private:
        Program running_program;
        int context_switches;

        void load_program();

    public:
        int time;
        Scheduler(int time = 0);
        void is_empty();
        Program run();
        void add_programs(std::list<Program> programs);

    protected:
        std::list<Program> queue;
        virtual void update_queue(Program input);
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
        virtual void update_queue();
};

class Program_Spawner
{
    private:
        std::list<Program> queue;
    
    public:
        int time;
        void read_program_file(std::string file_name);
        std::list<Program> run_spawner();
};

class Scheduler_Report
{
    public:
        std::list<Program> finished_programs;
}

#endif // __sched_sim_h__

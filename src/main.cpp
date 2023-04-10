
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sched_sim.h>

using namespace std;

int main(int argc, char *argv[]) {
    int report_spacing = 1;
    string input_file;
    string output_file;

    if(argc == 4) {
		input_file = argv[1];
        output_file = argv[2];
        report_spacing = atoi(argv[3]);
    }
    

    
}




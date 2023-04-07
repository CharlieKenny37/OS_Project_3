
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
    

    // not sure if this works, tried making it a global variable so we could do extern in other file like it says in the assn
    pBuffer = new Buffer(size, num_prod, num_cons);

    for(int loop = 0; loop < num_prod; loop++) {
        //make thread
        t_p[loop].thread_id = loop;
        cout << "Main: started producer " << loop << "\n";
        Pthread_create(&p_p[loop], NULL, producer, &t_p[loop]);


    }

    for(int loop = 0; loop < num_cons; loop++) {
        // create consumers
        t_c[loop].thread_id = loop;
        cout << "Main: started consumer " << loop << "\n"
        Pthread_create(&p[loop], NULL, consumer, &t_c[loop]);

    }

    // wait for join threads
}




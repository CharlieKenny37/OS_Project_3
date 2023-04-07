#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <pthread.h>
#include <iostream>
#include <prod_cons_MT.h>

using namespace std;

//Buffer constructor
Buffer::Buffer(int num_prod, int num_cons, int size)
{
    srand(time(NULL));
    buffer_head = 0;
    buffer_tail = 0;
    buffer_size = size;
    buffer[size];
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&full_cv, NULL);
    pthread_cond_init(&empty_cv, NULL);
}

//Buffer consumer implement
void Buffer::consumer(int thread_number) {
    int randVal = rand() % (10 + 1);
    cout << "P:" << thread_number << ": Consuming " << randVal << " values\n";

    for(int i = 0; i < randVal; i++)
    {
        // Lock access to the circular buffer
        pthread_mutex_lock(&mutex);

        // Check if the queue is empty
        if(buffer_head == buffer_tail)
        {
            cout << "C:" << thread_number << ": Blocked due to empty buffer";

            // Wait until the queue is not empty
            while(buffer_head == buffer_tail)
            {
                pthread_cond_wait(&empty_cv, &mutex);
            }

            cout << "C:" << thread_number << ": Done waiting on empty buffer";
        }

        // Read the buffer value
        cout << "P:" << &thread_number << ": Reading  " << buffer[buffer_tail] << " from position " << buffer_tail << "\n";

        // Move the tail of the buffer
        buffer_tail++;
        if(buffer_tail == buffer_size) {
            buffer_tail = 0;
        }
                
        // Signal that the buffer is no longer full
        pthread_cond_signal(&full_cv);
    }
}

//Buffer producer implement
void Buffer::producer(int thread_number)
{
    int randVal = rand() % (10 + 1);
    cout << "P:" << thread_number << ": Producing " << randVal << " values\n";

    for(int i = 0; i < randVal; i++)
    {
        // Lock access to the circular buffer
        pthread_mutex_lock(&mutex);

        // Check if the queue is empty
        if( (buffer_head == buffer_tail - 1) || ( (buffer_head == buffer_size - 1) && (buffer_head == 0)) )
        {
            cout << "P:" << thread_number << ": Blocked due to full buffer";

            // Wait until the queue is not full
            while( (buffer_head == buffer_tail - 1) || ( (buffer_head == buffer_size - 1) && (buffer_head == 0)))
            {
                pthread_cond_wait(&full_cv, &mutex);
            }

            cout << "P:" << thread_number << ": Done waiting on full buffer";
        }

        int rand_in = rand() % (10 + 1);
        cout << "P:" << thread_number << ": Writing  " << rand_in << " to position " << buffer_head << "\n";
        buffer[buffer_head] = rand_in;

        // Move the head of the buffer
        buffer_head++;
        if(buffer_head == buffer_size) {
            buffer_head = 0;
        }
                
        // Unlock access to buffer
        pthread_mutex_lock(&mutex);
                
        // Signal that the buffer is no longer full
        pthread_cond_signal(&empty_cv);
    }
}

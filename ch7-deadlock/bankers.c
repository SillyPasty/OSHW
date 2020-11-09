#include <stdio.h>
#include <string.h>
#include <pthread.h>

#define NUMBER_OF_CUSTOMERS 5
#define NUMBER_OF_RESOURCES 3

int avaliable[NUMBER_OF_RESOURCES];
int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
pthread_mutex_t lock;

int is_safe();
int request_resources(int customer_num, int request[]);
int release_resources(int customer_num, int release[]);
void *customer(void *arg);
void init();

typedef struct
{
    int customer_num;
}thread_args;


int main()
{

    return 0;
}

void *customer(void *arg)
{
    // generate resources which less than need
    // request random resources
    // if success, release
    // else if failed request again
    // else wait
}

void init()
{
    // init avaliable
    // init maximum
    // init need
    // init allocate
    memcpy(maximum, need, sizeof(int) * (NUMBER_OF_CUSTOMERS * NUMBER_OF_RESOURCES));
}

int request_resources(int customer_num, int request[])
{
    pthread_mutex_lock(&lock);
    // request lock
    // check the limit
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
    {
        if (request[i] > need[customer_num][i])
        {
            // over the limit
            pthread_mutex_unlock(&lock);
            return -1;
        }
    }
    // check the avaliable
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
    {
        if (request[i] > avaliable[i])
        {
            pthread_mutex_unlock(&lock);
            return 1; // wait in thread
        }
    }
    // request the resources
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
    {
        avaliable[i] -= request[i];
        allocation[customer_num][i] += request[i];
        need[customer_num][i] -= request[i];
    }
    // check the safe state
    if (!is_safe)
    {
        // roll back if unsafe
        for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
        {
            avaliable[i] += request[i];
            allocation[customer_num][i] -= request[i];
            need[customer_num][i] += request[i];
        }
        pthread_mutex_unlock(&lock);
        return -1; // failed
    }
    pthread_mutex_unlock(&lock);
    return 0;
}

int release_resources(int customer_num, int release[])
{
    pthread_mutex_lock(&lock);
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
    {
        avaliable[i] += release[i];
        allocation[customer_num][i] -= release[i];
        need[customer_num][i] += release[i];
    }
    pthread_mutex_unlock(&lock);
}

int is_safe()
{
    int cnt = 0;
    int finish[NUMBER_OF_CUSTOMERS];
    int work[NUMBER_OF_RESOURCES];
    memcpy(avaliable, work, sizeof(int) * NUMBER_OF_RESOURCES);
    while (cnt < NUMBER_OF_CUSTOMERS)
    {
        int avi_customer = -1;
        for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
        {
            // find aviliable process
            int flag = 1;
            for (int j = 0; j < NUMBER_OF_RESOURCES; j++)
            {
                // check if its need is less than aviliable resources.
                if (work[i] < need[i][j])
                {
                    flag = 0;
                    break;
                }
            }
            if (flag && !finish[i])
            {
                avi_customer = i;
                break;
            }
        }
        if (avi_customer == -1) 
        {
            // no aviliable, unsave
            return 1;
        }
        for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
        {
            // release the resources (simulate)
            work[i] += allocation[avi_customer][i];
        }
        finish[avi_customer] = 1;
        cnt++;
    }
    return 0;
}

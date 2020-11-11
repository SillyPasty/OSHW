#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define NUMBER_OF_CUSTOMERS 5
#define NUMBER_OF_RESOURCES 3

int avaliable[NUMBER_OF_RESOURCES];
int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
pthread_t thread[NUMBER_OF_CUSTOMERS];
pthread_mutex_t lock;

int get_rand(int lower, int upper);
int is_safe();
int request_resources(int customer_num, int request[]);
int release_resources(int customer_num, int release[]);
void *customer(void *arg);
void init();

typedef struct
{
    int cid;
}t_arg;

int main(int argc, char **argv)
{
    srand((unsigned int)time(NULL));
    t_arg arg[NUMBER_OF_CUSTOMERS];
    // get input
    for (int i = 1; i < NUMBER_OF_RESOURCES + 1; i++)
    {
        avaliable[i - 1] = atoi(argv[i]);
    }
    init();
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
    {
        arg[i].cid = i;
        // printf("Start:%d", arg[1].cid);
        pthread_create(&(thread[i]), NULL, customer, &(arg[i]));
        // sleep(1000);
    }
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
    {
        pthread_join(thread[i], NULL);
    }
    return 0;
}

int get_rand(int lower, int upper)
{
    // printf("UPPER%d\n", upper);
    return rand() % upper + lower;
}

void *customer(void *arg)
{
    t_arg *args = (t_arg *)arg;
    int cid = args->cid;
    printf("Customer%d start.\n", cid);
    sleep(1);
    // generate resources which less than need
    int request[NUMBER_OF_RESOURCES];
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
    {
        request[i] = get_rand(0, need[cid][i] + 1);
    }
    while (1)
    {   
        printf("Customer %d: Want to request: %d, %d, %d\n", cid, request[0], request[1], request[2]);
        // request random resources
        int req;
        req = request_resources(cid, request);

        if (req == 0)
        {
            // success
            printf("Customer %d: Request successful!\n", cid);
            sleep(1);
            release_resources(cid, request);
            
        }
        else if (req == -1)
        {
            printf("Customer %d: Requset failed.\n", cid);
        }
        else
        {
            printf("Customer %d: Waiting.\n", cid);
        }
        
        if (req != 1)
        {
            // Success or failed perpare for next request.
            for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
            {
                request[i] = get_rand(0, need[cid][i] + 1);
            }
        }
        sleep(3);
    }
}

void init()
{

    // init maximum
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
    {
        for (int j = 0; j < NUMBER_OF_RESOURCES; j++)
        {
            maximum[i][j] = get_rand(0, avaliable[j]);
        }
    }
    // init need
    memcpy(need, maximum, sizeof(int) * (NUMBER_OF_CUSTOMERS * NUMBER_OF_RESOURCES));
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
    if (!is_safe())
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
    return 0;
}

int is_safe()
{
    int cnt = 0;
    int finish[NUMBER_OF_CUSTOMERS];
    int work[NUMBER_OF_RESOURCES];
    memcpy(work, avaliable, sizeof(int) * NUMBER_OF_RESOURCES);
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

#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


double get_rand();
void *runner(void *param);
double in_circle = 0, total = 0;
int times = 0;
// pi is 3.1415 when times = 500000000
int main(int argc, char *argv[])
{
    
    srand((unsigned int)time(NULL));
    do {
        pthread_t tid;
        pthread_attr_t attr;
        printf("Please input the times:");
        scanf("%d", &times);
        if (times == 0)
            break;
        pthread_attr_init(&attr);
        pthread_create(&tid, &attr, runner, NULL);
        pthread_join(tid, NULL);
        double pi = 4 * in_circle / total;
        printf("Pi is %lf\n", pi);
    } while (times != 0);
    return 0;
}

double get_rand() {
    return ((double)rand()/(double)(RAND_MAX)) * 2 - 1;
}

void *runner(void *param)
{
    while (times-- > 0) {
        double x = get_rand();
        double y = get_rand();
        double dist = sqrt(x*x+y*y);
        if (dist < 1)
            in_circle++;
        total++;
    }
    pthread_exit(0);
}
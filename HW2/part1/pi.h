#include <cstdlib>
#include <time.h>
#include <pthread.h>
#include <iostream>
using namespace std;

typedef struct
{
    int tid;
    int num_toss;
    long long int *hits;
    pthread_mutex_t *mutex_hits;
} Arg;

pthread_mutex_t mutex_hits;


void set_time_seed(){
    srandom((unsigned)time(NULL));
}

double get_random(double upper_bound, double lower_bound){
    const long max_rand = 100000L;
    return lower_bound
     + (upper_bound -lower_bound)
     * (random() % max_rand)
     / max_rand;
}

void *_Monte_Carlo_method(void *arg){
    Arg *data = (Arg *)arg;
    long long int tmp_hits=0;
    for(int toss=0; toss<data->num_toss ; toss++){
        double x, y;
        x = get_random(1.0, -1.0);
        y = get_random(1.0, -1.0);
        double dis_squared = x*x + y*y;
        if(dis_squared < 1) tmp_hits++;
    }
    pthread_mutex_lock(data->mutex_hits);
    *(data->hits) += tmp_hits;
    pthread_mutex_unlock(data->mutex_hits);
    // print out
    // printf("Thread %d : \n\tnum toss : %d \n\thits : %lld\n\n", data->tid, data->num_toss, tmp_hits);
    // exit
    pthread_exit((void*)0);
}

double Monte_Carlo_method(long long int nums_toss, int process_num){
    long long int *nums_in_cir=(long long int *)malloc(sizeof(long long int));
    pthread_t **threads;
    threads = (pthread_t**)malloc(sizeof(pthread_t*)*process_num);
    pthread_mutex_t *hits_lock = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
    pthread_attr_t *attr = (pthread_attr_t*)malloc(sizeof(pthread_attr_t));
    // set mutex lock for num_in_cir
    pthread_mutex_init(hits_lock, NULL);
    // set attr to joinable
    pthread_attr_init(attr);
    pthread_attr_setdetachstate(attr, PTHREAD_CREATE_JOINABLE);
    // init 
    *nums_in_cir = 0;
    // threads' arg
    Arg **arg_list = (Arg**)malloc(sizeof(Arg*)*process_num);

    long long int part = (nums_toss + (nums_toss%process_num)) / process_num;
    // 7 / 2 = 3
    set_time_seed();
    for(int i =0; i<process_num ;i++){
        threads[i] = (pthread_t*)malloc(sizeof(pthread_t));
        arg_list[i] = (Arg*)malloc(sizeof(Arg));
        arg_list[i]->hits = nums_in_cir;
        arg_list[i]->mutex_hits = hits_lock;
        arg_list[i]->tid = i;
        arg_list[i]->num_toss = (part*(i+1) > nums_toss) ? (nums_toss - (part*i)):part;
        pthread_create(threads[i], attr, _Monte_Carlo_method, (void *)arg_list[i]);
    }

    // recycle attr
    pthread_attr_destroy(attr);

    void *status;
    for(int i=0 ; i<process_num ; i++){
        pthread_join(*threads[i], &status);
    }

    pthread_mutex_destroy(hits_lock);

    // free
    for(int i=0 ; i<process_num ; i++){
        free(arg_list[i]);
        free(threads[i]);
    }
    free(arg_list);
    free(threads);
    free(attr);
    free(hits_lock);
    long long int final_hit = (*nums_in_cir);
    free(nums_in_cir);
    
    return 4 * final_hit / ((double) nums_toss);
}


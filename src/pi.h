#include <cstdlib>
#include <time.h>
#include <iostream>
using namespace std;

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
double Monte_Carlo_method(int nums_toss){
    double nums_in_cir=0.0;
    set_time_seed();
    for(int toss=0; toss<nums_toss ; toss++){
        double x, y;
        x = get_random(1.0, -1.0);
        y = get_random(1.0, -1.0);
        // cout<<"DEBUG: x= "<<x<<endl;
        double dis_squared = x*x + y*y;
        if(dis_squared < 1) nums_in_cir++;
    }
    return 4 * nums_in_cir / ((double) nums_toss);
}


#include <iostream>
#include "./pi.h"
using namespace std;
int main(int argc, char **argv){
    // cout<<"what is PI?"<<endl;
    // cout<<"toss = 1000?"<<endl;
    // cout<<Monte_Carlo_method(1000)<<endl;
    // cout<<"toss = 1000000?"<<endl;
    // cout<<Monte_Carlo_method(1000000)<<endl;
    // cout<<"toss = 1000000000?"<<endl;
    cout<<"Pi is "<<Monte_Carlo_method(atoll(argv[2]), atoi(argv[1]))<<" by Monte Carlo method."<<endl;
    return 0;
}
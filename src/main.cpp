#include <iostream>
#include "./pi.h"
using namespace std;
int main(){
    cout<<"what is PI?"<<endl;
    // cout<<"toss = 1000?"<<endl;
    // cout<<Monte_Carlo_method(1000)<<endl;
    // cout<<"toss = 1000000?"<<endl;
    // cout<<Monte_Carlo_method(1000000)<<endl;
    cout<<"toss = 1000000000?"<<endl;
    cout<<Monte_Carlo_method(1000000000)<<endl;
    
    return 0;
}
#include <stdio.h>
#include <iostream>

using namespace std;

int add_integer(int a , int b);
float add_float(float a, float b);
int run();

int add_integer(int a , int b){
    return a + b;
}

float add_float(float a, float b){
    return a + b;
}

int run(){
    while(1){
        printf("hello, world\n");
    };
    return 1;
}

extern "C"{
    int add_integer_plus(int a , int b){
        return add_integer(a, b);
    }

    float add_float_plus(float a, float b){
        return add_float(a ,b);
    }

    int run_plus(){
        int sum = add_float(3, 4);
        return sum;
    }
}


/* main function will not be run */
int main(){
    printf("hello\n");
}


#include <stdio.h>
#include <iostream>

using namespace std;

int add_integer(int a , int b);
float add_float(float a, float b);

int add_integer(int a , int b){
    return a + b;
}

float add_float(float a, float b){
    return a + b;
}

extern "C"{
    int add_integer_plus(int a , int b){
        return add_integer(a, b);
    }

    float add_float_plus(float a, float b){
        return add_float(a ,b);
    }
}


/* main function will not be run */
int main(){
    printf("hello\n");
}


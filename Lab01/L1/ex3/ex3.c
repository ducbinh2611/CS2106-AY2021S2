/*************************************
* Lab 1 Exercise 3
* Name     : Ngo Duc Binh
* Matric No: A0180061U
* Lab Group: B01
*************************************/

#include <stdio.h>

//Datatype Declarations
typedef int (*arithFuncPtr)(int, int);


//Function Prototypes
int sum(int x, int y);
int gcd(int x, int y);
int exponential(int x, int y);
int removeFactor(int x, int y);

int main()
{
    int a, b, optype, res;
    arithFuncPtr functions[4] = {sum, gcd, removeFactor, exponential};
    arithFuncPtr ptr;
    char *spellNames[4] = {"lumos", "alohomora", "expelliarmus", "sonorus"};

    while (scanf("%i", &optype) == 1 && scanf("%i", &a) == 1 && scanf("%i", &b) == 1) {        
        ptr = functions[optype - 1];
        res = (*ptr)(a, b);
        printf("%s %i\n", spellNames[optype - 1], res);
    }
    //The code below is to show the usage of a function pointer only
    //You can remove/comment them off 

    //arithFuncPtr is a function pointer datatype
    //   - it points to afunction that takes (int, int) as parameter
    //   - and returns int as return result

    //arithFuncPtr ptr;

    //ptr points to the function add
    //ptr = add;

    //scanf("%i %i", &a, &b);

    //Defereference a function pointer === call the function
    //if you take (*ptr) as some function f
    // then below is the same as
    //   res = f(a, b);

    //res = (*ptr)(a, b);

    //printf("%i\n", res);
   
    return 0;
}

int sum(int x, int y) {
    int result = 0;
    for (int i = x; i <= y; i++) {
        result += i;
    }
    return result;
}

int gcd(int x, int y) {
    if (x == 0) {
        return y;
    } 
    if (y == 0) {
        return x;
    } 
    if (x == y) {
        return x;
    }
    if (x > y) {
        return gcd(y , x % y);    
    } else {
        return gcd(y % x , x);
    }
}

int exponential(int x, int y) {
    if (y == 0) {
        return 1;
    } else {
        return x * exponential(x, y - 1);
    }
}

int removeFactor(int x, int y) {
    if (x % y != 0) {
        return x;
    } else {
        return removeFactor(x / y, y);
    }
}
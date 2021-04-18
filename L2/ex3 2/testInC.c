#include <stdio.h>

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int main() {
    int a = 10;
    int b = 20;
    
    swap(&a, &b);
    printf("a is: %i\n" , a);
    printf("b is: %i\n" , b);
    return 0;
}




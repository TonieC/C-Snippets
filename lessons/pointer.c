// Code originally made by Tonie 
// Used for lessons on pointers in C
// This code demonstrates basic pointer usage in C

// libraries
#include <stdio.h>

int main() {
    int x = 10;     // normal integer variable
    int *p;         // pointer variable

    p = &x;         // store the address of x in pointer p
 // & is the address-of operator
 // * is the dereference operator
 // p now points to x
 // p holds the address of x
 // *p can be used to access the value of x
    printf("Value of x: %d\n", x);
    printf("Address of x: %p\n", &x);
    printf("Pointer p holds address: %p\n", p);
    printf("Value pointed by p: %d\n", *p); // dereference pointer
// Output the value of x using the pointer
    return 0;
}

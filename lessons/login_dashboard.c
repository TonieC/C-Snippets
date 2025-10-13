// Code originally made by Tonie 
// Used for lessons on string functions in C
// This code demonstrates a simple login system using string functions

// libraries
#include <stdio.h>
#include <string.h>

int main() {
    char username[20], password[20]; // variables to store user input
    char correctUsername[] = "admin"; // correct username
    char correctPassword[] = "1234";  // correct password

    printf("=== LOGIN DASHBOARD ===\n");

    // get username input
    printf("Enter username: ");
    fgets(username, sizeof(username), stdin); // reads string input
    username[strcspn(username, "\n")] = '\0'; // remove newline character

    // get password input
    printf("Enter password: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = '\0'; // remove newline character

    // compare entered username and password with correct ones
    // strcmp() returns 0 if both strings are equal
    if (strcmp(username, correctUsername) == 0 && strcmp(password, correctPassword) == 0) {
        printf("\nLogin successful! Welcome to the dashboard.\n");
    } else {
        printf("\nInvalid username or password.\n");
    }

    return 0;
}

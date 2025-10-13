// Code originally made by Tonie
// Used for lessons on functions, if-else, switch, and void in C
// This code demonstrates how to use multiple void functions for a menu system
// with main menu (if-else) and submenus (switch) including a playGame() function

// libraries
#include <stdio.h>
#include <stdlib.h>

// function prototypes
void easy();
void medium();
void hard();
void playGame(char mode[]); // takes the mode as an argument

// EASY MODE FUNCTION
void easy() {
    int choice;

    printf("\n**** EASY MODE ****\n");
    printf("1. Play a Game\n");
    printf("2. View Score\n");
    printf("3. Back to Main Menu\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            playGame("Easy"); // calls playGame() with "Easy"
            break;
        case 2:
            printf("\nViewing Easy Mode scores...\n");
            break;
        case 3:
            printf("\nReturning to Main Menu...\n");
            break;
        default:
            printf("\nInvalid choice in Easy Mode.\n");
            break;
    }
}

// MEDIUM MODE FUNCTION
void medium() {
    int choice;

    printf("\n**** MEDIUM MODE ****\n");
    printf("1. Play a Game\n");
    printf("2. View Score\n");
    printf("3. Back to Main Menu\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            playGame("Medium"); // calls playGame() with "Medium"
            break;
        case 2:
            printf("\nViewing Medium Mode scores...\n");
            break;
        case 3:
            printf("\nReturning to Main Menu...\n");
            break;
        default:
            printf("\nInvalid choice in Medium Mode.\n");
            break;
    }
}

// HARD MODE FUNCTION
void hard() {
    int choice;

    printf("\n**** HARD MODE ****\n");
    printf("1. Play a Game\n");
    printf("2. View Score\n");
    printf("3. Back to Main Menu\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            playGame("Hard"); // calls playGame() with "Hard"
            break;
        case 2:
            printf("\nViewing Hard Mode scores...\n");
            break;
        case 3:
            printf("\nReturning to Main Menu...\n");
            break;
        default:
            printf("\nInvalid choice in Hard Mode.\n");
            break;
    }
}

// PLAY GAME FUNCTION
// This function performs the "game" logic (simulated here with text)
void playGame(char mode[]) {
    printf("\n=== Playing the Game in %s Mode ===\n", mode);
    printf("Game is running...\n");
    printf("Congratulations! You finished the %s Mode game!\n", mode);
}

// MAIN FUNCTION
int main() {
    int mainChoice;

    printf("=== MAIN MENU ===\n");
    printf("1. Easy\n");
    printf("2. Medium\n");
    printf("3. Hard\n");
    printf("4. Exit\n");
    printf("Enter your choice: ");
    scanf("%d", &mainChoice);

    // Use if-else for main menu
    if (mainChoice == 1) {
        easy();
    } else if (mainChoice == 2) {
        medium();
    } else if (mainChoice == 3) {
        hard();
    } else if (mainChoice == 4) {
        printf("\nExiting program...\n");
        exit(0);
    } else {
        printf("\nInvalid choice in Main Menu.\n");
    }

    return 0;
}

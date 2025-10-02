#include <stdio.h>

int main() {
    int lives = 3;
    int answer;
    while (lives > 0) {
        printf("Lives: %d\n", lives);
        printf("What is 2x2? \n");
        printf("Your answer: ");
        scanf("%d", &answer);
        if (answer == 4) {
            printf("\nYou Win!\n");
            break;
        } else {
            lives--;
            printf("Wrong! You have %d lives left.\n\n", lives);
        }
    }
    if (lives <= 0) {
        printf("Sorry, You lose!\n");
    }
    return 0;
}

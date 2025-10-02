#include <stdio.h>

int main() {
    int lives = 3;
    int maxX, maxY, answer;
    printf("Enter the maximum value for x: ");
    scanf("%d", &maxX);
    printf("Enter the maximum value for y: ");
    scanf("%d", &maxY);
    while (lives > 0) {
        int num1 = (rand() % maxX) + 1; 
        int num2 = (rand() % maxY) + 1;
        printf("Lives: %d\n", lives);
        printf("What is %d x %d? \n", num1, num2);
        printf("Your answer: ");
        scanf("%d", &answer);
        if (answer == num1 * num2) {
            printf("\nCorrect!\n\n");
        } else {
            lives--;
            printf("Wrong! The correct answer is %d. You have %d lives left.\n\n", num1 * num2, lives);
        }
    }
    if (lives <= 0) {
        printf("Sorry, You lose!\n");
    }
    return 0;
}

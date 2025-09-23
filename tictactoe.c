#include <stdio.h>
#include <stdlib.h>

// Function prototypes
void printBoard(char board[3][3]);
int checkWinner(char board[3][3]);
void playerMove(char board[3][3], char player);

int main() {
    char board[3][3] = { {'1','2','3'}, {'4','5','6'}, {'7','8','9'} };
    char currentPlayer = 'X';
    int winner = 0;

    while (winner == 0) {
        printBoard(board);
        playerMove(board, currentPlayer);

        winner = checkWinner(board);

        // Switch player
        currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
    }

    printBoard(board);

    if (winner == 1) {
        printf("Player X wins!\n");
    } else if (winner == 2) {
        printf("Player O wins!\n");
    } else {
        printf("It's a draw!\n");
    }

    return 0;
}

void printBoard(char board[3][3]) {
    printf("\n");
    for(int i = 0; i < 3; i++) {
        printf(" %c | %c | %c \n", board[i][0], board[i][1], board[i][2]);
        if(i < 2) printf("---|---|---\n");
    }
    printf("\n");
}

void playerMove(char board[3][3], char player) {
    int choice;
    int valid = 0;

    while(!valid) {
        printf("Player %c, enter a number (1-9): ", player);
        scanf("%d", &choice);

        int row = (choice - 1) / 3;
        int col = (choice - 1) % 3;

        if(choice >= 1 && choice <= 9 && board[row][col] != 'X' && board[row][col] != 'O') {
            board[row][col] = player;
            valid = 1;
        } else {
            printf("Invalid move! Try again.\n");
        }
    }
}

int checkWinner(char board[3][3]) {
    // Check rows and columns
    for(int i = 0; i < 3; i++) {
        if(board[i][0]==board[i][1] && board[i][1]==board[i][2]) {
            return (board[i][0]=='X') ? 1 : 2;
        }
        if(board[0][i]==board[1][i] && board[1][i]==board[2][i]) {
            return (board[0][i]=='X') ? 1 : 2;
        }
    }

    // Check diagonals
    if(board[0][0]==board[1][1] && board[1][1]==board[2][2]) {
        return (board[0][0]=='X') ? 1 : 2;
    }
    if(board[0][2]==board[1][1] && board[1][1]==board[2][0]) {
        return (board[0][2]=='X') ? 1 : 2;
    }

    // Check for draw
    int draw = 1;
    for(int i=0;i<3;i++) {
        for(int j=0;j<3;j++) {
            if(board[i][j] != 'X' && board[i][j] != 'O') {
                draw = 0;
            }
        }
    }
    if(draw) return 3;

    return 0; // Game continues
}

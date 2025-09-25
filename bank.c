/*
===============================================================================
   _____              _      _             
  | ____|_  ___ __   (_) ___| |__  _   _   
  |  _| \ \/ / '_ \  | |/ __| '_ \| | | |  
  | |___ >  <| |_) | | | (__| | | | |_| |  
  |_____/_/\_\ .__/  |_|\___|_| |_|\__,_|  
             |_|                           

  Advanced Simple Bank System (file-based)
  Author / Watermark: Tonie
  ------------------------------------------------------------
  This file: bank.c
  Purpose:  A simple but advanced-enough console banking system in C
            - multiple accounts
            - register, login (PIN), deposit, withdraw, check balance
            - persistent storage using a plain text file (accounts.txt)
            - lots of inline comments explaining every part (because you asked)
  Compile:  gcc bank.c -o bank
  Run:      ./bank      (on Windows use bank.exe)
  Data file created/used: accounts.txt (same folder)
  License:  MIT (see LICENSE file)
  ------------------------------------------------------------
  NOTE: This is educational/demo level. Do NOT use for real money/accounts.
===============================================================================
*/

/* Standard headers */
#include <stdio.h>   /* printf, scanf, fopen, fprintf, fscanf, fclose */
#include <stdlib.h>  /* atoi, exit, malloc, free (not necessarily used but standard) */
#include <string.h>  /* strcmp, strncpy, strlen */

/* ---------------------------
   Configuration / constants
   --------------------------- */
#define MAX_ACCOUNTS 100        /* maximum number of accounts supported in-memory */
#define MAX_NAME_LEN 50         /* maximum length for account holder's name */
#define DATA_FILE "accounts.txt"/* file that stores accounts (simple text format) */

/* ---------------------------
   Data structures
   --------------------------- */

/* Account structure:
   - name: user-friendly identifier (no spaces handled in this simple impl)
   - pin: 4-digit integer PIN for authentication (not hashed — for demo only)
   - balance: double to hold money amounts (floating point for simplicity)
*/
typedef struct {
    char name[MAX_NAME_LEN];
    int pin;
    double balance;
} Account;

/* Global in-memory store and counter.
   In a larger program you would hide this inside a module instead of globals.
*/
Account accounts[MAX_ACCOUNTS];
int account_count = 0;

/* ---------------------------
   Utility / helper functions
   --------------------------- */

/* trim_newline:
   Remove trailing newline from strings coming from fgets (if used).
   Not strictly necessary here because we use scanf for simple tokens,
   but kept for completeness.
*/
void trim_newline(char *s) {
    size_t n = strlen(s);
    if (n == 0) return;
    if (s[n-1] == '\n') s[n-1] = '\0';
}

/* loadAccounts:
   Read accounts from DATA_FILE into the global accounts array.
   The simple file format used:
      name pin balance
   separated by whitespace, one account per line.
   - If the file does not exist, function returns quietly (first run).
   - If the file exists but contains invalid lines, those lines are skipped.
*/
void loadAccounts() {
    FILE *file = fopen(DATA_FILE, "r");
    if (file == NULL) {
        /* No data file yet — that's fine on first run */
        return;
    }

    /* Reset account_count just in case */
    account_count = 0;

    /* We read until EOF or until we hit MAX_ACCOUNTS */
    while (account_count < MAX_ACCOUNTS) {
        char name_buf[MAX_NAME_LEN];
        int pin;
        double bal;

        /* fscanf returns number of items successfully read */
        int scanned = fscanf(file, "%49s %d %lf", name_buf, &pin, &bal);
        if (scanned == 3) {
            /* Valid entry — copy into accounts array */
            strncpy(accounts[account_count].name, name_buf, MAX_NAME_LEN-1);
            accounts[account_count].name[MAX_NAME_LEN-1] = '\0';
            accounts[account_count].pin = pin;
            accounts[account_count].balance = bal;
            account_count++;
        } else {
            /* If we reached EOF, break; otherwise try to skip invalid token */
            if (scanned == EOF) break;
            /* Attempt to recover by consuming the rest of the line */
            int c;
            while ((c = fgetc(file)) != '\n' && c != EOF) {}
        }
    }

    fclose(file);
}

/* saveAccounts:
   Write the current accounts array back to DATA_FILE (overwrite).
   Format mirrors loadAccounts: name pin balance
   We save with 2 decimal places for balance.
*/
void saveAccounts() {
    FILE *file = fopen(DATA_FILE, "w");
    if (file == NULL) {
        printf("Error: cannot write to data file '%s'. Changes not saved.\n", DATA_FILE);
        return;
    }

    for (int i = 0; i < account_count; i++) {
        fprintf(file, "%s %d %.2f\n",
                accounts[i].name,
                accounts[i].pin,
                accounts[i].balance);
    }

    fclose(file);
}

/* findAccountByPin:
   Return the index in accounts[] for the given pin, or -1 if not found.
   NOTE: This uses PIN only for lookup (simple), so PINs must be unique.
*/
int findAccountByPin(int pin) {
    for (int i = 0; i < account_count; i++) {
        if (accounts[i].pin == pin) return i;
    }
    return -1;
}

/* isPinUnique:
   Check whether a given PIN already exists in the accounts store.
   Important to avoid collisions when registering new accounts.
*/
int isPinUnique(int pin) {
    return findAccountByPin(pin) == -1;
}

/* readInt:
   Simple helper for safer integer reads. This function uses scanf but
   consumes invalid input to avoid infinite loops on bad input.
   Returns 1 if an int was read into out, otherwise 0.
*/
int readInt(int *out) {
    if (scanf("%d", out) == 1) {
        return 1;
    } else {
        /* clear bad input */
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {}
        return 0;
    }
}

/* readDouble:
   Similar helper for reading double values safely.
*/
int readDouble(double *out) {
    if (scanf("%lf", out) == 1) {
        return 1;
    } else {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {}
        return 0;
    }
}

/* ---------------------------
   Feature functions
   --------------------------- */

/* registerAccount:
   - Prompts user for name and a 4-digit PIN.
   - Ensures PIN uniqueness (simple check).
   - Adds the account to memory and saves to file.
   - We keep name input as a single token (no spaces). You can change to fgets if you want spaces.
*/
void registerAccount() {
    if (account_count >= MAX_ACCOUNTS) {
        printf("Bank is full (max %d accounts). Cannot register more.\n", MAX_ACCOUNTS);
        return;
    }

    Account newAcc;
    memset(&newAcc, 0, sizeof(newAcc));

    printf("Enter your name (no spaces, max %d chars): ", MAX_NAME_LEN-1);
    /* scanf with %49s reads up to 49 chars and stops at whitespace */
    if (scanf("%49s", newAcc.name) != 1) {
        printf("Invalid name input. Registration cancelled.\n");
        return;
    }

    /* PIN input and validation loop */
    int pin_ok = 0;
    while (!pin_ok) {
        int pin;
        printf("Set a 4-digit PIN (1000-9999): ");
        if (!readInt(&pin)) {
            printf("Invalid input. Please enter numbers only.\n");
            continue;
        }
        if (pin < 1000 || pin > 9999) {
            printf("PIN must be a 4-digit number between 1000 and 9999.\n");
            continue;
        }
        if (!isPinUnique(pin)) {
            printf("PIN already in use. Choose a different PIN.\n");
            continue;
        }
        newAcc.pin = pin;
        pin_ok = 1;
    }

    newAcc.balance = 0.0; /* new accounts start at zero balance */

    /* Add to in-memory store and persist */
    accounts[account_count++] = newAcc;
    saveAccounts();

    printf("Account registered successfully! Welcome, %s.\n", newAcc.name);
}

/* bankingMenu:
   Present banking options for the logged-in account (index).
   Supports check balance, deposit, withdraw, and logout.
   After each state-changing operation we call saveAccounts() to persist changes.
*/
void bankingMenu(int index) {
    if (index < 0 || index >= account_count) {
        printf("Internal error: invalid account index.\n");
        return;
    }

    int choice = 0;
    do {
        printf("\n==== Banking Menu for %s ====\n", accounts[index].name);
        printf("1. Check Balance\n");
        printf("2. Deposit Money\n");
        printf("3. Withdraw Money\n");
        printf("4. Logout\n");
        printf("Choose (1-4): ");

        if (!readInt(&choice)) {
            printf("Invalid choice. Try again.\n");
            continue;
        }

        if (choice == 1) {
            printf("Current Balance: ₱%.2f\n", accounts[index].balance);
        } else if (choice == 2) {
            double amount;
            printf("Enter deposit amount (positive number): ");
            if (!readDouble(&amount)) {
                printf("Invalid amount.\n");
                continue;
            }
            if (amount <= 0) {
                printf("Deposit must be positive.\n");
                continue;
            }
            accounts[index].balance += amount;
            saveAccounts();
            printf("Successfully deposited ₱%.2f. New balance: ₱%.2f\n", amount, accounts[index].balance);
        } else if (choice == 3) {
            double amount;
            printf("Enter withdrawal amount: ");
            if (!readDouble(&amount)) {
                printf("Invalid amount.\n");
                continue;
            }
            if (amount <= 0) {
                printf("Withdrawal must be positive.\n");
                continue;
            }
            if (amount > accounts[index].balance) {
                printf("Insufficient balance. Your balance: ₱%.2f\n", accounts[index].balance);
                continue;
            }
            accounts[index].balance -= amount;
            saveAccounts();
            printf("Successfully withdrew ₱%.2f. New balance: ₱%.2f\n", amount, accounts[index].balance);
        } else if (choice == 4) {
            printf("Logging out %s...\n", accounts[index].name);
        } else {
            printf("Invalid option. Enter a number 1-4.\n");
        }

    } while (choice != 4);
}

/* ---------------------------
   Main loop
   --------------------------- */

int main() {
    /* Print watermark and header every run (console banner) */
    printf("============================================================\n");
    printf("  Advanced Simple Bank System — file-based, demo only\n");
    printf("  Watermark: Tonie\n");
    printf("  License: MIT (see LICENSE file)\n");
    printf("============================================================\n");

    /* Load accounts from disk into memory */
    loadAccounts();

    /* Main menu loop */
    while (1) {
        printf("\n==== Main Menu ====\n");
        printf("1. Register New Account\n");
        printf("2. Login (by PIN)\n");
        printf("3. Exit\n");
        printf("Choose (1-3): ");

        int choice;
        if (!readInt(&choice)) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }

        if (choice == 1) {
            registerAccount();
        } else if (choice == 2) {
            int pin;
            printf("Enter your 4-digit PIN: ");
            if (!readInt(&pin)) {
                printf("Invalid input. Returning to main menu.\n");
                continue;
            }
            int idx = findAccountByPin(pin);
            if (idx == -1) {
                printf("Invalid PIN. No matching account.\n");
                continue;
            }
            /* Successful login */
            bankingMenu(idx);
        } else if (choice == 3) {
            printf("Goodbye — thanks for using the demo bank. (Watermark: Tonie)\n");
            break;
        } else {
            printf("Invalid choice. Enter 1, 2, or 3.\n");
        }
    }

    /* Exiting — final save just in case (though operations already saved) */
    saveAccounts();
    return 0;
}

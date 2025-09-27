#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// --- Input Validation Helpers ---
int isValidBinary(const char *s) {
    while (*s) {
        if (*s != '0' && *s != '1') return 0;
        s++;
    }
    return 1;
}

int isValidOctal(const char *s) {
    while (*s) {
        if (*s < '0' || *s > '7') return 0;
        s++;
    }
    return 1;
}

int isValidHex(const char *s) {
    while (*s) {
        if (!isxdigit(*s)) return 0;
        s++;
    }
    return 1;
}

// --- Utility Functions ---

// Convert Decimal -> Binary string
void decToBin(unsigned long long dec, char *bin) {
    char temp[65];
    int i = 0;

    if (dec == 0) {
        strcpy(bin, "0");
        return;
    }
    while (dec > 0) {
        temp[i++] = (dec % 2) + '0';
        dec /= 2;
    }
    temp[i] = '\0';

    for (int j = 0; j < i; j++) {
        bin[j] = temp[i - j - 1];
    }
    bin[i] = '\0';
}

// Decimal -> Octal
void decToOct(unsigned long long dec, char *out) {
    sprintf(out, "%llo", dec);
}

// Decimal -> Hex
void decToHex(unsigned long long dec, char *out) {
    sprintf(out, "%llX", dec);
}

// Decimal -> Octal (3-bit binary groups)
void decToOctBits(unsigned long long dec, char *out) {
    char temp[256];
    int i = 0;

    if (dec == 0) {
        strcpy(out, "000");
        return;
    }

    while (dec > 0) {
        int digit = dec % 8;
        const char *bits[] = {"000","001","010","011","100","101","110","111"};
        for (int k = 0; k < 3; k++) temp[i++] = bits[digit][k];
        dec /= 8;
    }
    temp[i] = '\0';

    // reverse by groups of 3
    int len = i, idx = 0;
    for (int j = len - 3; j >= 0; j -= 3) {
        out[idx++] = temp[j];
        out[idx++] = temp[j + 1];
        out[idx++] = temp[j + 2];
        if (j > 0) out[idx++] = ' ';
    }
    out[idx] = '\0';
}

// Decimal -> Hex (4-bit binary groups)
void decToHexBits(unsigned long long dec, char *out) {
    char temp[256];
    int i = 0;

    if (dec == 0) {
        strcpy(out, "0000");
        return;
    }

    while (dec > 0) {
        int digit = dec % 16;
        const char *bits[] = {
            "0000","0001","0010","0011","0100","0101","0110","0111",
            "1000","1001","1010","1011","1100","1101","1110","1111"
        };
        for (int k = 0; k < 4; k++) temp[i++] = bits[digit][k];
        dec /= 16;
    }
    temp[i] = '\0';

    // reverse by groups of 4
    int len = i, idx = 0;
    for (int j = len - 4; j >= 0; j -= 4) {
        out[idx++] = temp[j];
        out[idx++] = temp[j + 1];
        out[idx++] = temp[j + 2];
        out[idx++] = temp[j + 3];
        if (j > 0) out[idx++] = ' ';
    }
    out[idx] = '\0';
}

// Binary -> Decimal
unsigned long long binToDec(const char *bin) {
    unsigned long long dec = 0;
    while (*bin) {
        dec = dec * 2 + (*bin - '0');
        bin++;
    }
    return dec;
}

// Print results
void printAll(long long dec) {
    char bin[65], octNum[65], hexNum[65], octBits[256], hexBits[256];
    decToBin(dec, bin);
    decToOct(dec, octNum);
    decToHex(dec, hexNum);
    decToOctBits(dec, octBits);
    decToHexBits(dec, hexBits);

    printf("\n------------------------------------\n");
    printf("   Conversion Results\n");
    printf("------------------------------------\n");
    printf(" Binary      : %s\n", bin);
    printf(" Octal       : %s   (bits: %s)\n", octNum, octBits);
    printf(" Decimal     : %lld\n", dec);
    printf(" Hexadecimal : %s   (bits: %s)\n", hexNum, hexBits);
    printf("------------------------------------\n");
    printf("      © Tonie 2025\n");
    printf("------------------------------------\n");
}

// --- Main Menu ---
int main() {
    int choice;
    char input[256];
    unsigned long long dec;

    while (1) {
        printf("\n====================================\n");
        printf("   NUMBER SYSTEM CONVERTER (v5)\n");
        printf("====================================\n");
        printf(" [1] Binary      → Octal, Decimal, Hex\n");
        printf(" [2] Octal       → Binary, Decimal, Hex\n");
        printf(" [3] Hexadecimal → Binary, Decimal, Octal\n");
        printf(" [4] Decimal     → Binary, Octal, Hex\n");
        printf(" [5] Exit\n");
        printf("------------------------------------\n");
        printf(" Choose option: ");

        if (scanf("%d", &choice) != 1) {
            printf("\n[!] Invalid input. Exiting.\n");
            break;
        }

        // flush leftover input
        while (getchar() != '\n');

        switch (choice) {
            case 1:
                printf("\n Enter Binary: ");
                if (!fgets(input, sizeof(input), stdin)) break;
                input[strcspn(input, "\n")] = '\0';
                if (!isValidBinary(input)) {
                    printf("\n[!] Invalid binary number.\n");
                    break;
                }
                dec = binToDec(input);
                printAll(dec);
                break;

            case 2:
                printf("\n Enter Octal: ");
                if (!fgets(input, sizeof(input), stdin)) break;
                input[strcspn(input, "\n")] = '\0';
                if (!isValidOctal(input)) {
                    printf("\n[!] Invalid octal number.\n");
                    break;
                }
                dec = strtoull(input, NULL, 8);
                printAll(dec);
                break;

            case 3:
                printf("\n Enter Hexadecimal: ");
                if (!fgets(input, sizeof(input), stdin)) break;
                input[strcspn(input, "\n")] = '\0';
                if (!isValidHex(input)) {
                    printf("\n[!] Invalid hexadecimal number.\n");
                    break;
                }
                dec = strtoull(input, NULL, 16);
                printAll(dec);
                break;

            case 4:
                printf("\n Enter Decimal: ");
                if (!fgets(input, sizeof(input), stdin)) break;
                input[strcspn(input, "\n")] = '\0';
                char *end;
                dec = strtoull(input, &end, 10);
                if (*end != '\0') {
                    printf("\n[!] Invalid decimal number.\n");
                } else {
                    printAll(dec);
                }
                break;

            case 5:
                printf("\n====================================\n");
                printf(" Exiting program...\n");
                printf("   Thank you for using!\n");
                printf("        © Tonie 2025\n");
                printf("====================================\n");
                exit(0);

            default:
                printf("\n Invalid choice! Try again.\n");
        }
    }

    return 0;
}

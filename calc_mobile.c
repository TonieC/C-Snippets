#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

double memory = 0.0; // Memory register

// HUD
void show_hud() {
    printf("=============================================\n");
    printf("           Console Calculator HUD            \n");
    printf("=============================================\n");
    printf(" Basic Ops   : a + b   | a - b | a * b | a / b\n");
    printf(" Powers      : pow a b | sqrt x | cbrt x      \n");
    printf(" Trig        : sin x   | cos x  | tan x       \n");
    printf(" Inverse     : asin x  | acos x | atan x      \n");
    printf(" Hyperbolic  : sinh x  | cosh x | tanh x      \n");
    printf(" Logs / Exp  : log x   | log10 x | exp x      \n");
    printf(" Other       : fabs x  | floor x | ceil x     \n");
    printf("             | round x                       \n");
    printf(" Memory      : m+ x   | m- x   | mr (recall)  \n");
    printf("=============================================\n");
    printf(" Commands    : help   | exit                  \n");
    printf("=============================================\n");
    printf(" Type 'help' for usage instructions.\n");
    printf(" Type 'exit' to quit.\n\n");
}

// Help
void show_help() {
    printf("\n================ Help Guide =================\n");
    printf("This calculator supports multiple operations:\n\n");

    printf("Basic Operations:\n");
    printf("  a + b   -> Addition\n");
    printf("  a - b   -> Subtraction\n");
    printf("  a * b   -> Multiplication\n");
    printf("  a / b   -> Division\n\n");

    printf("Powers & Roots:\n");
    printf("  pow a b -> a raised to power b\n");
    printf("  sqrt x  -> Square root of x\n");
    printf("  cbrt x  -> Cube root of x\n\n");

    printf("Trigonometry (x in radians):\n");
    printf("  sin x, cos x, tan x\n\n");

    printf("Inverse Trigonometry:\n");
    printf("  asin x, acos x, atan x\n\n");

    printf("Hyperbolic Functions:\n");
    printf("  sinh x, cosh x, tanh x\n\n");

    printf("Logs & Exponential:\n");
    printf("  log x    -> Natural log (base e)\n");
    printf("  log10 x  -> Logarithm base 10\n");
    printf("  exp x    -> e raised to power x\n\n");

    printf("Other Functions:\n");
    printf("  fabs x   -> Absolute value\n");
    printf("  floor x  -> Largest integer <= x\n");
    printf("  ceil x   -> Smallest integer >= x\n");
    printf("  round x  -> Nearest integer\n\n");

    printf("Memory Functions:\n");
    printf("  m+ x   -> Add x to memory\n");
    printf("  m- x   -> Subtract x from memory\n");
    printf("  mr     -> Recall memory value\n\n");

    printf("Commands:\n");
    printf("  help   -> Show this help guide\n");
    printf("  exit   -> Quit the program\n");
    printf("=============================================\n\n");
}

int main() {
    char input[100];
    double a, b;
    show_hud();

    while (1) {
        printf("> ");
        if (!fgets(input, sizeof(input), stdin)) break;

        // remove newline
        input[strcspn(input, "\n")] = 0;

        if (strcmp(input, "exit") == 0) {
            printf("Goodbye!\n");
            break;
        }
        if (strcmp(input, "help") == 0) {
            show_help();
            continue;
        }

        // Try parsing inputs
        if (sscanf(input, "%lf + %lf", &a, &b) == 2) printf("= %lf\n", a + b);
        else if (sscanf(input, "%lf - %lf", &a, &b) == 2) printf("= %lf\n", a - b);
        else if (sscanf(input, "%lf * %lf", &a, &b) == 2) printf("= %lf\n", a * b);
        else if (sscanf(input, "%lf / %lf", &a, &b) == 2) {
            if (b != 0) printf("= %lf\n", a / b);
            else printf("Error: Division by zero\n");
        }
        else if (sscanf(input, "pow %lf %lf", &a, &b) == 2) printf("= %lf\n", pow(a, b));
        else if (sscanf(input, "sqrt %lf", &a) == 1) printf("= %lf\n", sqrt(a));
        else if (sscanf(input, "cbrt %lf", &a) == 1) printf("= %lf\n", cbrt(a));
        else if (sscanf(input, "sin %lf", &a) == 1) printf("= %lf\n", sin(a));
        else if (sscanf(input, "cos %lf", &a) == 1) printf("= %lf\n", cos(a));
        else if (sscanf(input, "tan %lf", &a) == 1) printf("= %lf\n", tan(a));
        else if (sscanf(input, "asin %lf", &a) == 1) printf("= %lf\n", asin(a));
        else if (sscanf(input, "acos %lf", &a) == 1) printf("= %lf\n", acos(a));
        else if (sscanf(input, "atan %lf", &a) == 1) printf("= %lf\n", atan(a));
        else if (sscanf(input, "sinh %lf", &a) == 1) printf("= %lf\n", sinh(a));
        else if (sscanf(input, "cosh %lf", &a) == 1) printf("= %lf\n", cosh(a));
        else if (sscanf(input, "tanh %lf", &a) == 1) printf("= %lf\n", tanh(a));
        else if (sscanf(input, "log %lf", &a) == 1) printf("= %lf\n", log(a));
        else if (sscanf(input, "log10 %lf", &a) == 1) printf("= %lf\n", log10(a));
        else if (sscanf(input, "exp %lf", &a) == 1) printf("= %lf\n", exp(a));
        else if (sscanf(input, "fabs %lf", &a) == 1) printf("= %lf\n", fabs(a));
        else if (sscanf(input, "floor %lf", &a) == 1) printf("= %lf\n", floor(a));
        else if (sscanf(input, "ceil %lf", &a) == 1) printf("= %lf\n", ceil(a));
        else if (sscanf(input, "round %lf", &a) == 1) printf("= %lf\n", round(a));
        else if (sscanf(input, "m+ %lf", &a) == 1) {
            memory += a;
            printf("Memory = %lf\n", memory);
        }
        else if (sscanf(input, "m- %lf", &a) == 1) {
            memory -= a;
            printf("Memory = %lf\n", memory);
        }
        else if (strcmp(input, "mr") == 0) printf("Memory Recall = %lf\n", memory);
        else printf("Unknown command. Type 'help' for instructions.\n");
    }
    return 0;
}

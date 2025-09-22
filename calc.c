// calc.c
// Windows console calculator with HUD
// Compile: gcc calc.c -o calc.exe -lm -mconsole
// this code is made by Tonie 
// License: Public Domain
// Note: This code uses C17 standard
// Note: This code is designed for Windows console
// but may work on other platforms with minor adjustments.
// Features:
// - Basic arithmetic: + - * /
// - Scientific functions: sin, cos, tan, exp, log, sqrt, pow
// - Memory operations: M+, M-, MC
// - History of last 6 results
// - Angle mode toggle: degrees/radians

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <conio.h>
#include <windows.h>

#define HISTORY_SIZE 6
#define INPUT_SZ 128

// ANSI color helpers (Windows 10+ consoles support this if enabled)
// Colors
// Reset, Bright Cyan, Bright Yellow, Bright Green, Bright Red
// Note: If your console does not support ANSI, you may need to remove color codes.
// You can also use Windows API calls to set console text attributes if needed.
// For simplicity, we use ANSI codes here.
// ANSI escape codes
// Reference: https://en.wikipedia.org/wiki/ANSI_escape_code
// Format: \x1b[<code>m
// Example: \x1b[1;31m for bright red
// Reset: \x1b[0m
// Bright: 1
// Dim: 2
// Underscore: 4
// Blink: 5
const char *CLR_RESET = "\x1b[0m";
const char *CLR_HEADER = "\x1b[1;36m";   // bright cyan
const char *CLR_LABEL  = "\x1b[1;33m";   // bright yellow
const char *CLR_VALUE  = "\x1b[1;32m";   // bright green
const char *CLR_ERROR  = "\x1b[1;31m";   // bright red

// Global state
double memory_value = 0.0;
char history[HISTORY_SIZE][64];
int hist_count = 0;
int show_radians = 0; // 0 = degrees (default), 1 = radians

// Forward declarations
// Functions
void enable_ansi();
void clear_screen();
void add_history(const char *entry);
void draw_hud(const char *mode, const char *msg);
double prompt_number(const char *prompt, int *ok);
void basic_ops();
void scientific_ops();
void conversion_ops();
void show_help();

// Enable ANSI escape codes on Windows console
// This is needed for colored output
// Reference: https://docs.microsoft.com/en-us/windows/console/setconsolemode
// Note: This function should be called once at program start
void enable_ansi() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;
    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) return;
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}

// Clear console (cross-platform fallback)
void clear_screen() {
    // On Windows, prefer ANSI clear if available
    printf("\x1b[2J\x1b[H");
}

// Push a string to history (most recent at top)
void add_history(const char *entry) {
    // shift down
    for (int i = HISTORY_SIZE - 1; i > 0; --i) {
        strncpy(history[i], history[i-1], sizeof(history[i]));
        history[i][sizeof(history[i]) - 1] = '\0';
    }
    // insert at top
    strncpy(history[0], entry, sizeof(history[0]));
    history[0][sizeof(history[0]) - 1] = '\0';
    if (hist_count < HISTORY_SIZE) hist_count++;
}

// Draw the HUD
void draw_hud(const char *mode, const char *msg) {
    clear_screen();
    printf("%s+------------------------------------------------------------+%s\n", CLR_HEADER, CLR_RESET);
    printf("%s|%s                   Console Calculator                     %s|\n", CLR_HEADER, CLR_RESET, CLR_HEADER);
    printf("%s+------------------------------------------------------------+%s\n\n", CLR_HEADER, CLR_RESET);

    // Mode and memory
    printf("%sMode:%s  %s%-8s%s   %sMemory:%s  %s%g%s\n\n",
           CLR_LABEL, CLR_RESET, CLR_VALUE, mode, CLR_RESET,
           CLR_LABEL, CLR_RESET, CLR_VALUE, memory_value, CLR_RESET);

    // History box
    printf("%sHistory:%s\n", CLR_LABEL, CLR_RESET);
    printf("+------------------------------------------------------------+\n");
    for (int i = 0; i < HISTORY_SIZE; ++i) {
        if (i < hist_count && strlen(history[i]) > 0) {
            printf("| %-56s |\n", history[i]);
        } else {
            printf("| %-56s |\n", " ");
        }
    }
    printf("+------------------------------------------------------------+\n\n");

    // Instructions / shortcuts
    printf("%sShortcuts:%s  %s[1]%s Basic   %s[2]%s Scientific   %s[3]%s Conversions   %s[m]%s Mem+  %s[M]%s Mem-  %s[c]%s Clear Memory\n",
           CLR_LABEL, CLR_RESET,
           CLR_VALUE, CLR_RESET, CLR_VALUE, CLR_RESET, CLR_VALUE, CLR_RESET,
           CLR_VALUE, CLR_RESET, CLR_VALUE, CLR_RESET, CLR_VALUE, CLR_RESET);
    printf("%s          %s[h]%s Help   %s[q]%s Quit   %s[r]%s Toggle Deg/Rad (%s)%s\n\n",
           CLR_LABEL, CLR_RESET, CLR_LABEL, CLR_RESET, CLR_LABEL, CLR_RESET,
           CLR_LABEL, CLR_RESET, show_radians ? "RAD" : "DEG", CLR_RESET);

    if (msg && strlen(msg) > 0) {
        printf("%sMessage:%s %s\n\n", CLR_LABEL, CLR_RESET, msg);
    } else {
        printf("\n");
    }

    fflush(stdout);
}

// Prompt a double from user with validation. ok returns 1 on success.
double prompt_number(const char *prompt, int *ok) {
    char buf[INPUT_SZ];
    double val = 0.0;
    *ok = 0;
    printf("%s%s:%s ", CLR_LABEL, prompt, CLR_RESET);
    if (!fgets(buf, sizeof(buf), stdin)) return 0.0;
    // trim newline
    buf[strcspn(buf, "\r\n")] = 0;
    if (strlen(buf) == 0) return 0.0;
    // allow 'm' to insert memory
    if (strlen(buf) == 1 && (buf[0] == 'm' || buf[0] == 'M')) {
        val = memory_value;
        *ok = 1;
        return val;
    }
    char *end;
    val = strtod(buf, &end);
    if (end == buf || *end != '\0') {
        printf("%sInvalid number input.%s\n", CLR_ERROR, CLR_RESET);
        return 0.0;
    }
    *ok = 1;
    return val;
}

// Basic operations: + - * /
void basic_ops() {
    char opbuf[16];
    int ok;
    draw_hud("Basic", "Enter operation: + - * /  (or 'b' to go back)");
    printf("%sOperation:%s ", CLR_LABEL, CLR_RESET);
    if (!fgets(opbuf, sizeof(opbuf), stdin)) return;
    char op = opbuf[0];
    if (op == 'b' || op == 'B') return;

    double a = prompt_number("First number (or 'm' for memory)", &ok);
    if (!ok) { draw_hud("Basic", "Cancelled: invalid first number"); return; }
    double b = prompt_number("Second number (or 'm' for memory)", &ok);
    if (!ok) { draw_hud("Basic", "Cancelled: invalid second number"); return; }

    double res = 0.0;
    char entry[64];
    switch (op) {
        case '+': res = a + b; break;
        case '-': res = a - b; break;
        case '*': res = a * b; break;
        case '/':
            if (b == 0.0) { draw_hud("Basic", "Error: Division by zero"); return; }
            res = a / b; break;
        default:
            draw_hud("Basic", "Unknown operator"); return;
    }
    snprintf(entry, sizeof(entry), "%g %c %g = %g", a, op, b, res);
    add_history(entry);
    draw_hud("Basic", entry);
}

// Scientific operations
void scientific_ops() {
    char opbuf[32];
    int ok;
    draw_hud("Scientific", "Ops: sin cos tan exp log sqrt pow (enter name) or 'b' back");
    printf("%sOp name:%s ", CLR_LABEL, CLR_RESET);
    if (!fgets(opbuf, sizeof(opbuf), stdin)) return;
    // trim newline
    opbuf[strcspn(opbuf, "\r\n")] = 0;
    if (strlen(opbuf) == 0) return;
    if (opbuf[0] == 'b' || opbuf[0] == 'B') return;

    // unary functions
    if (strcmp(opbuf, "sin") == 0 || strcmp(opbuf, "cos") == 0 || strcmp(opbuf, "tan") == 0 ||
        strcmp(opbuf, "exp") == 0 || strcmp(opbuf, "log") == 0 || strcmp(opbuf, "sqrt") == 0) {
        double a = prompt_number("Value (or 'm')", &ok);
        if (!ok) { draw_hud("Scientific", "Cancelled: invalid number"); return; }
        double input_val = a;
        if (!show_radians && (strcmp(opbuf, "sin") == 0 || strcmp(opbuf, "cos") == 0 || strcmp(opbuf, "tan") == 0)) {
            // convert degrees to radians
            a = a * M_PI / 180.0;
        }
        double res = 0.0;
        if (strcmp(opbuf, "sin") == 0) res = sin(a);
        else if (strcmp(opbuf, "cos") == 0) res = cos(a);
        else if (strcmp(opbuf, "tan") == 0) res = tan(a);
        else if (strcmp(opbuf, "exp") == 0) res = exp(a);
        else if (strcmp(opbuf, "log") == 0) {
            if (a <= 0.0) { draw_hud("Scientific", "Error: log domain"); return; }
            res = log(a);
        }
        else if (strcmp(opbuf, "sqrt") == 0) {
            if (a < 0.0) { draw_hud("Scientific", "Error: sqrt domain"); return; }
            res = sqrt(a);
        }
        char entry[64];
        snprintf(entry, sizeof(entry), "%s(%g) = %g", opbuf, input_val, res);
        add_history(entry);
        draw_hud("Scientific", entry);
        return;
    }

    // binary pow
    // pow(base, exponent)
    // Example: pow(2, 3) = 8
    if (strcmp(opbuf, "pow") == 0) {
        double a = prompt_number("Base (or 'm')", &ok);
        if (!ok) { draw_hud("Scientific", "Cancelled: invalid base"); return; }
        double b = prompt_number("Exponent (or 'm')", &ok);
        if (!ok) { draw_hud("Scientific", "Cancelled: invalid exponent"); return; }
        double res = pow(a, b);
        char entry[64];
        snprintf(entry, sizeof(entry), "pow(%g, %g) = %g", a, b, res);
        add_history(entry);
        draw_hud("Scientific", entry);
        return;
    }

    draw_hud("Scientific", "Unknown scientific op");
}

// Conversion ops (deg/rad toggle, temperature, etc)
void conversion_ops() {
    char buf[32];
    int ok;
    draw_hud("Conversions", "Options: deg2rad rad2deg c2f f2c  (or 'b' back)");
    printf("%sOption:%s ", CLR_LABEL, CLR_RESET);
    if (!fgets(buf, sizeof(buf), stdin)) return;
    buf[strcspn(buf, "\r\n")] = 0;
    if (strlen(buf) == 0) return;
    if (buf[0] == 'b' || buf[0] == 'B') return;

    if (strcmp(buf, "deg2rad") == 0) {
        double a = prompt_number("Degrees", &ok);
        if (!ok) { draw_hud("Conversions", "Cancelled"); return; }
        double res = a * M_PI / 180.0;
        char entry[64]; snprintf(entry, sizeof(entry), "%g deg = %g rad", a, res);
        add_history(entry); draw_hud("Conversions", entry); return;
    }
    if (strcmp(buf, "rad2deg") == 0) {
        double a = prompt_number("Radians", &ok);
        if (!ok) { draw_hud("Conversions", "Cancelled"); return; }
        double res = a * 180.0 / M_PI;
        char entry[64]; snprintf(entry, sizeof(entry), "%g rad = %g deg", a, res);
        add_history(entry); draw_hud("Conversions", entry); return;
    }
    if (strcmp(buf, "c2f") == 0) {
        double c = prompt_number("Celsius", &ok); if (!ok) { draw_hud("Conversions", "Cancelled"); return; }
        double f = (c * 9.0 / 5.0) + 32.0;
        char entry[64]; snprintf(entry, sizeof(entry), "%g C = %g F", c, f);
        add_history(entry); draw_hud("Conversions", entry); return;
    }
    if (strcmp(buf, "f2c") == 0) {
        double f = prompt_number("Fahrenheit", &ok); if (!ok) { draw_hud("Conversions", "Cancelled"); return; }
        double c = (f - 32.0) * 5.0 / 9.0;
        char entry[64]; snprintf(entry, sizeof(entry), "%g F = %g C", f, c);
        add_history(entry); draw_hud("Conversions", entry); return;
    }

    draw_hud("Conversions", "Unknown conversion option");
}
// Show help screen
void show_help() {
    draw_hud("Help", "Type a shortcut key or choose an option. Use 'm' to add last result to memory.");
    printf("\n%sDetailed Help:%s\n", CLR_LABEL, CLR_RESET);
    printf(" - Basic: enter operator (+ - * /) then two numbers. Use 'm' when prompted for number to use memory.\n");
    printf(" - Scientific: sin cos tan exp log sqrt pow. trig uses degrees by default (toggle r to change).\n");
    printf(" - Memory: press M to subtract, m to add last result to memory, c clears memory.\n");
    printf(" - History shows last %d results.\n", HISTORY_SIZE);
    printf("\nPress Enter to return...");
    getchar();
}

int main() {
    enable_ansi();
    memset(history, 0, sizeof(history));
    char choice[8];
    char msg[128] = "";
    char last_entry[64] = "";
    double last_result = 0.0;
// Main loop
// Clear screen and show HUD
// Wait for user input
// Process input
    while (1) {
        draw_hud("Idle", msg);
        msg[0] = '\0';
        printf("%sChoose:[1]Basic [2]Sci [3]Conv [m]Mem+ [M]Mem- [c]ClearMem [r]ToggleDeg/Rad [h]Help [q]Quit%s\n> ",
               CLR_LABEL, CLR_RESET);
        if (!fgets(choice, sizeof(choice), stdin)) break;
        char cmd = choice[0];

        if (cmd == '1') basic_ops();
        else if (cmd == '2') scientific_ops();
        else if (cmd == '3') conversion_ops();
        else if (cmd == 'm') {
            // add last result to memory
            // only if we have a last result
            // check history[0]
            // if history[0] is empty, no last result
            if (strlen(history[0]) == 0) { strncpy(msg, "No result to add to memory", sizeof(msg)); }
            else {
                memory_value += last_result;
                snprintf(msg, sizeof(msg), "Added %g to memory. (mem=%g)", last_result, memory_value);
                char buf[64]; snprintf(buf, sizeof(buf), "M+ %g -> mem=%g", last_result, memory_value);
                add_history(buf);
            }
        }
        else if (cmd == 'M') {
            // subtract last result
            if (strlen(history[0]) == 0) { strncpy(msg, "No result to subtract from memory", sizeof(msg)); }
            else {
                memory_value -= last_result;
                snprintf(msg, sizeof(msg), "Subtracted %g from memory. (mem=%g)", last_result, memory_value);
                char buf[64]; snprintf(buf, sizeof(buf), "M- %g -> mem=%g", last_result, memory_value);
                add_history(buf);
            }
        }
        // clear memory
        else if (cmd == 'c') {
            memory_value = 0.0;
            strncpy(msg, "Memory cleared", sizeof(msg));
        }
        else if (cmd == 'r') {
            show_radians = !show_radians;
            snprintf(msg, sizeof(msg), "Angle mode: %s", show_radians ? "RAD" : "DEG");
        }
        else if (cmd == 'h') show_help();
        else if (cmd == 'q') break;
        else {
            strncpy(msg, "Unknown command", sizeof(msg));
        }

        // Update last_result if history top changed (quick heuristic)
        if (hist_count > 0 && strlen(history[0]) > 0) {
            // try parse last number from history[0]
            double val = 0.0;
            const char *s = history[0];
            const char *p = strrchr(s, '=');
            if (p) {
                p++;
                while (*p == ' ') p++;
                char *end;
                val = strtod(p, &end);
                if (end != p) last_result = val;
            }
        }
    }
// Exit
// Clear screen and say goodbye
    clear_screen();
    printf("Goodbye.\n");
    return 0;
}

// Note: This code is made by Tonie and is in the Public Domain.
// You can use, modify, and distribute it freely.
// Please keep this notice if you share the code.
// Enjoy!

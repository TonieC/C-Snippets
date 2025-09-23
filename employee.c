#include <stdio.h>
#include <string.h>

// Max employees and max extra tax entries per employee
#define MAX_EMPLOYEES 100
#define MAX_EXTRA_TAX 5

// Struct for extra tax with label + amount
typedef struct {
    char label[30];
    double amount;
} ExtraTax;

// Employee struct with basic info + tax details
typedef struct {
    char name[50];       // Employee name
    int age;             // Employee age
    char email[50];      // Employee email
    double salary;       // Monthly salary
    double tax;          // Base tax (TRAIN law)
    ExtraTax extraTaxes[MAX_EXTRA_TAX]; // Up to 5 extra deductions
    int extraCount;      // Number of extra deductions
    double totalTax;     // Base tax + all extras
    double netPay;       // Salary - totalTax
} Employee;

// Function to compute base monthly tax using TRAIN law
double computeTax(double salary) {
    if (salary <= 250000.0 / 12) {
        return 0.0; // No tax if <= 250k/year
    } else if (salary <= 400000.0 / 12) {
        return (salary * 12 - 250000.0) * 0.20 / 12;
    } else if (salary <= 800000.0 / 12) {
        return (30000.0 + (salary * 12 - 400000.0) * 0.25) / 12;
    } else if (salary <= 2000000.0 / 12) {
        return (130000.0 + (salary * 12 - 800000.0) * 0.30) / 12;
    } else if (salary <= 8000000.0 / 12) {
        return (490000.0 + (salary * 12 - 2000000.0) * 0.32) / 12;
    } else {
        return (2410000.0 + (salary * 12 - 8000000.0) * 0.35) / 12;
    }
}

int main() {
    Employee employees[MAX_EMPLOYEES]; // Array to store employees
    int count = 0;                     // Current number of employees
    char choice;                       // User choice to add more

    do {
        if (count >= MAX_EMPLOYEES) {
            printf("Employee list is full!\n");
            break;
        }

        // Get employee basic details
        printf("\nEnter employee name: ");
        scanf(" %[^\n]s", employees[count].name);

        printf("Enter employee age: ");
        scanf("%d", &employees[count].age);

        printf("Enter employee email: ");
        scanf(" %[^\n]s", employees[count].email);

        // Salary and tax calculation
        printf("Enter monthly salary: ");
        scanf("%lf", &employees[count].salary);

        employees[count].tax = computeTax(employees[count].salary);
        employees[count].extraCount = 0;
        employees[count].totalTax = employees[count].tax;

        // Ask if employee has extra taxes (SSS, PhilHealth, etc.)
        char extraChoice;
        printf("Does this employee have extra taxes/deductions? (y/n): ");
        scanf(" %c", &extraChoice);

        if (extraChoice == 'y' || extraChoice == 'Y') {
            for (int i = 0; i < MAX_EXTRA_TAX; i++) {
                double extra;
                char label[30];

                // Ask for label (e.g., SSS, PhilHealth, Pag-IBIG)
                printf("Enter label for extra tax #%d: ", i + 1);
                scanf(" %[^\n]s", label);

                // Ask for amount (enter 0 to stop early)
                printf("Enter amount for %s (or 0 to stop): ", label);
                scanf("%lf", &extra);

                if (extra <= 0) break; // Stop if user enters 0

                // Store label + amount
                strcpy(employees[count].extraTaxes[employees[count].extraCount].label, label);
                employees[count].extraTaxes[employees[count].extraCount].amount = extra;

                // Update totals
                employees[count].extraCount++;
                employees[count].totalTax += extra;
            }
        }

        // Compute Net Pay
        employees[count].netPay = employees[count].salary - employees[count].totalTax;

        // Move to next employee
        count++;

        printf("Do you want to add another employee? (y/n): ");
        scanf(" %c", &choice);

    } while (choice == 'y' || choice == 'Y');

    // Print summary table
    printf("\n%-20s %-5s %-25s %-12s %-12s %-12s %-12s\n",
           "Employee Name", "Age", "Email", "Salary", "Base Tax", "Total Tax", "Net Pay");
    printf("---------------------------------------------------------------------------------------------------------\n");

    // Loop through employees and print their info
    for (int i = 0; i < count; i++) {
        printf("%-20s %-5d %-25s %-12.2f %-12.2f %-12.2f %-12.2f\n",
               employees[i].name,
               employees[i].age,
               employees[i].email,
               employees[i].salary,
               employees[i].tax,
               employees[i].totalTax,
               employees[i].netPay);

        // Print extra taxes if any
        if (employees[i].extraCount > 0) {
            printf("   Extra Taxes:\n");
            for (int j = 0; j < employees[i].extraCount; j++) {
                printf("      %s: %.2f\n",
                       employees[i].extraTaxes[j].label,
                       employees[i].extraTaxes[j].amount);
            }
        }
    }

    return 0;
}

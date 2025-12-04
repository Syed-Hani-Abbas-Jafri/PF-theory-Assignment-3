#include <stdio.h>
#include <math.h>

float recursiveRepay(float loan, float inst, float rate, int year, int yrs, float extra) {
    if (year > yrs || loan <= 0) {
        return 0;
    }

    float intr = loan * rate;
    float pay = inst + extra;
    float owed = loan + intr;

    if (owed < pay) {
        pay = owed;
    }

    float rem = owed - pay;

    printf("Year %d: Interest = %.2f | Payment = %.2f | Remaining loan = %.2f\n",
           year, intr, pay, rem);

    return pay + recursiveRepay(rem, inst, rate, year + 1, yrs, extra);
}

void calculateRepayment(float loan, float rate, int yrs) {
    float num = loan * rate * pow((1 + rate), yrs);
    float den = pow((1 + rate), yrs) - 1;
    float inst = num / den;

    printf("--- Loan: %.2f | Rate: %.0f%% | Duration: %d Years ---\n", loan, rate * 100, yrs);
    printf("Calculated Fixed Annual Installment: %.2f\n\n", inst);

    float total = recursiveRepay(loan, inst, rate, 1, yrs, 0);

    printf("\nTotal Repayment Amount: %.2f\n", total);
}

int main() {
    calculateRepayment(100000, 0.05, 3);
    return 0;
}

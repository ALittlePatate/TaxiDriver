#include "math_module.h"

MathModule::MathModule() {
    // Constructor, if any initialization is needed
}

int MathModule::add(int a, int b) {
    return a + b;
}

int MathModule::subtract(int a, int b) {
    return a - b;
}

int MathModule::multiply(int a, int b) {
    return a * b;
}

double MathModule::divide(double a, double b) {
    if (b == 0) {
        throw "Division by zero is not allowed.";
    }
    return a / b;
}

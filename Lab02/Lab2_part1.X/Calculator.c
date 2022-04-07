// Standard libraries
#include <stdio.h>
#include <math.h>


//User libraries:
#include "BOARD.h"

#ifdef PIC32
// Microchip libraries
#include <xc.h>
#endif


// **** Declare function prototypes ****
double Add(double operand1, double operand2);
double Subtract(double operand1, double operand2);
double Multiply(double operand1, double operand2);
double Divide(double operand1, double operand2);
double Average(double operand1, double operand2);
double AbsoluteValue(double operand);
double FahrenheitToCelsius(double operand);
double CelsiusToFahrenheit(double operand);
double Tangent(double operand);
//add more prototypes here

void CalculatorRun(void) {
    printf("\nWelcome to NAME's calculator program! Compiled at %s %s\n", __DATE__, __TIME__);
    // Your code here
    while (1) {
        char op;
        double operand1, operand2, result;
        printf("Enter a mathematical operation to perform(*,/,+/-/m/a/c/f/t/q): ");
        scanf("%s", &op);
        if (op == 'q') {
            return;
        }
        if (op != '*' && op != '/' && op != '+' && op != '-' && op != 'm' && op != 'a' && op != 'c' && op != 'f' && op != 't'){
            op = 0;
            printf("You have entered an Invalid Operator.\n");
            continue;
        }
        printf("Enter the first operand: ");
        scanf("%lf", &operand1);
        if (op == '*' || op == '/' || op == '+' || op == '-' || op == 'm') {
            printf("Enter the second operand:");
            scanf("%lf", &operand2);
        }
        if (op == '*') {
            result = Multiply(operand1, operand2);
            printf("Result of (%lf * %lf):%lf\n", operand1, operand2, result);
        }
        else if (op == '/') {
            if (operand2 == 0) {
                printf("Divide by zero error!\n");
            } else {
                result = Divide(operand1, operand2);
                printf("Result of (%lf / %lf):%lf\n", operand1, operand2, result);
            }
        }
        else if (op == '+') {
            result = Add(operand1, operand2);
            printf("Result of (%lf + %lf):%lf\n", operand1, operand2, result);
        }
        else if (op == '-') {
            result = Subtract(operand1, operand2);
            printf("Result of (%lf - %lf):%lf\n", operand1, operand2, result);
        }
        else if (op == 'm') {
            result = Average(operand1, operand2);
            printf("Result of average(%lf and %lf):%lf\n", operand1, operand2, result);
        }
        else if (op == 'a') {
            result = AbsoluteValue(operand1);
            printf("Result of |%lf|:%lf\n", operand1, result);
        }
        else if (op == 'c') {
            result = FahrenheitToCelsius(operand1);
            printf("Result of FahrenheitToCelsius(%lf):%lf\n", operand1, result);
        }
        else if (op == 'f') {
            result = CelsiusToFahrenheit(operand1);
            printf("Result of CelsiusToFahrenheit(%lf):%lf\n", operand1, result);
        }
        else if (op == 't') {
            result = Tangent(operand1);
            printf("Result of tan(%lf):%lf\n", operand1, result);
        }
    }



    BOARD_End();
}

/********************************************************************************
 * Define the Add function here.
 ********************************************************************************/
double Add(double operand1, double operand2) {
    double result = operand1 + operand2;
    return result;
}

/********************************************************************************
 * Define the Subtract function here.
 ********************************************************************************/
double Subtract(double operand1, double operand2) {
    double result = operand1 - operand2;
    return result;
}

/********************************************************************************
 * Define the Multiply function here.
 ********************************************************************************/
double Multiply(double operand1, double operand2) {
    double result = operand1 * operand2;
    return result;
}

/********************************************************************************
 * Define the Divide function here.
 ********************************************************************************/
double Divide(double operand1, double operand2) {
    double result = operand1 / operand2;
    return result;
}

/********************************************************************************
 * Define the Absolute Value function here.
 ********************************************************************************/
double AbsoluteValue(double operand) {
    if (operand < 0){
        double result = operand * -1;
        return result;
    }
    else{
        return operand;
    }
   
}

/*********************************************************************************
 * Define the Fahrenheit to Celsius function here.
 ********************************************************************************/
double FahrenheitToCelsius(double operand) {
    double result = (operand - 32.0) * (5.0/9.0);
    return result;
}

/*********************************************************************************
 * Define the Celsius to Fahrenheit function here.
 ********************************************************************************/
double CelsiusToFahrenheit(double operand) {
    double result = (operand * (9.0/5.0)) + 32;
    return result;
}

/********************************************************************************
 * Define the Average function here.
 *******************************************************************************/
double Average(double operand1, double operand2) {
    double result = (operand1 + operand2)/2;
    return result;
}

/*********************************************************************************
 * Define the Tangent function that takes input in degrees.
 ********************************************************************************/
double Tangent(double operand) {
    double degrees = operand * (M_PI/180);
    double result = tan(degrees);
    return result;
}

/*********************************************************************************
 * Define the Round function here.
 * In order to receive the extra credit your calculator MUST ALSO CORRECTLY utilize
 * this function.
 ********************************************************************************/
double Round(double operand) {
    int rounded = operand;
    return rounded;
}



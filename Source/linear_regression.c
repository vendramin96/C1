#include "linear_regression.h"

internal_function double
MeanSquaredError(double *X, double *Y, double W, double B, int N)
{
    double Cost = 0.0;
    for(int Index = 0; Index < N; Index++)
    {
        double Predicted = W * X[Index] + B;
        Cost += (Y[Index] - Predicted) * (Y[Index] - Predicted);
    }

    return Cost / N;
}

internal_function void
GradientDescedent(double *X, double *Y, double *W, double *B, int N, double Alpha, int Iterations)
{
    for(int Iteration = 0; Iteration < Iterations; Iteration++)
    {
        double DW = 0.0;
        double DB = 0.0;
        for(int Index = 0; Index < N; Index++)
        {
            double Predicted = *W * X[Index] + *B;
            DW += -2 * X[Index] * (Y[Index] - Predicted);
            DB += -2 * (Y[Index] - Predicted);
        }

        *W -= Alpha * DW / N;
        *B -= Alpha * DB / N;

        if(Iteration % 100)
        {
            Print("Iteration %d: Cost = %f\n", Iteration, MeanSquaredError(X, Y, *W, *B, N));
        }
    }
}

int main(int ArgumentCount, char **Argument)
{
    int Result = 1;

    double X[] = { 1, 2, 3, 4, 5};
    double Y[] = {1, 2, 1.3, 3.75, 2.25};
    int N = 5;

    double W = 0.0;
    double B = 0.0;
    double Alpha = 0.01;
    int Iterations = 1000;

    GradientDescedent(X, Y, &W, &B, N, Alpha, Iterations);

    Print("Trained weight (w): %f\n", W);
    Print("Trained bias (b): %f\n", B);

    double NewX = 0.0;
    double Prediction = W * NewX + B;
    Print("Prediction for x = 6: %f\n", Prediction);

    Result = 0;
    return Result;
}
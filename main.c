#include <stdio.h>
#include <stdlib.h>
#include <string.h> // For strcmp
#include <time.h>   // For measuring CPU time
#include "utility.h" // Includes definitions for ObjectiveFunction and Bound
#include "OF_lib.h"  // Includes objective functions

int main(int argc, char *argv[]) {
    if (argc < 7) {
        printf("Usage: %s <function> <numVariables> <lowerBound> <upperBound> <numParticles> <maxIterations>\n", argv[0]);
        return 1;
    }

    const char *functionName = argv[1];
    int numVariables = atoi(argv[2]);
    double lowerBound = atof(argv[3]);
    double upperBound = atof(argv[4]);
    int numParticles = atoi(argv[5]);
    int maxIterations = atoi(argv[6]);

    // Set up the objective function
    ObjectiveFunction objFunc = NULL;
    if (strcmp(functionName, "griewank") == 0) objFunc = griewank;
    else if (strcmp(functionName, "levy") == 0) objFunc = levy;
    else if (strcmp(functionName, "rastrigin") == 0) objFunc = rastrigin;
    else if (strcmp(functionName, "rosenbrock") == 0) objFunc = rosenbrock;
    else if (strcmp(functionName, "schwefel") == 0) objFunc = schwefel;
    else if (strcmp(functionName, "dixon_price") == 0) objFunc = dixon_price;
    else if (strcmp(functionName, "michalewicz") == 0) objFunc = michalewicz;
    else if (strcmp(functionName, "styblinski_tang") == 0) objFunc = styblinski_tang;
    else {
        printf("Unknown function: %s\n", functionName);
        return 1;
    }

    // Set up bounds
    Bound *bounds = malloc(numVariables * sizeof(Bound));
    for (int i = 0; i < numVariables; i++) {
        bounds[i].lowerBound = lowerBound;
        bounds[i].upperBound = upperBound;
    }

    // Allocate space for the best position
    double *bestPosition = malloc(numVariables * sizeof(double));

    // Measure CPU time
    clock_t start = clock();

    // Run the PSO algorithm
    double bestFitness = pso(objFunc, functionName, numVariables, bounds, numParticles, maxIterations, bestPosition);

    clock_t end = clock();
    double cpuTimeUsed = ((double)(end - start)) / CLOCKS_PER_SEC;

    // Print the results
    printf("----------------------------------------------\n");
    printf("Function: %s\n", functionName);
    printf("Bounds: [%.6f, %.6f]\n", lowerBound, upperBound);
    printf("Particles: %d\n", numParticles);
    printf("Iterations: %d\n", maxIterations);
    printf("CPU Time: %.6f seconds\n", cpuTimeUsed);
    printf("Optimal fitness: %.6f\n", bestFitness);
    printf("Optimal position:\n");
    for (int i = 0; i < numVariables; i++) {
        printf("%.6f ", bestPosition[i]);
    }
    printf("\n----------------------------------------------\n");

    // Clean up
    free(bounds);
    free(bestPosition);

    return 0;
}

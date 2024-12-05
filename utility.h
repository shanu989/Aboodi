#ifndef UTILITY_H
#define UTILITY_H

typedef struct {
    double lowerBound;
    double upperBound;
} Bound;

typedef double (*ObjectiveFunction)(int, double *);

double pso(
    ObjectiveFunction objective_function,
    const char *func_name,
    int NUM_VARIABLES,
    Bound *bounds,
    int NUM_PARTICLES,
    int MAX_ITERATIONS,
    double *best_position);

#endif // UTILITY_H
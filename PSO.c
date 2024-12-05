#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h> // For strcmp
#include "utility.h"
#include "OF_lib.h"

// Generate random double within a range
double random_double(double min, double max) {
    return min + (rand() / (RAND_MAX / (max - min)));
}

double pso(ObjectiveFunction objective_function, const char *func_name, int NUM_VARIABLES, Bound *bounds, int NUM_PARTICLES, int MAX_ITERATIONS, double *best_position) {
    double inertia_start = 0.9, inertia_end = 0.4;
    double c1 = 1.5, c2 = 2.0;
    double global_best_fitness = INFINITY;
    double *global_best_position = malloc(NUM_VARIABLES * sizeof(double));

    // Allocate memory for particles
    double **positions = malloc(NUM_PARTICLES * sizeof(double *));
    double **velocities = malloc(NUM_PARTICLES * sizeof(double *));
    double **personal_best_positions = malloc(NUM_PARTICLES * sizeof(double *));
    double *personal_best_fitness = malloc(NUM_PARTICLES * sizeof(double));

    for (int i = 0; i < NUM_PARTICLES; i++) {
        positions[i] = malloc(NUM_VARIABLES * sizeof(double));
        velocities[i] = malloc(NUM_VARIABLES * sizeof(double));
        personal_best_positions[i] = malloc(NUM_VARIABLES * sizeof(double));

        for (int j = 0; j < NUM_VARIABLES; j++) {
            positions[i][j] = random_double(bounds[j].lowerBound, bounds[j].upperBound);
            velocities[i][j] = random_double(
                -fabs(bounds[j].upperBound - bounds[j].lowerBound) * 0.5,
                fabs(bounds[j].upperBound - bounds[j].lowerBound) * 0.5);
            personal_best_positions[i][j] = positions[i][j];
        }

        personal_best_fitness[i] = objective_function(NUM_VARIABLES, positions[i]);
        if (personal_best_fitness[i] < global_best_fitness) {
            global_best_fitness = personal_best_fitness[i];
            memcpy(global_best_position, personal_best_positions[i], NUM_VARIABLES * sizeof(double));
        }
    }

    // Main PSO loop
    for (int iter = 0; iter < MAX_ITERATIONS; iter++) {
        double inertia = inertia_start - ((inertia_start - inertia_end) * (double)iter / MAX_ITERATIONS);

        // Print iteration progress
        if (iter % 1 == 0 || iter == MAX_ITERATIONS - 1) {
            printf("Iteration %d: Best fitness so far: %f\n", iter, global_best_fitness);
        }

        // Adjust parameters for specific functions
        if (strcmp(func_name, "rastrigin") == 0) {
            // inertia = 0.7;
            // c1 = 1.5;
            // c2 = 1.5;
        } else if (strcmp(func_name, "rosenbrock") == 0) {
            // inertia = 0.7;
            // c1 = 1.5;
            // c2 = 1.5;
        } else if (strcmp(func_name, "schwefel") == 0) {
            // inertia = 0.7;
        }

        for (int i = 0; i < NUM_PARTICLES; i++) {
            for (int j = 0; j < NUM_VARIABLES; j++) {
                double r1 = random_double(0.0, 1.0);
                double r2 = random_double(0.0, 1.0);

                velocities[i][j] = inertia * velocities[i][j]
                    + c1 * r1 * (personal_best_positions[i][j] - positions[i][j])
                    + c2 * r2 * (global_best_position[j] - positions[i][j]);

                // Clamp velocity
                double max_velocity = fabs(bounds[j].upperBound - bounds[j].lowerBound) * 0.2;
                if (velocities[i][j] > max_velocity) velocities[i][j] = max_velocity;
                if (velocities[i][j] < -max_velocity) velocities[i][j] = -max_velocity;

                positions[i][j] += velocities[i][j];

                // Clamp positions within bounds
                if (positions[i][j] < bounds[j].lowerBound) {
                    positions[i][j] = bounds[j].lowerBound;
                } else if (positions[i][j] > bounds[j].upperBound) {
                    positions[i][j] = bounds[j].upperBound;
                }
            }

            double fitness = objective_function(NUM_VARIABLES, positions[i]);
            if (fitness < personal_best_fitness[i]) {
                personal_best_fitness[i] = fitness;
                memcpy(personal_best_positions[i], positions[i], NUM_VARIABLES * sizeof(double));
            }

            if (fitness < global_best_fitness) {
                global_best_fitness = fitness;
                memcpy(global_best_position, positions[i], NUM_VARIABLES * sizeof(double));
            }
        }
    }

    memcpy(best_position, global_best_position, NUM_VARIABLES * sizeof(double));

    // Free allocated memory
    for (int i = 0; i < NUM_PARTICLES; i++) {
        free(positions[i]);
        free(velocities[i]);
        free(personal_best_positions[i]);
    }
    free(positions);
    free(velocities);
    free(personal_best_positions);
    free(personal_best_fitness);
    free(global_best_position);

    return global_best_fitness;
}

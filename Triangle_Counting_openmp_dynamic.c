#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int count_triangles(int** graph, int num_nodes) {
    int triangles = 0;

#pragma omp parallel for reduction(+:triangles) schedule(dynamic)
    for (int i = 0; i < num_nodes; i++) {
        for (int j = i + 1; j < num_nodes; j++) {
            if (graph[i][j] == 1) {
#pragma omp parallel for reduction(+:triangles) schedule(static)
                for (int k = j + 1; k < num_nodes; k++) {
                    if (graph[i][k] == 1 && graph[j][k] == 1)
                        triangles++;
                }
            }
        }
    }

    return triangles;
}

int main() {
    int num_nodes;
    printf("Enter the number of nodes: ");
    scanf("%d", &num_nodes);

    int** graph = (int**)malloc(num_nodes * sizeof(int*));
    for (int i = 0; i < num_nodes; i++) {
        graph[i] = (int*)malloc(num_nodes * sizeof(int));
    }

    // Initialize the graph, for simplicity, let's assume an undirected graph
    // and fill the adjacency matrix randomly.
    for (int i = 0; i < num_nodes; i++) {
        for (int j = i + 1; j < num_nodes; j++) {
            graph[i][j] = graph[j][i] = rand() % 2;
        }
    }

    // Print the adjacency matrix (optional)
    printf("Adjacency Matrix:\n");
    for (int i = 0; i < num_nodes; i++) {
        for (int j = 0; j < num_nodes; j++) {
            printf("%d ", graph[i][j]);
        }
        printf("\n");
    }

    // Count triangles using OpenMP
    double start_time = omp_get_wtime();
    int triangles = count_triangles(graph, num_nodes);
    double end_time = omp_get_wtime();

    printf("Number of triangles: %d\n", triangles);
    printf("Time elapsed: %f seconds\n", end_time - start_time);

    // Free allocated memory
    for (int i = 0; i < num_nodes; i++) {
        free(graph[i]);
    }
    free(graph);

    return 0;
}


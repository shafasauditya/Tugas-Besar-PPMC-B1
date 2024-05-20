/** EL2208 Praktikum Pemecahan Masalah dengan C 2023/2024
 *   Modul               : 09
 *   Nama (NIM)          : Gemuruh Bagus Wirayudha (13222019)
 *   Nama File           : DFS.c
 *   Deskripsi           : Program berisikan cara menyelesaikan TSP dengan algortima DFS.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define R 6371
#define MAX_CITIES 100

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

struct Kota {
    char Nama[50];
    float Latitude;
    float Longitude;
};

struct ArrayKota {
    struct Kota* array_kota;
    int size;
    int neff;
};

float distance(float lat1, float lon1, float lat2, float lon2) {
    float lat = (lat2 - lat1) * M_PI / 180.0f;
    float lon = (lon2 - lon1) * M_PI / 180.0f;
    float a = sinf(lat / 2) * sinf(lat / 2) + cosf(lat1 * M_PI / 180.0f) * cosf(lat2 * M_PI / 180.0f) * sinf(lon / 2) * sinf(lon / 2);
    float c = 2 * atanf(sqrtf(a) / sqrtf(1 - a));
    float d = R * c;
    return d;
}

void DFS(int curr_city, int* visited, int* path, int level, int n, float adj_matrix[][MAX_CITIES], float curr_distance, float* min_distance, int* best_path, int start_index, clock_t start_time) {
    clock_t current_time = clock();
    double elapsed_time = (double)(current_time - start_time) / CLOCKS_PER_SEC;

    if (elapsed_time > 60) {
        char response[4]; // Array large enough to hold "Yes" or "No"
        printf("Execution has exceeded 1 minute. Do you want to continue? (Perkiraan selama 10 Jam) Yes/No: ");
        scanf(" %3s", response);

        if (strcmp(response, "No") == 0 || strcmp(response, "no") == 0) {
            exit(0);
        }

        start_time = clock(); // Reset start time
    }

    visited[curr_city] = 1;
    path[level] = curr_city;

    if (level == n - 1) {
        float total_distance = curr_distance + adj_matrix[curr_city][start_index];
        if (total_distance < *min_distance) {
            *min_distance = total_distance;
            memcpy(best_path, path, n * sizeof(int));
        }
    } else {
        for (int i = 0; i < n; i++) {
            if (!visited[i]) {
                DFS(i, visited, path, level + 1, n, adj_matrix, curr_distance + adj_matrix[curr_city][i], min_distance, best_path, start_index, start_time);
            }
        }
    }

    visited[curr_city] = 0;
}

int main() {
    char filename[100];
    char start[50];
    struct Kota* array_kota = (struct Kota*)malloc(sizeof(struct Kota) * MAX_CITIES);
    struct ArrayKota array_kota_struct = { array_kota, MAX_CITIES, 0 };

    while (1) {
        printf("Enter list of cities file name: ");
        scanf("%99s", filename);

        FILE* file = fopen(filename, "r");

        if (file == NULL) {
            printf("Failed to open file %s. Ensure the file exists.\n", filename);
        } else {
            char line[100];
            int i = 0;
            while (fgets(line, sizeof(line), file)) {
                char nama[50];
                float Latitude, Longitude;

                char* token = strtok(line, ",");
                if (token == NULL) continue;
                strncpy(nama, token, sizeof(nama));
                nama[sizeof(nama) - 1] = '\0'; // Ensure null termination

                token = strtok(NULL, ",");
                if (token == NULL) continue;
                Latitude = atof(token);

                token = strtok(NULL, ",");
                if (token == NULL) continue;
                Longitude = atof(token);

                strncpy(array_kota[i].Nama, nama, sizeof(array_kota[i].Nama));
                array_kota[i].Nama[sizeof(array_kota[i].Nama) - 1] = '\0'; // Ensure null termination
                array_kota[i].Latitude = Latitude;
                array_kota[i].Longitude = Longitude;
                i++;
            }

            array_kota_struct.neff = i;
            fclose(file);

            if (array_kota_struct.neff == 0) {
                printf("The file is empty. Please provide a file with city data.\n");
            } else {
                break;
            }
        }
    }

    // Clear input buffer
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }

    while (1) {
        printf("Enter starting point: ");
        // Use fgets to read the full city name including spaces
        fgets(start, sizeof(start), stdin);
        // Remove newline character if present
        start[strcspn(start, "\n")] = '\0';

        int start_index = -1;
        for (int j = 0; j < array_kota_struct.neff; j++) {
            if (strcmp(array_kota_struct.array_kota[j].Nama, start) == 0) {
                start_index = j;
                break;
            }
        }

        if (start_index == -1) {
            printf("Starting point not found. Please enter a valid city name.\n");
        } else {
            float adj_matrix[MAX_CITIES][MAX_CITIES] = { 0 };
            for (int j = 0; j < array_kota_struct.neff; j++) {
                for (int k = 0; k < array_kota_struct.neff; k++) {
                    adj_matrix[j][k] = distance(array_kota[j].Latitude, array_kota[j].Longitude, array_kota[k].Latitude, array_kota[k].Longitude);
                }
            }

            int path[MAX_CITIES], best_path[MAX_CITIES], visited[MAX_CITIES] = { 0 };
            float min_distance = INFINITY;
            clock_t start_time = clock();

            DFS(start_index, visited, path, 0, array_kota_struct.neff, adj_matrix, 0, &min_distance, best_path, start_index, start_time);

            clock_t end_time = clock();

            printf("Best route found:\n%s -> ", array_kota_struct.array_kota[start_index].Nama);
            for (int j = 1; j < array_kota_struct.neff; j++) {
                printf("%s -> ", array_kota_struct.array_kota[best_path[j]].Nama);
            }
            printf("%s\n", array_kota_struct.array_kota[start_index].Nama);

            printf("Minimum distance for the best route: %.2f kilometers\n", min_distance);
            printf("Time elapsed: %.6f seconds\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);

            break;
        }
    }

    free(array_kota_struct.array_kota);

    return 0;
}

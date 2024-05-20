/** EL2208 Praktikum Pemecahan Masalah dengan C 2023/2024
 *   Modul               : 09
 *   Nama (NIM)          : Mohammad Gabriel Hammam (13222014)
 *   Nama File           : BruteForce.c
 *   Deskripsi           : Program berisikan cara menyelesaikan TSP dengan algortima Brute Force.
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define R 6371
#define M_PI 3.14159265358979323846

struct Kota {
    char Nama[50];
    float Lattitude;
    float Longitude;
    struct Kota* next;
};

void insert(struct Kota** head, char* new_nama, float new_lattitude, float new_longitude) {
    struct Kota* new_Kota = (struct Kota*)malloc(sizeof(struct Kota));
    strcpy(new_Kota->Nama, new_nama);
    new_Kota->Lattitude = new_lattitude;
    new_Kota->Longitude = new_longitude;
    new_Kota->next = *head;
    *head = new_Kota;
}

void freeList(struct Kota* head) {
    struct Kota* tmp;
    while (head != NULL) {
        tmp = head;
        head = head->next;
        free(tmp);
    }
}

float distance(float lat1, float lon1, float lat2, float lon2) {
    float dlat = (lat2 - lat1) * M_PI / 180.0;
    float dlon = (lon2 - lon1) * M_PI / 180.0;
    float a = sin(dlat / 2) * sin(dlat / 2) + cos(lat1 * M_PI / 180.0) * cos(lat2 * M_PI / 180.0) * sin(dlon / 2) * sin(dlon / 2);
    float b = sqrt(a);
    float c = 2 * asin(b);
    return R * c;
}

void printPath(struct Kota* head, int path[], int n) {
    struct Kota* city;
    for (int i = 0; i < n; i++) {
        city = head;
        for (int j = 0; j < path[i]; j++) {
            city = city->next;
        }
        printf("%s -> ", city->Nama);
    }
    city = head;
    for (int j = 0; j < path[0]; j++) {
        city = city->next;
    }
    printf("%s\n", city->Nama);
}

void shortestdistance(struct Kota *head, int path[], int n, int index,clock_t start_time) {
    float minDist = INFINITY;
    int minPath[n];
    int c[n];

    clock_t current_time = clock();
    double elapsed_time = (double)(current_time - start_time) / CLOCKS_PER_SEC;

    if (elapsed_time > 60) {
        char response;
        printf("Execution has exceeded 1 minute. Do you want to continue? Y/N: ");
        scanf(" %c", &response);
        if (response == 'N' || response == 'n') {
            exit(0);
        }
        start_time = clock(); // Reset start time
    }

    for (int i = 0; i < n; i++) {
        c[i] = 0;
        path[i] = i;
    }

    int i = 1;
    while (i < n) {
        if (c[i] < i) {
            if (i % 2 == 0) {
                int temp = path[0];
                path[0] = path[i];
                path[i] = temp;
            } else {
                int temp = path[c[i]];
                path[c[i]] = path[i];
                path[i] = temp;
            }

            if (path[0] == index) {
                float currentDist = 0;
                for (int j = 0; j < n - 1; j++) {
                    struct Kota* city1 = head;
                    struct Kota* city2 = head;
                    for (int k = 0; k < path[j]; k++) {
                        city1 = city1->next;
                    }
                    for (int k = 0; k < path[j + 1]; k++) {
                        city2 = city2->next;
                    }
                    currentDist += distance(city1->Lattitude, city1->Longitude, city2->Lattitude, city2->Longitude);
                }
                struct Kota* city1 = head;
                struct Kota* city2 = head;
                for (int k = 0; k < path[n - 1]; k++) {
                    city1 = city1->next;
                }
                for (int k = 0; k < path[0]; k++) {
                    city2 = city2->next;
                }
                currentDist += distance(city1->Lattitude, city1->Longitude, city2->Lattitude, city2->Longitude);
                if (currentDist < minDist) {
                    minDist = currentDist;
                    memcpy(minPath, path, n * sizeof(int));
                }
            }

            c[i]++;
            i = 1;
        } else {
            c[i] = 0;
            i++;
        }
    }
    printf("Minimum distance: %.4f km\n", minDist);
    printPath(head, minPath, n);
}

int main() {
    char filename[50];
    char start[50];

    printf("Enter list of cities file name: ");
    scanf("%s", filename);

    struct Kota* head = NULL;
    FILE* file = fopen(filename, "r");

    if (file == NULL) {
        printf("Failed to open file %s. Make sure the file exists.\n", filename);
        return 1;
    }

    char line[100];
    while (fgets(line, sizeof(line), file)) {
        char nama[50];
        float Lattitude, Longitude;

        char* token = strtok(line, ",");
        strcpy(nama, token);

        token = strtok(NULL, ",");
        Lattitude = atof(token);

        token = strtok(NULL, ",");
        Longitude = atof(token);

        insert(&head, nama, Lattitude, Longitude);
    }
    fclose(file);

    printf("Enter starting point: ");
    scanf(" %[^\n]s", start);

    int n = 0;
    struct Kota* current = head;
    while (current != NULL) {
        n++;
        current = current->next;
    }

    int startIdx = -1;
    current = head;
    int idx = 0;
    while (current != NULL) {
        if (strcmp(current->Nama, start) == 0) {
            startIdx = idx;
            break;
        }
        idx++;
        current = current->next;
    }

    if (startIdx == -1) {
        printf("Starting point not found in the list.\n");
        freeList(head);
        return 1;
    }

    int path[n];

    clock_t start_time = clock();
    shortestdistance(head, path, n, startIdx,start_time);
    clock_t end_time = clock();

    printf("Time elapsed: %.6f seconds\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);

    freeList(head);
    return 0;
}

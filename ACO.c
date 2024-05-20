/** EL2208 Praktikum Pemecahan Masalah dengan C 2023/2024
*   Modul               : 09 (Tugas Besar)
*   Hari dan Tanggal    : 20/05/2024
*   Nama (NIM)          : Aira Ardistya A. (13222015)
*   Asisten (NIM)       : Nicholas Manuel Tjahjadi (18320012)
*   Nama file           : ACO.c
*   Deskripsi           : Tubes PPMC (No.2) - Ant Colony Optimization untuk Travelling Salesman Problem
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define M_PI 3.14159265358979323846 // Bilangan PI
#define MAX_CITIES 15               // Maks Kota
#define ALPHA 1.0                   // Konstanta Jarak
#define BETA 10.0                   // Konstanta Pheromone
#define EVAPORATION 0.5             // Konstanta Evaporasi
#define INITIAL_PHEROMONE 1.0       // Pheromone Awal
#define NUM_ANTS 225                // Banyak Semut (Total iterasi sebelum update pheromone)
#define MAX_ITERATIONS 100          // Banyak Pengulangan (Total Pengulangan mengupdate pheromone)

// Struktur untuk merepresentasikan jalur antara kota
typedef struct {
    float distance;
    float pheromone;
} Edge;

// Struktur untuk merepresentasikan kota dalam graf
typedef struct {
    char name[50];
    float longitude;
    float latitude;
    Edge edges[MAX_CITIES];
} City;

// Inisialisasi Array Kota Agar Memudahkan Akses
City cities[MAX_CITIES];
int numCities = 0;

// Kalkulasi Jarak Antarkota
float calculateDistance(float lat1, float lon1, float lat2, float lon2) {
    float lat = (lat2 - lat1) * M_PI / 180.0;
    float lon = (lon2 - lon1) * M_PI / 180.0;
    float X = sqrt((sin(lat/2) * sin(lat/2)) + cos(lat1 * M_PI / 180.0) * cos(lat2 * M_PI / 180.0) * sin(lon/2) * sin(lon/2));
    float Arcsine = asin(X);
    float d = 2 * 6371 * Arcsine;
    return d;
};

// Pengambilan Data Kota Dari CSV 
void readCities(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("File doesn't exist!\n");
        return;
    }

    char line[128];
    while (fgets(line, sizeof(line), file) && numCities < MAX_CITIES) {
        char* token = strtok(line, ",");
        strcpy(cities[numCities].name, token);
        token = strtok(NULL, ",");
        cities[numCities].latitude = atof(token);
        token = strtok(NULL, "\n");
        cities[numCities].longitude = atof(token);

        for (int i = 0; i < numCities; i++) {
            cities[numCities].edges[i].distance = calculateDistance(cities[numCities].latitude, cities[numCities].longitude, cities[i].latitude, cities[i].longitude);
            cities[i].edges[numCities].distance = cities[numCities].edges[i].distance;
            cities[numCities].edges[i].pheromone = INITIAL_PHEROMONE;
            cities[i].edges[numCities].pheromone = INITIAL_PHEROMONE;
        }
        numCities++;
    }
    fclose(file);
}

// Update pheromone
void updatePheromones(int tours[NUM_ANTS][MAX_CITIES], float tourLengths[NUM_ANTS]) {
    for (int i = 0; i < numCities; i++) {
        for (int j = 0; j < numCities; j++) {
            cities[i].edges[j].pheromone *= (1.0 - EVAPORATION);
        }
    }

    for (int ant = 0; ant < NUM_ANTS; ant++) {
        for (int i = 0; i < numCities; i++) {
            int from = tours[ant][i];
            int to = tours[ant][(i + 1) % numCities];
            cities[from].edges[to].pheromone += INITIAL_PHEROMONE / tourLengths[ant];
        }
    }
}

// Algoritma Utama Untuk Menyelesaikan Masalah Travelling Salesman
float ACO(int startCityIndex, int bestTour[MAX_CITIES]) {
    int tours[NUM_ANTS][MAX_CITIES];
    float tourLengths[NUM_ANTS];
    float bestLength = INFINITY;

    for (int iteration = 0; iteration < MAX_ITERATIONS; iteration++) {
        for (int ant = 0; ant < NUM_ANTS; ant++) {
            int tour[MAX_CITIES];
            int visited[MAX_CITIES] = {0};
            float tourLength = 0;

            int currentCity = startCityIndex;
            tour[0] = currentCity;
            visited[currentCity] = 1;

            for (int step = 1; step < numCities; step++) {
                float sumProbabilities = 0;
                float probabilities[MAX_CITIES] = {0};

                for (int nextCity = 0; nextCity < numCities; nextCity++) {
                    if (!visited[nextCity]) {
                        probabilities[nextCity] = pow(cities[currentCity].edges[nextCity].pheromone, BETA) *
                                                  (1/pow(cities[currentCity].edges[nextCity].distance, ALPHA));
                        sumProbabilities += probabilities[nextCity];
                    }
                }

                float random = (float)rand() / RAND_MAX * sumProbabilities;
                float cumulative = 0;

                for (int nextCity = 0; nextCity < numCities; nextCity++) {
                    if (!visited[nextCity]) {
                        cumulative += probabilities[nextCity];
                        if (cumulative >= random) {
                            currentCity = nextCity;
                            tour[step] = currentCity;
                            visited[currentCity] = 1;
                            tourLength += cities[tour[step - 1]].edges[currentCity].distance;
                            break;
                        }
                    }
                }
            }

            tourLength += cities[tour[numCities - 1]].edges[tour[0]].distance;
            memcpy(tours[ant], tour, sizeof(tour));
            tourLengths[ant] = tourLength;

            if (tourLength < bestLength) {
                bestLength = tourLength;
                memcpy(bestTour, tour, sizeof(tour));
            }
        }

        updatePheromones(tours, tourLengths);
    }

    return bestLength;
}

int main() {
    char filename[100];
    char startCityName[50];
    printf("Enter list of cities file name: ");
    scanf("%s", filename);
    printf("Enter starting point: ");
    scanf ("%s", startCityName);

    clock_t mulai = clock();
    readCities(filename);

    int startCityIndex = -1;
    for (int i = 0; i < numCities; i++) {
        if (strcmp(cities[i].name, startCityName) == 0) {
            startCityIndex = i;
            break;
        }
    }

    if (startCityIndex == -1) {
        printf("Starting city not found!\n");
        return 1;
    }

    srand(time(NULL));
    int bestTour[MAX_CITIES];
    float bestLength = ACO(startCityIndex, bestTour);

    printf("Best route found:\n");
    for (int i = 0; i < numCities; i++) {
        printf("%s -> ", cities[bestTour[i]].name);
    }
    printf("%s\n", cities[bestTour[0]].name);
    printf("Best route distance: %.10f km\n", bestLength);

    clock_t akhir = clock();
    printf("Time elapsed: %.10f s", (double)(akhir - mulai) / CLOCKS_PER_SEC);
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#import <time.h>

#define POPULATION_SIZE 2000
#define MAX_ITERATIONS 1000
#define INFINITY 999999

int startidx;
int i,j,k;

// Struktur koordinat kota
typedef struct {
    char name[50];
    float latitude;
    float longitude;
} Coordinate;

// fungsi insert pada list struct
void insert(Coordinate* cities, char* new_name, float new_latitude, float new_longitude) {
    strcpy(cities->name, new_name);
    cities->latitude = new_latitude;
    cities->longitude = new_longitude;
}

// perhitungan jarak dengan rumus haversine
double distance(Coordinate city1, Coordinate city2) {
    double lat1 = city1.latitude / 180.0 * M_PI; // Convert latitude to radians
    double lon1 = city1.longitude / 180.0 * M_PI; // Convert longitude to radians
    double lat2 = city2.latitude / 180.0 * M_PI; // Convert latitude to radians
    double lon2 = city2.longitude / 180.0 * M_PI; // Convert longitude to radians

    double dlon = lon2 - lon1;
    double dlat = lat2 - lat1;

    double a = pow(sin(dlat / 2), 2) + cos(lat1) * cos(lat2) * pow(sin(dlon / 2), 2);
    double c = 2 * asin(sqrt(a));

    return 6371 * c; // Radius of the Earth in kilometers
}

// Fungsi untuk menghitung panjang sirkit
double tourDistance(int tour[], Coordinate cities[], int dim) {
    double totalDistance = 0.0;
    for (i = 0; i < dim - 1; i++) {
        totalDistance += distance(cities[tour[i]], cities[tour[i + 1]]);
    }
    totalDistance += distance(cities[tour[dim - 1]], cities[tour[0]]); 
    return totalDistance;
}

// Fungsi PSO
void PSO_TSP(Coordinate cities[], int dim) {
    int population[POPULATION_SIZE][dim]; // Population of particles
    double pBest[POPULATION_SIZE]; // Personal best of each particle
    int pBestTour[POPULATION_SIZE][dim]; // Personal best tour of each particle
    double gBest = INFINITY; // Global best
    int gBestTour[dim]; // Global best tour
    double velocities[POPULATION_SIZE][dim]; // Velocities of particles

    // inisialisasi populasi dan velocity
    for (int i = 0; i < POPULATION_SIZE; i++) {
        for (int j = 0; j < dim; j++) {
            population[i][j] = j;
        }
        for (int j = 0; j < dim; j++) {
            int k = rand() % (j + 1);
            int temp = population[i][j];
            population[i][j] = population[i][k];
            population[i][k] = temp;
        }
        for (int j = 0; j < dim; j++) {
            velocities[i][j] = (double)rand() / RAND_MAX;
        }
        pBest[i] = INFINITY;
    }

    // Main loop
    for (int iter = 0; iter < MAX_ITERATIONS; iter++) {
        // mengupdate personal dan global best
        for (int i = 0; i < POPULATION_SIZE; i++) {
            double tourDist = tourDistance(population[i], cities, dim);
            if (tourDist < pBest[i]) {
                pBest[i] = tourDist;
                for (int j = 0; j < dim; j++) {
                    pBestTour[i][j] = population[i][j];
                }
            }
            if (tourDist < gBest) {
                gBest = tourDist;
                for (int j = 0; j < dim; j++) {
                    gBestTour[j] = population[i][j];
                }
            }
        }

        // mengupdate populasi dan velocity
        for (int i = 0; i < POPULATION_SIZE; i++) {
            double r1 = (double)rand() / RAND_MAX;
            double r2 = (double)rand() / RAND_MAX;
            // mengupdate velocity berdasarkan lintasan terbaik global dan personal
            for (int j = 0; j < dim; j++) {
                velocities[i][j] = velocities[i][j] + r1 * (pBestTour[i][j] - population[i][j]) + r2 * (gBestTour[j] - population[i][j]);
            }
            // membuat array visited agar semua kota dikunjungi sekali
            int visited[dim];
            for (int j = 0; j < dim; j++) {
                visited[j] = 0;
            }
            for (int j = 0; j < dim; j++) {
                // Mencari kota dengan velocity terdekat
                int minIdx = 0;
                double minDist = INFINITY;
                for (int k = 0; k < dim; k++) {
                    if (!visited[k]) {
                        double dist = fabs(velocities[i][j] - velocities[i][k]);
                        if (dist < minDist) {
                            minDist = dist;
                            minIdx = k;
                        }
                    }
                }
                population[i][j] = minIdx;
                visited[minIdx] = 1;
            }
        }
    }

    // Output dari fungsi
    printf("Best route found:\n");
    double totalDistance = 0.0;
    for (int i = 0; i < dim; i++ ){
        if (gBestTour[i] == startidx){
            startidx = i;
            break;
        }
    } 
    for (int i = startidx; i < dim; i++) {
        printf("%s -> ", cities[gBestTour[i]].name);
        if (i < dim - 1) {
            totalDistance += distance(cities[gBestTour[i]], cities[gBestTour[i + 1]]);
        }
    }
    for (int i = 0; i < startidx; i++) {
        printf("%s -> ", cities[gBestTour[i]].name);
        if (i < dim - 1) {
            totalDistance += distance(cities[gBestTour[i]], cities[gBestTour[i + 1]]);
        }
    }
    printf("%s\n", cities[gBestTour[startidx]].name); // Print the starting city to complete the tour
    totalDistance += distance(cities[gBestTour[dim - 1]], cities[gBestTour[0]]); // Connect back to starting city
    printf("Best route distance: %lf km\n", totalDistance);
}

int main() {
    int dim;
    char filename[20];

    printf("Enter list of cities file name: ");
    scanf("%s", filename);

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Failed to open file %s. Make sure the file exists.\n", filename);
        return 1;
    }

    // Menghitung jumlah kota pada array
    dim = 0;
    char line[100];
    while (fgets(line, sizeof(line), file)) {
        dim++;
    }
    fseek(file, 0, SEEK_SET); // Reset file pointer to beginning

    // Alokasi memori untuk kota baru
    Coordinate* cities = (Coordinate*)malloc(dim * sizeof(Coordinate));
    if (cities == NULL) {
        fclose(file);
        printf("Memory allocation failed.\n");
        return 1;
    }

    // Parsing file
    i = 0;
    while (fgets(line, sizeof(line), file)) {
        char name[50];
        float latitude, longitude;

        char* token = strtok(line, ",");
        strcpy(name, token);

        token = strtok(NULL, ",");
        latitude = atof(token);

        token = strtok(NULL, "");
        longitude = atof(token);

        insert(&cities[i], name, latitude, longitude);
        i++;
    }
    fclose(file);
    // Input kota awal
    char start[50]; 
    printf("Enter starting point: ");
    scanf("%s", start);
    for (i=0;i<dim;i++){
        if (strcmp(start, cities[i].name) == 0){
            startidx = i;
        }
    }
    long time1 = clock();
    // melakukan algoritpa PSO
    PSO_TSP(cities, dim);

    // membebaskan memori
    free(cities);
    // Menghitung waktu yang terlewat
    long time2 = clock();
    long dif = time2-time1;
    double elapsed = (double)dif/CLOCKS_PER_SEC;
    printf("time elapsed: %lf",elapsed);
    return 0;
}

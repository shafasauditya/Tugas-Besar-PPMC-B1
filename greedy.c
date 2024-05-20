/** Tugas Besar EL2208 Praktikum Pemecahan Masalah dengan C 2022/2023
 *   Hari dan Tanggal    : Rabu, 15 Mei 2024
 *   Nama (NIM)          : Aditya Ridwan Indrasta (13222017)
 *   Nama File           : Greedy.c
 *   Deskripsi           : Program untuk mencari total jarak minimum dengan persamaan Haversine dari beberapa kota yang terdapat pada 
 *                         file CSV menggunakan algoritma greedy
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define r_Bumi 6371.0 // Jari-jari Bumi dalam kilometer
#define pi 3.14159265358979323846 // Nilai pi

// Inisialisasi Struct
typedef struct {
    char name[50];
    double latitude;
    double longitude;
    int visited;
} Kota;

// Fungsi untuk menghitung jarak antara dua kota dengan persamaan Haversine
double hitungJarak(Kota kota1, Kota kota2) {
    double phi1 = kota1.latitude * pi / 180.0;
    double phi2 = kota2.latitude * pi / 180.0;
    double lambda1 = kota1.longitude * pi / 180.0;
    double lambda2 = kota2.longitude * pi / 180.0;
    double deltaPhi = phi2 - phi1;
    double deltaLambda = lambda2 - lambda1;
    double a = pow(sin(deltaPhi / 2), 2) + cos(phi1) * cos(phi2) * pow(sin(deltaLambda / 2), 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    return r_Bumi * c;
}

// Fungsi untuk mencari kota terdekat yang belum dikunjungi dari kota saat ini
int cariKotaTerdekat(Kota cities[], int numCities, int currentCityIndex) {
    int nearestCityIndex = -1;
    double jarak_min = INFINITY;
    for (int i = 0; i < numCities; ++i) {
        if (!cities[i].visited && i != currentCityIndex) {
            double jarak = hitungJarak(cities[currentCityIndex], cities[i]);
            // printf("Jarak antara kota %s dan kota %s: %.6lf km\n", cities[currentCityIndex].name, cities[i].name, jarak); // Digunakan untuk mengecek jarak antara kedua kota
            if (jarak < jarak_min) {
                jarak_min = jarak;
                nearestCityIndex = i;
            }
        }
    }
    // printf("Kota terdekat dari %s: %s\n", cities[currentCityIndex].name, cities[nearestCityIndex].name); // Digunakan untuk mengecek kebenaran algoritma greedy, yakni mengambil jarak terdekat
    return nearestCityIndex;
}

// Fungsi untuk mencari rute terbaik menggunakan Nearest Neighbor algorithm
void cariRute(Kota cities[], int numCities, char kotaAwal[]) {
    int startIndex = -1;
    for (int i = 0; i < numCities; ++i) {
        if (strcmp(cities[i].name, kotaAwal) == 0) {
            startIndex = i;
            break;
        }
    }
    if (startIndex == -1) {
        printf("Kota awal tidak ditemukan.\n");
        return;
    }

    cities[startIndex].visited = 1;
    printf("Best route:\n%s -> ", cities[startIndex].name);

    double totalJarak = 0.0;
    int currentCityIndex = startIndex;
    clock_t start = clock();

    for (int i = 0; i < numCities - 1; ++i) {
        int nearestCityIndex = cariKotaTerdekat(cities, numCities, currentCityIndex);
        printf("%s -> ", cities[nearestCityIndex].name);
        totalJarak += hitungJarak(cities[currentCityIndex], cities[nearestCityIndex]);
        cities[nearestCityIndex].visited = 1;
        currentCityIndex = nearestCityIndex;
    }

    totalJarak += hitungJarak(cities[currentCityIndex], cities[startIndex]);

    clock_t end = clock();
    double timeElapsed = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("%s\n", cities[startIndex].name);
    printf("Total distance: %.4lf km\n", totalJarak);
    printf("Time elapsed: %.4lf s\n", timeElapsed);
}

int main() {
    char filename[35];
    printf("Masukkan nama file: ");
    scanf("%s", filename);

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("File tidak ditemukan.\n");
        return 1;
    }

    int numCities = 0;
    Kota cities[100];

    // Baca nama-nama kota dari file
    while (fscanf(file, "%[^,],%lf,%lf\n", cities[numCities].name, &cities[numCities].latitude, &cities[numCities].longitude) == 3) {
        cities[numCities].visited = 0;
        numCities++;
    }
    fclose(file);

    if (numCities == 0) {
        printf("File kosong.\n");
        return 1;
    }

    char kotaAwal[50];
    printf("Masukkan kota awal: ");
    getchar(); // Membersihkan newline character yang tertinggal di buffer
    fgets(kotaAwal, sizeof(kotaAwal), stdin);
    kotaAwal[strcspn(kotaAwal, "\n")] = 0; // Menghapus newline character jika ada

    cariRute(cities, numCities, kotaAwal);

    return 0;
}
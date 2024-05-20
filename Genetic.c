#include<stdio.h>
#include<stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#define R 6371
#define M_PI 3.14159265358979323846

//Menentukan apakah nilai yang di-randomized unik dalam urutan
int isUnique(int *ptr, int banyakKota, int nilai){
    for(int i = 0; i < banyakKota; i++){
        if(*ptr == nilai){
            return 0;
        }
        ptr++;
    }
    return 1;
}

//Melakukan random populasi pertama sebanyak 50 kali
void randomPopulation(int *ptr, int banyakKota, int banyakRandom, int numKotaStart){
    srand(time(NULL));
    int tempPopulation[100][banyakKota+1];
    int temp;
    int *tempptr1;
    int *tempptr2;
    ptr++;
    tempptr1 = ptr;
    tempptr2 = ptr;
    for(int i = 0; i < banyakRandom; i++){
        tempptr2 = ptr;
        for(int j = 0; j < banyakKota; j++){
            tempptr1 = tempptr2;
            temp = rand()%(banyakKota+1);
            while(isUnique(tempptr1, banyakKota, temp) == 0 || temp == numKotaStart){
                tempptr1 = tempptr2;
                temp = rand()%(banyakKota+1);
            }
            *ptr = temp;
            ptr++;
        }
        ptr = ptr + 2;
    }

}

//Melakukan crossover
void crossover(int *array1, int *array2, int *arraytarget, int size){
    int counter = 0;
    int nilai1;
    int nilai2;
    int nilai3;
    int *arraytemp1;
    int *arraytemp2;
    int *arraytemptarget;
    arraytemptarget = arraytarget;
    arraytemp2 = array2;
    arraytemp1 = array1;

    arraytemp1 = arraytemp1 + 2;
    nilai1 = *arraytemp1;
    arraytemp1++;
    nilai2 = *arraytemp1;
    arraytemp1++;
    nilai3 = *arraytemp1;
    
    arraytemp2++;
    arraytemptarget++;


    for(int i = 1; i < size+1; i++){
        if(*arraytemp2 != nilai1 && *arraytemp2 != nilai2 && *arraytemp2 != nilai3){
            *arraytemptarget = *arraytemp2;
            arraytemptarget++;
        }
        arraytemp2++;
    }

    *arraytemptarget = nilai1;
    arraytemptarget++;
    *arraytemptarget = nilai2;
    arraytemptarget++;
    *arraytemptarget = nilai3;

    arraytemptarget = arraytarget;
}

//Mutasi jenis swap
//Menukar kota ke-2 dengan kota ke-4
void mutationSwap(int *arrayparent, int *arraytarget, int size){
    int *arrayparenttemp;
    int *arraytargettemp;
    int temp;
    int nilai1;
    int nilai2;
    arrayparenttemp = arrayparent;
    arraytargettemp = arraytarget;
    arrayparenttemp = arrayparenttemp + 2;
    nilai1 = *arrayparenttemp;
    arrayparenttemp = arrayparenttemp + 2;
    nilai2 = *arrayparenttemp;

    arrayparenttemp=arrayparent;

    for(int i = 1; i < size+1; i++){
        arrayparenttemp++;
        arraytargettemp++;
        if(i == 2){
            *arraytargettemp = nilai2;
        }
        else if(i == 4){
            *arraytargettemp = nilai1;
        }
        else{
            *arraytargettemp = *arrayparenttemp;
        }
    }
}

//Mutasi insertion
//Kota di ujung sebelum end point akan diubah menjadi kota 3 dan kota-kota lain digeser
void mutationInsertion(int *arrayparent, int *arraytarget, int size){
    int *arrayparenttemp;
    int *arraytargettemp;
    int nilai;
    arrayparenttemp = arrayparent;
    arraytargettemp = arraytarget;
    arrayparenttemp = arrayparenttemp + size;
    nilai = *arrayparenttemp;
    arrayparenttemp = arrayparent;
    arraytargettemp++;
    arrayparenttemp++;
    *arraytargettemp = *arrayparenttemp;
    arraytargettemp++;
    *arraytargettemp = nilai;

    for(int i = 3; i < size+1; i++){
        arraytargettemp++;
        arrayparenttemp++;
        *arraytargettemp = *arrayparenttemp;
    }
}


//Mutasi inversi
//Memutarbalikkan seluruh urutan kota
void mutationInversion(int *arrayparent, int *arraytarget, int size){
    int *arrayparenttemp;
    int *arraytargettemp;
    int nilai;
    arrayparenttemp = arrayparent;
    arraytargettemp = arraytarget;
    arrayparenttemp = arrayparenttemp + size;
    arraytargettemp++;
    for(int i = 1; i < size+1; i++){
        *arraytargettemp = *arrayparenttemp;
        arraytargettemp++;
        arrayparenttemp--;
    }
}

//Menghitung jarak antar kota
float distance(float lat1, float lon1, float lat2, float lon2) {
    float dlat = (lat2 - lat1) * M_PI / 180.0;
    float dlon = (lon2 - lon1) * M_PI / 180.0;
    float a = sin(dlat / 2) * sin(dlat / 2) +cos(lat1 * M_PI / 180.0) * cos(lat2 * M_PI / 180.0) * sin(dlon / 2) * sin(dlon / 2);
    float c = 2 * atan2(sqrt(a), sqrt(1 - a));
    return R * c;
}


//Menghitung jarak seluruh urutan kota dari suatu solusi
float fulldistance(float *ptrlokasi, int size, int *ptr){
    float lat1, lon1, lat2, lon2;
    int *tempptr;
    tempptr = ptr;
    int temp = *tempptr;
    float totaldistance = 0;

    
    for(int i = 0; i < size+2; i++){
        // Accessing the first point (lat1, lon1)
        lat1 = *(ptrlokasi + 2 * temp);
        lon1 = *(ptrlokasi + 2 * temp + 1);
        tempptr++;
        temp = *tempptr;
        // Accessing the next point (lat2, lon2)
    
        lat2 = *(ptrlokasi + 2 * temp);
        lon2 = *(ptrlokasi + 2 * temp + 1);

        // Calculate the distance and add to totaldistance
        totaldistance += distance(lat1, lon1, lat2, lon2);
    }

    return totaldistance;
}

//Copy 3 karakter terakhir agar mendapat format file yang diinput
void copy_last_three_chars(char *source, char *destination) {
    //Mencari panjang
    int len = strlen(source);

    //Menyalin 3 kata terakhir
    strncpy(destination, source + len - 3, 3);
    destination[3] = '\0'; // Ujung string
}

//Memilih mana yang akan dilanjutkan
void survivalFittest(int *ptr, int size, float *ptrlokasi){
    int *ptrawal;
    int *ptrtemp;
    float temp;
    int temp2;
    float *tempptrlokasi;
    int *tempptrawal;
    tempptrlokasi = ptrlokasi;
    tempptrawal = ptrawal;
    int temp3;
    float distancepersolusi[100][2];
    ptrawal = ptr;

    //Menghitung jarak dari seluruh solusi yang ada
    for(int i = 0; i < 100; i++){
        tempptrawal = ptrawal + i*(size+2);
        distancepersolusi[i][0] = fulldistance(tempptrlokasi, size, tempptrawal);
        temp = (float) i;
        distancepersolusi[i][1] = temp;
        tempptrlokasi = ptrlokasi;
    }

    //Bubble sort

    int swapped = 0;  // Untuk mengecek apakah ada pertukaran

    // Loop worst case harus diulang sebanyak seluruh panjang array-nya
    for (int i = 0; i < 100; i++) {  
        swapped = 0;  // Reset kondisi

        // Membandingkan komponen di sebelahnya dan menukarnya
        for (int j = 0; j < 99 - i; j++) {
            if (distancepersolusi[j][0] > distancepersolusi[j + 1][0]) { 

                temp = distancepersolusi[j][0];  // Tukar elemen
                distancepersolusi[j][0] = distancepersolusi[j + 1][0];
                distancepersolusi[j + 1][0] = temp;
                temp = distancepersolusi[j][1];  // Tukar elemen
                distancepersolusi[j][1] = distancepersolusi[j + 1][1];
                distancepersolusi[j + 1][1] = temp;
                swapped = 1;  // Catat bahwa terjadi sebuah pertukaran
            }
        }

        // Jika tidak ada pertukaran yang terjadi, maka sudah terurut
        if (swapped == 0) {
            break;
        }
    }

    //Menukar posisi solusi di array asli
    ptrawal = ptr;
    for(int i = 0; i < 50; i++){
        ptrtemp = ptr;
        temp2 = (int) distancepersolusi[i][1];
        ptrtemp = ptr + (size+2)*temp2;

        for(int k = 0; k < size+2; k++){
            *ptrawal = *ptrtemp;
            ptrawal++;
            ptrtemp++;
        }

    }
}

int main(){
    char filename[20];
    char start[20];
    char namaKota[15][100];
    float lokasiKota[15][2];
    int banyakKota = 0;
    char filetype[4];
    int numKotaStart;

    //Input nama file
    printf("Enter list of cities file name: ");
    scanf("%s", &filename);

    copy_last_three_chars(&filename[0], &filetype[0]);

    if(strcmp(filetype, "txt") != 0 && strcmp(filetype, "csv") != 0){
        printf("File %s bukan berformat .csv atau .txt", filename);
        return 0;
    }

    //Memasukkan isi file ke array
    FILE* file = fopen(filename, "r");

    if (file == NULL) {
        printf("Gagal membuka file %s. Pastikan file tersebut ada.\n", filename);
        return 0;
    }
    
    char line[100];
    while (fgets(line, sizeof(line), file)) {
        char nama[50];
        float Lattitude, Longitude;

        char* token = strtok(line, ",");
        strcpy(namaKota[banyakKota], token);

        token = strtok(NULL, ",");
        lokasiKota[banyakKota][0] = atof(token);

        token = strtok(NULL, "");
        lokasiKota[banyakKota][1] = atof(token);

        banyakKota++;
    }

    banyakKota--;

    //Input kota awal
    printf("Enter starting point: ");
    scanf("%s", &start);

    //Tutup file
    fclose(file);

    //Mulai menghitung waktu
    clock_t start1 = clock();

    //Menemukan Kota Start
    for(int i = 0; i<15; i++){
        if(strcmp(namaKota[i], start) == 0){
            numKotaStart = i;
            break;
        }
    }

    //Deklarasi populasi
    int population[100][banyakKota+2];

    // Inisialisasi isi array populasi
    for(int i = 0; i < 100; i++){
        for(int j = 0; j < banyakKota+2; j++){
            if(j == banyakKota+1 || j == 0){
                population[i][j] = numKotaStart;
            }
            else{
                population[i][j] = 16;
            }
        }
    }

    //Isi random 50 array awal
    randomPopulation(&population[0][0], banyakKota, 50, numKotaStart);

    //Mulai genetic algorithm untuk mencari solusi optimal
    int counter = 0;
    int randominteger1;
    int randominteger2;
    float samedistance = 0.0;
    srand(time(NULL));

    //Akan ada seleksi sebanyak 1000 kali
    for(int i = 0; i < 1000; i++){

        //Dari solusi 50 ke 100 akan dilakukan crossover dan mutasi
        for(int j = 50; j < 100; j++){

            //Dari solusi 50 ke 82 akan dilakukan crossover
            if(j >= 50 && j <82){

                //Random populasi dari 0 sampai 50 yang akan dicrossover
                randominteger1 = rand()%50;
                randominteger2 = rand()%50;

                //Pengecekan jika keduanya sama. Jika sama akan dicoba terus
                while(randominteger1 == randominteger2){
                    randominteger1 = rand()%50;
                    randominteger2 = rand()%50;
                }

                //Dilakukan crossover
                crossover(&population[randominteger1][0], &population[randominteger2][0], &population[j][0], banyakKota);
            }

            //Populasi 82 sampai 100 akan dilakukan mutasi
            else{
                //Random populasi dari 0 sampai 50 mana yang akan dimutasi
                randominteger1 = rand()%50;

                //Pembagian kasus dan dilakukan mutasi
                if(j%3 == 0){
                    mutationSwap(&population[randominteger1][0], &population[j][0], banyakKota);
                }
                else if(j%3 == 1){
                    mutationInsertion(&population[randominteger1][0], &population[j][0], banyakKota);
                }
                else{
                    mutationInversion(&population[randominteger1][0], &population[j][0], banyakKota);
                }
            }
        }

        //Menyeleksi solusi terbaik
        survivalFittest(&population[0][0], banyakKota, &lokasiKota[0][0]);

        //Jika jarak solusi terbaik di generasi sebelummya sama, maka counter akan naik
        if(fulldistance(&lokasiKota[0][0],banyakKota,&population[0][0]) == samedistance){
            counter++;
        }

        //Jika berbeda, maka counter di-reset dan samedistance akan diubah
        else{
            samedistance = fulldistance(&lokasiKota[0][0],banyakKota,&population[0][0]);
            counter = 0;
        }

        //Jika jarak solusi sama selama 20 generasi. Maka itu adalah solusi optimal dan iterasi selesai
        if(counter == 20){
            break;
        }
    }

    //Menghentikan penghitungan waktu
    clock_t end1 = clock();

    //Menghitung detik
    float seconds1 = (float)(end1 - start1) / CLOCKS_PER_SEC;

    //Print output
    printf("Best route found : \n");
    for(int i = 0; i<banyakKota+2; i++){
        printf("%s", namaKota[population[0][i]]);
        if(i != banyakKota+1){
            printf(" -> ");
        }
    }

    printf("\nBest route distance : %f\n", samedistance);
    printf("Time elapsed : %f\n", seconds1);

    //Selesai
    return 0;
}
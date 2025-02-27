#include <iostream>
#include <chrono>
#include <random>
#include <fstream>



int random(int* arr) {
    unsigned seed = 1001;
    std::default_random_engine rng(seed);
    std::uniform_int_distribution<unsigned>dstr(0, 9);
    return arr[dstr(rng)];
}


/*int random(int min, int max) {
    srand(time(NULL)); // генератор случайных чисел
    int num = min + rand() % (max - min + 1); // получение случайного числа

    return num;
}*/


int* generator(int N) {
    int* arr = new int[N];
    for (int i = 0; i < N; i++)
        arr[i] = i;

    return arr;
}


int finder_bin(int* begin, int* end, int& x) {
    int* l = begin - 1;
    int* r = end;
    while (l < r - 1) {
        int* m = l + (r - l) / 2;
        if (*m == x) {
            return *m;
        }
        if (*m < x) {
            l = m;
        } else {
            r = m;
        }
    }
    return -1;
}   

int main() {
    std::ofstream file("out13.txt");
    int* arr = new int[1000000];
    for (int k = 0; k < 1000000; k++)
        arr[k] = k;
    int x = -1; 
    for (int i = 100; i < 1000000; i += 100) {
        auto begin = std::chrono::steady_clock::now();
        for (int j = 1; j < 100001; j++) {
            finder_bin(arr, arr + i - 1, x);
        }
        auto end = std::chrono::steady_clock::now();
        auto time_span =
        std::chrono::duration_cast<std::chrono::nanoseconds>((end - begin)/100000);
        file << time_span.count() << std::endl;
    }
    delete arr;

    
    file.close();
    return 0;
}
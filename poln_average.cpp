#include <iostream>
#include <chrono>
#include <random>
#include <fstream>


int random(int* arr, int N) {
    unsigned seed = 1001;
    std::default_random_engine rng(seed);
    std::uniform_int_distribution<unsigned>dstr(0, N);

    return arr[dstr(rng)];
}


int* generator(int N) {
    int* arr = new int[N];
    for (int i {0}; i < N; i++)
        arr[i] = i;

    return arr;
}


bool finder_poln(int* arr, int N, int x) {
    for (int i = 0; i < N; i++)
        if (arr[i] == x)
            return true;

    return false;
}


 
int main() {
    std::ofstream file("out12.txt");
    for (int i = 1000; i < 1000000; i += 1000) {
        int* arr = generator(i);
        auto begin = std::chrono::steady_clock::now();
        for (int j = 0; j < 2000; j++) {
            int x;
            x = random(arr, i);
            finder_poln(arr, i, x);
        }
        auto end = std::chrono::steady_clock::now();
        auto time_span =
        std::chrono::duration_cast<std::chrono::nanoseconds>((end - begin)/2000);
        file << time_span.count() << std::endl;
        delete arr;
    }

    file.close();
    return 0;
}
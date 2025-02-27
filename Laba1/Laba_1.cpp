#include <chrono>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <random>
#include <string>

constexpr size_t kSizeStep = 50;
constexpr size_t kTestsCount = 50;
constexpr size_t kInnerCount = 100; 

size_t GetRandomIndex(size_t min, size_t max) {
    static std::random_device random_device;
    static std::mt19937 generator(random_device());
    std::uniform_int_distribution<size_t> distribution(min, max);

    
    return distribution(generator);
}

int* GenerateArray(size_t size, bool sorted) {
    int* array = new int[size];
    if (sorted) {
        for (size_t index = 0; index < size; ++index) {
            array[index] = index;
         }
    } else {
        for (size_t index = 0; index < size; ++index) {
            array[index] = GetRandomIndex(0, size);
        }
    }

    return array;
}

int* FinderBinarySearch(int* begin, int* end, int value) {
    int* left = begin;
    int* right = end;

    while (left != right) {
        int* middle = left + (right - left) / 2;

        if (*middle == value) {
            return middle;
        }

        if (*middle < value) {
            left = middle + 1;
        } else {
            right = middle;
        }
    }

    return nullptr;
}

int* FinderDummy(int* begin, int* end, int value) {
    for (int* current = begin; current != end; ++current) {
        if (*current == value) {
            return current;
        }
    }

    return nullptr;
}

std::pair<int*, int*> FinderSummOfTwo(int* begin, int* end, int summ) {
    for (int* Index_1 = begin; Index_1 != end; ++Index_1) {
        for (int* Index_2 = Index_1 + 1; Index_2 != end; ++Index_2) {
            if (*Index_1 + *Index_2 == summ) {
                return {Index_1, Index_2};
            }
        }
    }

    return {};
}


std::pair<int*, int*> FinderSummOfTwoSorted(int* begin, int* end, int summ){
    int* left = begin;
    int* right = end - 1;
    
    while(left != right){
        if (*left + *right == summ)
        {
            return {left, right};
        } else if (*left + *right > summ){
            right -= 1;
        } else if (*left + *right < summ){
            left += 1;
        }
    }

    return {};
} 

void TestAverage(const std::string& filename, int* finder(int*, int*, int), bool will_find, bool sorted, size_t tests_count = kTestsCount, size_t inner_count = kInnerCount, size_t size_step = kSizeStep) {
    std::ofstream file("Files_with_time/" + filename);

    for (size_t iter = 0; iter < tests_count; ++iter) {
        std::cerr << "\titer = " << iter << "\n";

        size_t size = size_step * (iter + 1);
        int* array = GenerateArray(size, sorted);
        int value = 0;

        auto begin = std::chrono::steady_clock::now();

        for (size_t _ = 0; _ < inner_count; ++_) {
            if (will_find) {
                value = GetRandomIndex(0, size);
            } else {
                value = -1;
            }

            finder(array, array + size, value);
        }

        auto end = std::chrono::steady_clock::now();
        auto time_span = std::chrono::duration_cast<std::chrono::nanoseconds>((end - begin) / inner_count);

        file << time_span.count() << "\n";

        delete[] array;
    }
}    

void TestAverageSumm(const std::string& filename, std::pair<int*, int*> finder(int*, int*, int), bool sorted, size_t tests_count = kTestsCount, size_t inner_count = kInnerCount, size_t size_step = kSizeStep){
    std::ofstream file("Files_with_time/" + filename);
    for (size_t iter = 0; iter < tests_count; ++iter) {
        std::cerr << "\titer = " << iter << "\n";

        size_t size = size_step * (iter + 1);
        int* array = GenerateArray(size, sorted);
        //int summ = 0;

        auto begin = std::chrono::steady_clock::now();

        for (size_t _ = 0; _ < inner_count; ++_) {
            finder(array, array + size, -1);
        }

        auto end = std::chrono::steady_clock::now();
        auto time_span = std::chrono::duration_cast<std::chrono::nanoseconds>((end - begin) / inner_count);

        file << time_span.count() << "\n";

        delete[] array;

    }
}

int main() {
    // std::cerr << "Dummy algorithm, not find:\n";
    // TestAverage("dummy_not-find.txt", FinderDummy, false, false);

    // std::cerr << "Dummy algorithm, will find:\n";
    // TestAverage("dummy_will-find.txt", FinderDummy, true, false);

    // std::cerr << "Binary search algorithm, not find:\n";
    // TestAverage("bs_not-find.txt", FinderBinarySearch, false, true);

    // std::cerr << "Binary search algorithm, will find:\n";
    // TestAverage("bs_will-find.txt", FinderBinarySearch, true, true);

    std::cerr << "Not sorted algoritm: \n";
    TestAverageSumm("not-sort-summ.txt", FinderSummOfTwo, false);
    
    std::cerr << "Sorted algoritm: \n";
    TestAverageSumm("sort-summ.txt", FinderSummOfTwoSorted, true);

    return 0;
}

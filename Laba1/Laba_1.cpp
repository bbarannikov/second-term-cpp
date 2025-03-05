#include <chrono>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <random>

const size_t kSizeStep = 1000;
const size_t kTestsCount = 1000;
const size_t kInnerCount = 200; 

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
    for (int* index_1 = begin; index_1 != end; ++index_1) {
        for (int* index_2 = index_1 + 1; index_2 != end; ++index_2) {
            if (*index_1 + *index_2 == summ) {
                return {index_1, index_2};
            }
        }
    }

    return {};
}


std::pair<int*, int*> FinderSummOfTwoSorted(int* begin, int* end, int summ){
    int* left = begin;
    int* right = end - 1;
    
    while(left != right){
        if (*left + *right == summ){
            return {left, right};
        } else if (*left + *right > summ){
            right -= 1;
        } else if (*left + *right < summ){
            left += 1;
        }
    }

    return {};
} 

int* FinderDummyStrategyA(int* begin, int* end, int value) {
    for (int* current = begin; current != end; ++current) {
        if (*current == value) {
            if(current != begin){
                std::swap (*current, *begin);
            }
            return current;
        }
    }

    return nullptr;
}

int* FinderDummyStrategyB(int* begin, int* end, int value) {
    for (int* current = begin; current != end; ++current) {
        if (*current == value) {
            if(current != begin){
                std::swap (*current, *(current - 1));
            }
            return current;
        }
    }

    return nullptr;
}

int* FinderDummyStrategyC(int* begin, int* end, int* count_begin, int* count_end, int value) {
    int* count_current = count_begin;
    for (int* current = begin; current != end; ++current) {
        if (*current == value) {
            if(current != begin and *(count_current) > *(count_current - 1)){
                std::swap (*current, *(current - 1));
                std::swap (*count_current, *(count_current - 1));
            }
            return current;
        }
        ++count_current;
    }

    return nullptr;
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

void TestStrategy(const std::string& filename, int* finder(int*, int*, int), bool will_find, bool sorted, size_t tests_count = kTestsCount, size_t inner_count = kInnerCount, size_t size_step = kSizeStep) {
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
                if (value > (int)size / 2){
                    value = size / 3;
                }
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

void TestStrategyC(const std::string& filename, int* finder(int*, int*, int*, int*, int), bool will_find, bool sorted, size_t tests_count = kTestsCount, size_t inner_count = kInnerCount, size_t size_step = kSizeStep) {
    std::ofstream file("Files_with_time/" + filename);

    for (size_t iter = 0; iter < tests_count; ++iter) {
        std::cerr << "\titer = " << iter << "\n";

        size_t size = size_step * (iter + 1);
        int* array = GenerateArray(size, sorted);
        int value = 0;

        auto begin = std::chrono::steady_clock::now();

        int* count = new int[size];
        for (size_t index = 0; index < size; ++index) {
             count[index] = 0;
        }
        for (size_t _ = 0; _ < inner_count; ++_) {
            if (will_find) {
                value = GetRandomIndex(0, size);
                if (value > (int)size / 2){
                    value = size / 3;
                }
            } else {
                value = -1;
            }

            finder(array, array + size, count, count + size, value);
        }

        auto end = std::chrono::steady_clock::now();
        auto time_span = std::chrono::duration_cast<std::chrono::nanoseconds>((end - begin) / inner_count);

        file << time_span.count() << "\n";

        delete[] count;
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

    // std::cerr << "Algoritm of find two in sorted array: \n";
    // TestAverageSumm("not-sort-summ.txt", FinderSummOfTwo, false);
    
    // std::cerr << "Algoritm of find two in not sorted array: \n";
    // TestAverageSumm("sort-summ.txt", FinderSummOfTwoSorted, true);

    std::cerr << "Strategy A: \n";
    TestStrategy("strat-A.txt", FinderDummyStrategyA, true, false);

    std::cerr << "Strategy B: \n";
    TestStrategy("strat-B.txt", FinderDummyStrategyB, true, false);

    std::cerr << "Strategy C: \n";
    TestStrategyC("strat-C.txt", FinderDummyStrategyC, true, false);
    return 0;
}

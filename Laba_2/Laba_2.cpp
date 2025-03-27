#include <chrono>
#include <fstream>
#include <iostream>
#include <random>

const size_t kSizeStep = 100;
const size_t kTestsCount = 100;
const size_t kInnerCount = 20; 
const float ReductionFactorCombSort = 1.25;
size_t counter = 0;

using std::swap;

// template <typename T>

// void swap(T& a, T& b) {
//     T temp = a;
//     a = b;
//     b = temp;
//     // ++counter;
// }

size_t GetRandomIndex(size_t min, size_t max) {
    static std::random_device random_device;
    static std::mt19937 generator(random_device());
    std::uniform_int_distribution<size_t> distribution(min, max);
    
    return distribution(generator);
}

int* GenerateArray(size_t size) {
    int* array = new int[size];
    for (size_t index = 0; index < size; ++index) {
        array[index] = GetRandomIndex(0, size);
    }

    return array;
}

void ArrayOutPut(int* array, size_t size) {
    for (size_t index = 0; index < size; ++index) {
        std::cout << *(array + index) << ' ';
    }
    std::cout << '\n';
}

void ForwardStep (int* array, size_t first_index, size_t second_index, bool& flag) {
    for (size_t index = first_index; index + 1 < second_index; ++index) {
        if (*(array + index) > *(array + index + 1)) {
            swap (*(array + index), *(array + index + 1));
            flag = true;
        }
    }
}

void BackwardStep (int* array, size_t first_index, size_t second_index, bool& flag) {
    for (size_t index = second_index; index > first_index; --index) {
        if (*(array + index) < *(array + index - 1)) {
            swap (*(array + index), *(array + index - 1));
            flag = true;
        }
    }
}

void ShakerSort (int* array, size_t size) {
    bool flag = true;
    for (size_t index = 0; index < size / 2; ++index) {
        flag = false;
        ForwardStep(array, index, size - index, flag);
        BackwardStep(array, index, size - index - 1, flag);
        if (!flag) {
            break;
        }
    }
}

void BubbleSortForward(int* array, size_t size){
    bool flag = true;
    for (size_t index = 0; index < size; ++index) {
        flag = false;
        ForwardStep(array, 0, size - index, flag);
        if (!flag) {
            break;
        }
    }
}

void BubbleSortBackward(int* array, size_t size){
    bool flag = true;
    for (size_t index = 0; index < size; ++index) {
        flag = false;
        BackwardStep(array, index, size, flag);
        if (!flag) {
            break;
        }
    }
}

void CombSort(int* array, size_t size) {
    bool flag = true;
    size_t step = size;

    while ((step > 1) or flag) {
        flag = false;
        
        if (step > 1){
            step /= ReductionFactorCombSort;
        }

        for (size_t index = 0; index + step < size; ++index) {
            if (*(array + index) > *(array + index + step)) {
                swap (*(array + index), *(array + index + step));

                flag = true;
            }
        }
    }
}

void ShellaSort(int* array, size_t size) {
    bool flag = true;
    size_t step = size;

    while ((step > 1) or flag) {
        flag = false;

        if (step > 1){
            step /= 2;
        }

        for(size_t index1 = step; index1 < size; ++index1) {
            for(size_t index2 = index1; index2 >= step && *(array + index2) < *(array + index2 - step) ; index2 -= step) {
                swap( *(array + index2), *(array +  index2 - step));
                flag = true;
            }
        }
    }
}

bool TestSort(int* array, size_t size) {
    for (size_t index = 1; index < size; ++index) {
        if (*(array + index - 1) > *(array + index)) {
            return false;
        }
    }
    return true;
}

void TestAverage (std::string filename, void sorter(int*, size_t), size_t tests_count = kTestsCount, size_t inner_count = kInnerCount, size_t size_step = kSizeStep) {
    std::ofstream file_1("Files_with_time/" + filename);
    // std::ofstream file_2("Files_with_changes/" + filename);

    for(size_t iter = 0; iter < tests_count; ++iter) {
        counter = 0;
        std::cout << "\titer = " << iter << "\n";

        size_t size = size_step * (iter + 1);

        auto begin = std::chrono::steady_clock::now();

        for(size_t _ = 0; _ < inner_count; ++_) {
            int* array = GenerateArray(size);
            sorter(array, size);
            delete array;
        }

        auto end = std::chrono::steady_clock::now();
        auto time_span = std::chrono::duration_cast<std::chrono::nanoseconds>((end - begin) / inner_count);

        file_1 << time_span.count() << "\n";
        // file_2 << counter / inner_count << "\n";
    }
}

int main() {
    // std::cout << "BubbleSortForward\n";
    // TestAverage("BubbleForward.txt", BubbleSortForward);

    // std::cout << "BubbleSortBackward\n";
    // TestAverage("BubbleBackward.txt", BubbleSortBackward);

    // std::cout << "ShakerSort\n";
    // TestAverage("Shaker.txt", ShakerSort);

    std::cout << "CombSort\n";
    TestAverage("Comb_1_25.txt", CombSort);

    // std::cout << "ShellaSort\n";
    // TestAverage("Shella.txt", ShellaSort);    

    // for (int index = 0; index < 1000; ++index) {
    //     int* array = GenerateArray(10000);
        
    //     // ArrayOutPut(array, 100);
    //     ShellaSort(array, 10000);
    //     // ArrayOutPut(array, 100);
        
    //     if (!TestSort(array, 10000)){
    //         std::cout << "Пиздец" << '\n';
    //         break;
    //     }

    //     std::cout << "\titer = " << index << '\n';
    //     delete array;
    // }

    return 0;
}
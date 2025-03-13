#include <iostream>

void Bubble_sort(int* array, size_t size){
    for (size_t index1 = 0; index1 < size; ++index1) {
        for (size_t index2 = 0; index2 < size - index1; ++index2){
            if (*(array + index2 + 1) < *(array + index2)) {
               std::swap(*(array + index2 + 1), *(array + index2));
            }
        }
    }
}

void Selection_sort (int* array, size_t size){
    for (size_t index1 = 0; index1 < size; ++index1) {
        size_t min = index1;
        for (size_t index2 = index1 + 1; index2 < size; ++index2) {
            if (*(array + min) > *(array + index2)) {
                min = index2;
            }
        }
        if (array + min != array + index1) {
            std::swap(*(array + min), *(array + index1));
        }
    }
}

void Insertion_sort (int* array, size_t size) {
    for (size_t index1 = 0; index1 < size; ++index1) {
        for (size_t index2 = index1 + 1; index2 > 0; --index2) {
            if (*(array + index2) < *(array + index2 - 1)) {
                std::swap(*(array + index2), *(array + index2 - 1));
            } else {
                break;
            }
        }
    }
}

void Merge(int* array, size_t middle, size_t size){
    size_t size_left = middle;  
    size_t size_right = size - middle;
    int* left_array = new int [size_left];
    int* right_array = new int [size_right];
    for (size_t index = 0; index < size_left; ++index) {
        *(left_array + index) = *(array + index);
    }
    for (size_t index = 0; index < size_right; ++index) {
        *(right_array + index) = *(array + middle + index);
    }

    size_t index1 = 0;
    size_t index2 = 0;
    size_t index = 0;
    while ((index1 != size_left) && (index2 != size_right)){
        if (*(left_array + index1) < *(right_array + index2)) {
            *(array + index++) = *(left_array + index1++);

        } else {
            *(array + index++) = *(right_array + index2++);
        }
    }
    if (index1 == size_left) {
        while(index2 < size_right) {
            *(array + index++) = *(right_array + index2++);
        }
    } else {
        while(index1 < size_left) {
            *(array + index++) = *(left_array + index1++);
        }
    }

    delete right_array;
    delete left_array;
}

void Merge_sort(int* array, size_t size) {
    size_t middle;    
    if(size != 1) {
        middle = size / 2;

        Merge_sort(array, middle);
        Merge_sort(array + middle, size - middle);

        Merge(array, middle, size);
    }
}

int main(){
    
}
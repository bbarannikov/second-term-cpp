#include <iostream>

void modifyArray(int (&arr)[5]) {
    for(int i = 0; i < 5; ++i) {
        arr[i] += 5;
    }
}

int main() {
    int arr[5] = {1, 2, 3};
    modifyArray(arr);
    for(auto it : arr) {
        std::cout << it << " ";
    }
    return 0;
}
#include <iostream>
#include <algorithm>
#include <vector>

// Function to find the median of three elements
int medianOfThree(int a, int b, int c) {
    if ((a > b) != (a > c)) return a;
    else if ((b > a) != (b > c)) return b;
    else return c;
}

// Hoare's partition scheme
int hoarePartition(std::vector<int>& arr, int low, int high) {
    int mid = low + (high - low) / 2;
    int pivot = medianOfThree(arr[low], arr[mid], arr[high]);
    int i = low - 1;
    int j = high + 1;

    while (true) {
        do {
            i++;
        } while (arr[i] < pivot);

        do {
            j--;
        } while (arr[j] > pivot);

        if (i >= j) return j;

        std::swap(arr[i], arr[j]);
    }
}

// Quicksort function
void quicksort(std::vector<int>& arr, int low, int high) {
    if (low < high) {
        int p = hoarePartition(arr, low, high);
        quicksort(arr, low, p);
        quicksort(arr, p + 1, high);
    }
}

int main() {
    std::vector<int> arr = {24, 8, 42, 75, 29, 77, 38, 57};
    quicksort(arr, 0, arr.size() - 1);

    std::cout << "Sorted array: ";
    for (int num : arr) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    return 0;
}
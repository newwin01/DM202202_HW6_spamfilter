#include <iostream>
#include <fstream>
#include <string>
using namespace std;

typedef struct {
    int count;
    string type; 
} data_feature;

void swap(data_feature* a, data_feature* b)
{
    data_feature temp;
    temp = *a;
    a->count = b->count;
    a->type = b->type;
    b->count = temp.count;
    b->type = temp.type;
}

int partition(data_feature arr[], int low, int high)
{
    int pivot = arr[high].count; 
    int i = (low- 1); 

  
    for (int j = low; j <= high - 1; j++) {
        if (arr[j].count < pivot) {
            i++; 
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quickSort(data_feature arr[], int low, int high)
{
    if (low < high) {
        int pi = partition(arr, low, high);
  
        // Separately sort elements before
        // partition and after partition
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void printArray(data_feature arr[], int size)
{
    int i;
    for (i = 0; i < size; i++)
        cout << arr[i].count << "/" <<arr[i].type << " ";
    cout << endl;
}

int main()
{
    data_feature arr[5];
    arr[0].count = 100;
    arr[0].type = "string";
    arr[1].count = 190;
    arr[1].type = "string";
    arr[2].count = 0;
    arr[2].type = "string";
    arr[3].count = 80;
    arr[3].type = "string";
    arr[4].count = 1100;
    arr[4].type = "string";

    int n = sizeof(arr) / sizeof(arr[0]);
    quickSort(arr, 0, n - 1);
    cout << "Sorted array: \n";
    printArray(arr, n);
    return 0;
}
  
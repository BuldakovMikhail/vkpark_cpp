#include "vector_operations.h"

int insertInSorted(std::vector<double> &arr, double value)
{
    for (int i = 0; i < arr.size(); ++i)
    {
        if (arr[i] > value)
        {
            arr.insert(arr.begin() + i, value);
            return i;
        }
    }

    arr.push_back(value);

    return arr.size() - 1;
}

void printVector(std::vector<std::string> vector)
{
    for (int i = 0; i < vector.size(); ++i)
        std::cout << vector[i] << std::endl;
}
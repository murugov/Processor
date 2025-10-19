#include <stdio.h>


template <typename T>
ssize_t BinSearch(T *arr, size_t size, T target)
{
    size_t left = 0;
    size_t right = size - 1;

    while (left <= right)
    {
        size_t mid = left + (right - left) / 2;

        if (arr[mid] == target)
            return (ssize_t)mid;

        if (arr[mid] < target)
            left = mid + 1;
        else
            right = mid - 1;
    }

    return -1;
}
#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <stdlib.h>

int main(void)
{
    int size = 8;
    int nums[] = {9,4,6,3,7,4,1,9};
    bool sorted = false;
    while (!sorted)
    {
        sorted = true;
        for (int i = 0; i < size-1; i++)
        {
            if (nums[i] > nums[i+1])
            {
                sorted = false;
                int nums_i = nums[i];
                nums[i] = nums[i+1];
                nums[i+1] = nums_i;
            }
        }
    }
    for (int i = 0; i < size; i++)
    {
        printf("%i\n", nums[i]);
    }
}
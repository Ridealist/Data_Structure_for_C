#include <stdio.h>
#include <stdlib.h>

int calculate_sum(int array[])
{
    int sum = 0;
    for (int i = 0; i < 10; i++)
        sum += *(array + i); // instead of array[i]
    return sum;
}

int main(void)
{
    // int sum, average;
    // int num[10];
    // for (int i = 0; i < 10; i++)
    //     scanf("%d", &num[i]);

    // sum = calculate_sum(num);

    // average = sum / 10;
    // printf("%d\n", average);
    // return 0;

    // int * array = malloc(4 * sizeof(int));
    int array[4];
    array[1] = 10;
    array[2] = 10;
    array[3] = 10;

    array[6] = 12;

    int * tmp = malloc(8 * sizeof(int));

    for (int i = 0; i < 4; i++)
        tmp[i] = array[i];

    
    // array = tmp;  // array 변수값에 tmp 변수값을 복사해줌 -> 배열의 첫번째 원소의 주소 덮어씌우기

    free(array);    
}
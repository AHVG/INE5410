#include <stdio.h>
#include <stdlib.h>

typedef struct MinMax
{
    int min;
    int max;
}MinMax;

MinMax *getMinMax(int *array, const int SIZE) {
    MinMax *minMax = (MinMax *) malloc(sizeof(MinMax));
    minMax->min = array[0];
    minMax->max = array[0];
    for (int i = 1; i < SIZE; i++) {
        if (array[i] < minMax->min) {
            minMax->min = array[i];
        }
        if (array[i] > minMax->max) {
            minMax->max = array[i];
        }
    }
    return minMax;
}

int main()
{
    int valores[4] = {1, 3, 980, -3};
    MinMax *minMax = getMinMax(valores, 4);
    printf("min=%d max=%d", minMax->min, minMax->max);
    free(minMax);
    return 0;
}

/******************************************************************************

Welcome to GDB Online.
  GDB online is an online compiler and debugger tool for C, C++, Python, PHP, Ruby,
  C#, OCaml, VB, Perl, Swift, Prolog, Javascript, Pascal, COBOL, HTML, CSS, JS
  Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 100

// Lê os elementos do teclado e os coloca em arr. Size possui o tamanho do vetor
void inputArray(int *arr, int size) {
    int arr_l = 0;
    for (int i = 0; i < size; i++) {
        scanf("%d", &arr[arr_l]);
        arr_l++;
    }

}

// Imprime o conteúdo do vetor arr com tamanho size
void printArray(int * arr, int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
}

// Função que compara dois inteiros. A função retorna a diferença entre o primeiro e o
// segundo
int sortAscending(int * num1, int * num2) {
    return *num1 - *num2;
}

// Função que compara dois inteiros. A função retorna a diferença entre o segundo e o
// primeiro
int sortDescending(int * num1, int * num2) {
    return *num2 - *num1;
}

// Função que realizada a ordenação. O terceiro argumento é um ponteiro para função que
// realiza a comparação entre dois inteiros do vetor (funções sortAscending ou
// sortDescending)
void sort(int * arr, int size, int (* compare)(int *, int*)) {
    for (int i = 1; i < size; i++) {
        int curr = arr[i];
        int j = i - 1;
        while (j >= 0 && compare(&curr, &arr[j]) < 0) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j+1] = curr;
    }
}

int main()
{
    int arr[MAX_SIZE];
    int size;
    /*
    * Input array size and elements.
    */
    printf("Enter array size: ");
    scanf("%d", &size);
    printf("Enter elements in array: ");
    inputArray(arr, size);
    printf("\n\nElements before sorting: ");
    printArray(arr, size);
    // Sort and print sorted array in ascending order.
    printf("\n\nArray in ascending order: ");
    sort(arr, size, sortAscending);
    printArray(arr, size);
    // Sort and print sorted array in descending order.
    printf("\nArray in descending order: ");
    sort(arr, size, sortDescending);
    printArray(arr, size);
    return 0;
}

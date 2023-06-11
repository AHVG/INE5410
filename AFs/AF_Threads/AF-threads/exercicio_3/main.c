#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <pthread.h>

// Lê o conteúdo do arquivo filename e retorna um vetor E o tamanho dele
// Se filename for da forma "gen:%d", gera um vetor aleatório com %d elementos
//
// +-------> retorno da função, ponteiro para vetor malloc()ado e preenchido
// | 
// |         tamanho do vetor (usado <-----+
// |         como 2o retorno)              |
// v                                       v
double* load_vector(const char* filename, int* out_size);


// Avalia se o prod_escalar é o produto escalar dos vetores a e b. Assume-se
// que ambos a e b sejam vetores de tamanho size.
void avaliar(double* a, double* b, int size, double prod_escalar);

typedef struct {
    int start_idx;
    int qnt;
    double* a;
    double* b;
    double partial_result;
} ThreadArgs;

void* thread(void* args) {
    double partial_result = 0;
    ThreadArgs* thread_args = (ThreadArgs*) args;
    for (int i = 0; i < thread_args->qnt; i++) {
        int c_idx = i + thread_args->start_idx;
        partial_result += thread_args->a[c_idx] * thread_args->b[c_idx];
    }
    thread_args->partial_result = partial_result;
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    srand(time(NULL));

    //Temos argumentos suficientes?
    if(argc < 4) {
        printf("Uso: %s n_threads a_file b_file\n"
               "    n_threads    número de threads a serem usadas na computação\n"
               "    *_file       caminho de arquivo ou uma expressão com a forma gen:N,\n"
               "                 representando um vetor aleatório de tamanho N\n", 
               argv[0]);
        return 1;
    }
  
    //Quantas threads?
    int n_threads = atoi(argv[1]);
    if (!n_threads) {
        printf("Número de threads deve ser > 0\n");
        return 1;
    }
    //Lê números de arquivos para vetores alocados com malloc
    int a_size = 0, b_size = 0;
    double* a = load_vector(argv[2], &a_size);
    if (!a) {
        //load_vector não conseguiu abrir o arquivo
        printf("Erro ao ler arquivo %s\n", argv[2]);
        return 1;
    }
    double* b = load_vector(argv[3], &b_size);
    if (!b) {
        printf("Erro ao ler arquivo %s\n", argv[3]);
        return 1;
    }
    
    //Garante que entradas são compatíveis
    if (a_size != b_size) {
        printf("Vetores a e b tem tamanhos diferentes! (%d != %d)\n", a_size, b_size);
        return 1;
    }

    //Calcula produto escalar. Paralelize essa parte
    if (n_threads > a_size) 
        n_threads = a_size;

    pthread_t* threads = (pthread_t*) malloc(n_threads*sizeof(pthread_t));
    if (!threads) 
        return 1;
    double items_per_thread = (double) a_size / (double) n_threads;
    ThreadArgs* args = (ThreadArgs*) malloc(n_threads*sizeof(ThreadArgs));

    for (int i = 0; i < n_threads; i++) {
        int previous_end = items_per_thread * i;
        int current_end = items_per_thread * (i+1);
        int item_qnt = current_end - previous_end;
        args[i].start_idx = previous_end;
        args[i].qnt = item_qnt;
        args[i].a = a;
        args[i].b = b;
        pthread_create(&threads[i], NULL, thread, (void*) &args[i]);
    }

    double result = 0;
    for (int i = 0; i < n_threads; i++) {
        pthread_join(threads[i], NULL);
        result += args[i].partial_result;
    }
    
    //    +---------------------------------+
    // ** | IMPORTANTE: avalia o resultado! | **
    //    +---------------------------------+
    avaliar(a, b, a_size, result);

    //Libera memória
    free(a);
    free(b);
    free(threads);
    free(args);

    return 0;
}

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>


typedef struct {
    double *inicio_a;
    double *inicio_b;
    double *inicio_c;
    int tam;
} Info;

// Lê o conteúdo do arquivo filename e retorna um vetor E o tamanho dele
// Se filename for da forma "gen:%d", gera um vetor aleatório com %d elementos
//
// +-------> retorno da função, ponteiro para vetor malloc()ado e preenchido
// | 
// |         tamanho do vetor (usado <-----+
// |         como 2o retorno)              |
// v                                       v
double* load_vector(const char* filename, int* out_size);


// Avalia o resultado no vetor c. Assume-se que todos os ponteiros (a, b, e c)
// tenham tamanho size.
void avaliar(double* a, double* b, double* c, int size);

void* thread(void* arg) {
    Info *info = (Info *)arg;
    for (int  i = 0; i < info->tam; i++)
        info->inicio_c[i] = info->inicio_a[i] + info->inicio_b[i];
    return 0;
}

int main(int argc, char* argv[]) {
    // Gera um resultado diferente a cada execução do programa
    // Se **para fins de teste** quiser gerar sempre o mesmo valor
    // descomente o srand(0)
    srand(time(NULL)); //valores diferentes
    //srand(0);        //sempre mesmo valor

    //Temos argumentos suficientes?
    if(argc < 4) {
        printf("Uso: %s n_threads a_file b_file\n"
               "    n_threads    número de threads a serem usadas na computação\n"
               "    *_file       caminho de arquivo ou uma expressão com a forma gen:N,\n"
               "                 representando um vetor aleatório de tamanho N\n",
               argv[0]);
        fflush(stdout);
        return 1;
    }
  
    //Quantas threads?
    int n_threads = atoi(argv[1]);
    if (!n_threads) {
        printf("Número de threads deve ser > 0\n");
        fflush(stdout);
        return 1;
    }
    //Lê números de arquivos para vetores alocados com malloc
    int a_size = 0, b_size = 0;
    double* a = load_vector(argv[2], &a_size);
    if (!a) {
        //load_vector não conseguiu abrir o arquivo
        printf("Erro ao ler arquivo %s\n", argv[2]);
        fflush(stdout);
        return 1;
    }
    double* b = load_vector(argv[3], &b_size);
    if (!b) {
        printf("Erro ao ler arquivo %s\n", argv[3]);
        fflush(stdout);
        return 1;
    }
    
    //Garante que entradas são compatíveis
    if (a_size != b_size) {
        printf("Vetores a e b tem tamanhos diferentes! (%d != %d)\n", a_size, b_size);
        fflush(stdout);
        return 1;
    }
    //Cria vetor do resultado 
    double* c = malloc(a_size*sizeof(double));

    // Calcula com uma thread só. Programador original só deixou a leitura 
    // do argumento e fugiu pro caribe. É essa computação que você precisa 
    // paralelizar

    clock_t start, end;

    start = clock();

    for (int i = 0; i < a_size; ++i) 
        c[i] = a[i] + b[i];
    
    end = clock();

    // Calculo paralelo

    clock_t start_threads, end_threads;

    start_threads = clock();


    if (a_size < n_threads)
        n_threads = a_size;

    pthread_t *threads = (pthread_t *) malloc(n_threads * sizeof(pthread_t));
    if (!threads)
        return 1;

    int n_loops_per_threads = a_size / n_threads;
    int resto = a_size % n_threads;

    double *inicio_a = a;
    double *inicio_b = b;
    double *inicio_c = c;

    Info *infos = (Info *)malloc(n_threads * sizeof(Info));
    if (!infos)
        return 1;
    for (int i = 0; i < n_threads; i++) {

        int adicional = 0;
        if (resto > 0) {
            adicional = 1;
            resto--;
        }
        int loops = adicional + n_loops_per_threads;
        infos[i].inicio_a = inicio_a;
        infos[i].inicio_b = inicio_b;
        infos[i].inicio_c = inicio_c;
        infos[i].tam = loops;
        inicio_a = &inicio_a[loops - 1];
        inicio_b = &inicio_b[loops - 1];
        inicio_c = &inicio_c[loops - 1];
        pthread_create(&threads[i], NULL, thread, (void *)(&infos[i]));
    }

    for (int i = 0; i < n_threads; i++)
        pthread_join(threads[i], NULL);

    end_threads = clock();

    //    +---------------------------------+
    // ** | IMPORTANTE: avalia o resultado! | **
    //    +---------------------------------+

    avaliar(a, b, c, a_size);

    double exe_time = (double) (end - start);
    double exe_time_threads = (double) (end_threads - start_threads);
    double speed_up = (double) (exe_time / exe_time_threads);
    printf("Tempo de execução sem threads (ms): %f\n", exe_time);
    printf("Tempo de execução com threads (ms): %f\n", exe_time_threads);
    printf("Speed up: %f\n",  speed_up);

    //Importante: libera memória
    free(a);
    free(b);
    free(c);
    free(threads);
    free(infos);

    return 0;
}

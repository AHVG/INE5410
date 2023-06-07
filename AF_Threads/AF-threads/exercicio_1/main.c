#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <pthread.h>

//                 (main)      
//                    |
//    +----------+----+------------+
//    |          |                 |   
// worker_1   worker_2   ....   worker_n


// ~~~ argumentos (argc, argv) ~~~
// ./program n_threads

// ~~~ printfs  ~~~
// main thread (após término das threads filhas): "Contador: %d\n"
// main thread (após término das threads filhas): "Esperado: %d\n"

// Obs:
// - pai deve criar n_threds (argv[1]) worker threads 
// - cada thread deve incrementar contador_global (operador ++) n_loops vezes
// - pai deve esperar pelas worker threads  antes de imprimir!

int contador_global = 0;

void* thread(void* arg) {
    for (int i = 0; i < *((int *)arg); i++)
        contador_global++;
    return 0;
}


int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("n_threads é obrigatório!\n");
        fflush(stdout);
        printf("Uso: %s n_threads n_loops\n", argv[0]);
        fflush(stdout);
        return 1;
    }

    int n_threads = atoi(argv[1]);
    int n_loops = atoi(argv[2]);

    pthread_t *threads = (pthread_t *) malloc(n_threads * sizeof(pthread_t));
    if (!threads) {
        printf("Impossível criar %d threads!\n", n_threads);
        fflush(stdout);
        return 1;
    }

    for (int i = 0; i < n_threads; i++)
        pthread_create(&threads[i], NULL, thread, (void *)&n_loops);

    for (int i = 0; i < n_threads; i++)
        pthread_join(threads[i], NULL);

    free(threads);
    
    printf("Contador: %d\n", contador_global);
    fflush(stdout);
    printf("Esperado: %d\n", n_threads*n_loops);
    fflush(stdout);
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<sys/wait.h>

//                          (principal)
//                               |
//              +----------------+--------------+
//              |                               |
//           filho_1                         filho_2
//              |                               |
//    +---------+-----------+          +--------+--------+
//    |         |           |          |        |        |
// neto_1_1  neto_1_2  neto_1_3     neto_2_1 neto_2_2 neto_2_3

// ~~~ printfs  ~~~
//      principal (ao finalizar): "Processo principal %d finalizado\n"
// filhos e netos (ao finalizar): "Processo %d finalizado\n"
//    filhos e netos (ao inciar): "Processo %d, filho de %d\n"

// Obs:
// - netos devem esperar 5 segundos antes de imprmir a mensagem de finalizado (e terminar)
// - pais devem esperar pelos seu descendentes diretos antes de terminar

void processoNeto() {
    printf("Processo %d, filho de %d\n", getpid(), getppid());
    fflush(stdout);
    sleep(5);
    printf("Processo %d finalizado\n", getpid());
    fflush(stdout);
    exit(0);
}

void processoFilho() {
    printf("Processo %d, filho de %d\n", getpid(), getppid());
    fflush(stdout);
    for (int i = 0; i < 3; i++)
        if (!fork())
            processoNeto();
    while (wait(NULL) >= 0);
    printf("Processo %d finalizado\n", getpid());
    fflush(stdout);
    exit(0);
}

void processoPai() {
    for (int i = 0; i < 2; i++)
        if (!fork())
            processoFilho();
    while (wait(NULL) >= 0);
    printf("Processo principal %d finalizado\n", getpid());
    fflush(stdout);
}


int main(int argc, char** argv) {


    /*************************************************
     * Dicas:                                        *
     * 1. Leia as intruções antes do main().         *
     * 2. Faça os prints exatamente como solicitado. *
     * 3. Espere o término dos filhos                *
     *************************************************/

    processoPai();
    
    return 0;
}

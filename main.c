#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "arvoreb.h"

int main(int argc, char *argv[]) {

    raiz = criarArv();
    
    int escolha;
    do
    {
        menu(&escolha);
    } while (escolha != 1);
    
    apagar(raiz);

    return 0;

}
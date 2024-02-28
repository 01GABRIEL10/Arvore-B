#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdlib.h>

#include "arvoreb.h"

#define METADE ORDEM/2

#pragma region Inicialização e Informações

Arvb *criarArv(){ // Inicializa a Árvore e retorna ela
    Arvb *aux;
    aux=(Arvb*)malloc(sizeof(Arvb));
    aux->tamanho=0;
    return aux;
}

int vaziaOrdem(Arvb *arv){ // Retorna se a Árvore(Página) existe (true || false)
    return arv==NULL;
}

int vazia(Arvb *arv){ // Retorna se a Árvore(Página) está vazia (true || false)
    return arv->tamanho==0;
}

int cheia(Arvb *arv){ // // Retorna se a Árvore(Página) está cheia (true || false)
    return arv->tamanho>=ORDEM-1;
}

int tamanho(Arvb *arv){ // Retorna o tamanho da Árvore
    return arv->tamanho;
}

#pragma endregion

#pragma region Funções de Inserção

void dividir(Arvb *origem, Arvb *destino){ // Recebe a página e divide os elementos na metade com outra página
    int i = METADE+1;
    for(; i<ORDEM-1; i++){
        destino->filhos[(destino->tamanho)] = origem->filhos[i];
        origem->filhos[i] = NULL;
        destino->itens[(destino->tamanho)++] = origem->itens[i];
    }
    destino->filhos[(destino->tamanho)] = origem->filhos[i];
    origem->tamanho = ORDEM-METADE-1;
}

void swap(int *a,int *b){
    int c = *a;
    *a = *b;
    *b = c;
}
 
void inserirOrd(Arvb *arv,int valor){ // Insere os itens de forma ordenada
    int status = 0; // Informa se ja foi feita a troca 
    int guardado = 0; // Guarda o número
    if (vazia(arv)){
        arv->itens[arv->tamanho] = valor;
    } else {
        for (int i = 0; i<=arv->tamanho; i++) {
            if (status == 0) {
                if (valor==arv->itens[i]) {
                    printf("itens iguais:%d\n",valor);
                    arv->tamanho--;
                    break;
                }
                if (valor < arv->itens[i]) {
                    guardado = arv->itens[i];
                    arv->itens[i] = valor;
                    status = 1;
                } else if (i == arv->tamanho) {
                    arv->itens[i] = valor;
                }
            }else{
                swap(&guardado,&(arv->itens[i]));
            }
        }
    }
    arv->tamanho++;
}

int buscabinariaFilho(Arvb *arv, Arvb *filho, int esq, int dir){
    if (esq == dir) {
        if(arv->filhos[esq]->itens[0] == filho->itens[0])
            return esq;
        else
            return -1;
    } else {
        int meio = (esq + dir)/2;
        if (arv->filhos[meio]->itens[0] < filho->itens[0]){
            return buscabinariaFilho(arv, filho, meio+1, dir);
        } else {
            return buscabinariaFilho(arv, filho, esq, meio);
        }
    }
}

int buscaProximo(Arvb *arv, int item , int esq, int dir){
    if (esq == dir) {
        if(arv->itens[esq] > item)
            return esq;
        else
            return esq+1;
    } else {
        int meio = (esq + dir)/2;
        if(arv->itens[meio] > item)
            return buscaProximo(arv, item, esq, meio);
        else
            return buscaProximo(arv, item, meio+1, dir);
    }
}

int numeroFilho(Arvb *pai, Arvb *filho){ // Retorna a posição do filho em relação ao pai
    return buscabinariaFilho(pai, filho, 0, pai->tamanho);
}

void inserirFilho(Arvb *pai, Arvb *filho, int lugar){ // Insere um novo filho
    Arvb *aux, *aux1 = filho;
    for (int i = lugar+1; i <= pai->tamanho; i++){
        aux = pai->filhos[i];
        pai->filhos[i] = aux1;
        aux1 = aux;
    }
}

void split(Arvb *arv, Arvb *pai){ // Separa os itens da Árvore(Página) e reoganiza
    if(arv == raiz){
        Arvb *encima = criarArv(), *lado = criarArv();
        inserirOrd(encima, arv->itens[(METADE)]);
        dividir(arv, lado);
        raiz = encima;
        raiz->filhos[0] = arv;
        raiz->filhos[1] = lado;
    }else{
        Arvb *lado = criarArv();
        inserirOrd(pai, arv->itens[(METADE)]);
        dividir(arv, lado);
        inserirFilho(pai, lado, numeroFilho(pai, arv));
    }
}

void inserir(Arvb *arv, Arvb *pai, int valor){ // Inserir um item na Árvore(Página)
   if (vaziaOrdem(arv)) {
        printf("nao inicializada\n");
   } else {
        if (vaziaOrdem(arv->filhos[0])) { // Caso não possui filhos, indica que a Árvore(Página) é uma folha, então inserimos
            inserirOrd(arv, valor);
        } else { // Caso possua, procuramos aonde inserir
            int i = buscaProximo(arv, valor, 0, arv->tamanho-1);
            inserir(arv->filhos[i], arv, valor);
        }
        if (cheia(arv)) { // Se a Árvore(Página) está cheia, efetuamos o split
            split(arv, pai);
        }
   }
}

#pragma endregion

#pragma region Funções de Pesquisa

int buscabinaria(int *arv, int valor, int esq, int dir) {
    if (esq == dir) {
        if (arv[esq] == valor) {
            return esq;
        } else {
            return -1;
        }
    } else {
        int meio = (esq + dir)/2;
        if(arv[meio] > valor)
            return buscabinaria(arv, valor, esq, meio-1);
        else
            return buscabinaria(arv, valor, meio, dir);
    }
}

int possui(Arvb *arv,int valor){
    return buscabinaria(arv->itens, valor, 0, arv->tamanho-1)!=-1?1:0;
}

Arvb *pesquisa(Arvb *arv, int valor){ // Retorna a Árvore(Página) onde se encontra o item
    if (vaziaOrdem(arv)) {
        printf("nao iniciada\n");
        exit(1);
    } else if (vazia(arv)) {
        printf("sem elementos\n");
        exit(1);
    } else if (possui(arv, valor)) {
        printf("possui na lista\n");
        return arv;
    } else if (vaziaOrdem(arv->filhos[0])) {
        printf("nao possui na lista\n");
        return NULL;
    } else {
        int i = buscaProximo(arv, valor, 0, arv->tamanho-1);
        pesquisa(arv->filhos[i], valor);
    }
}

#pragma endregion

#pragma region Funções de Remoção

void adicionarPai(Arvb *pai, Arvb *filho, int local) { // Remove os itens da Página pai e insere na Página filha
    filho->itens[filho->tamanho++] = pai->itens[local];
    for (int i = local; i < pai->tamanho-1; i++) {
        pai->itens[i] = pai->itens[i+1];
        pai->filhos[i+1] = pai->filhos[i+2];
    }
    pai->tamanho--;
}

int suporta(Arvb *a, Arvb *b){ // Retorna se suporta adicionar uma Página B na Página A e um item da Página pai
    return tamanho(a) + tamanho(b) < ORDEM-2;
}

int metade(Arvb *arv){ // Retorna se está abaixo da capacidade mínima
    return arv->tamanho < METADE;
}

void unir(Arvb *a,Arvb *b){ // Une a Página A com a B e remove a B
    printf("iniciada a uniao\n");
    if (vaziaOrdem(a) || vaziaOrdem(b)) {
        printf("nao pode unir\n");
        exit(0);
    }
    int i = a->tamanho, j = 0;
    for(; i<(a->tamanho + b->tamanho + 1); i++) {
        a->itens[i] = b->itens[j];
        a->filhos[i] = b->filhos[j++];
    }
    a->tamanho += b->tamanho;
    b->tamanho = 0;
}

void underSplit(Arvb *arv, Arvb *pai) { // Faz o inverso do split
    int local = numeroFilho(pai, arv); // Descobre a localização do filho na Página pai
    if (local < tamanho(pai)-1 && local != 0) {
        /*verifica se de acordo com a posicão a pagina vai ter 
        vizinhos tanto na esquerda quanto na direita,caso seja maior que 0
        e menor do que o tamanho ela possui dos dois vizinhos*/
        if (suporta(arv, pai->filhos[local-1])) {
            Arvb *aux = pai->filhos[local-1];
            adicionarPai(pai, pai->filhos[local-1], local-1);
            unir(aux, arv);
            if (vazia(raiz)) {//caso a raiz fique vazia muda a raiz para o filho
                free(raiz);
                raiz = aux;
            }
        }
        if (suporta(arv, pai->filhos[local+1])) {
            Arvb *aux = pai->filhos[local+1];
            adicionarPai(pai, arv, local);
            unir(arv, aux);
            if (vazia(raiz)) {
                free(raiz);
                raiz = arv;
            }
        }
    }
    else if (local == 0) { // Caso seja o 0, só possui o vizinho da direita
        if (suporta(arv,pai->filhos[local+1])) {
            Arvb *aux = pai->filhos[local+1];
            adicionarPai(pai, arv, local);
            unir(arv, aux);
            if (cheia(arv))
                split(arv, pai);
            }
            if (vazia(raiz)){
                free(raiz);
                raiz = arv;
            }
    } else { // Caso seja o ultimo so possui o vizinho da esquerda
        if (suporta(arv, pai->filhos[local-1])) {
            Arvb *aux = pai->filhos[local-1];
            adicionarPai(pai, pai->filhos[local-1], local-1);
            unir(aux, arv);
            if (vazia(raiz)) {
                free(raiz);
                raiz = aux;
            }
        }
    }
    
}

void substituir(Arvb *arv, int *valor, Arvb *pai) { // Caso precise apagar um item de uma Página que possua filhos, vai substituir o item com número mais próximo(sucessor)
    if (vaziaOrdem(arv->filhos[0])) {
        swap(&(arv->itens[arv->tamanho-1]), valor);
        arv->tamanho--;
    } else {
        substituir(arv->filhos[arv->tamanho], valor, arv);
    }
    if (metade(arv))
        underSplit(arv, pai);
}

int removerPagina(Arvb *arv, int valor){ // Remove o item da Página
    int status = 0;
    if (vaziaOrdem(arv->filhos[0])) {
        for (int i = 0; i < arv->tamanho; i++) {
            if (arv->itens[i] == valor){ // Coloca o item no final da Página e diminui o tamanho dela
                swap(&(arv->itens[i]), &(arv->itens[i+1]));
                status = 1;
            }
        }
    }
    if (status == 1)
        arv->tamanho--;
    return status;
}

void removerArv(Arvb *arv, Arvb *pai, int valor){ // Remover ...
    if (vaziaOrdem(arv)) {
        printf("arvore nao inicida ainda\n");
    } else {
        if (vazia(arv)) {
            printf("nao possui elementos para remover\n");
        } else {
            if (vaziaOrdem(arv->filhos[0])) { //caso ela nao possua filhos
               removerPagina(arv, valor) ? printf("removeu\n"):printf("nao existe\n");
            } else {
                for (int i = 0; i < arv->tamanho; i++) {
                    if(valor == arv->itens[i]){ //executa a funcao de troca
                        substituir(arv->filhos[i], &(arv->itens[i]), arv);
                        break;
                    } else if (valor < arv->itens[i]) {
                        removerArv(arv->filhos[i], arv, valor);
                        break;
                    } else if (i == arv->tamanho-1) {
                        removerArv(arv->filhos[i+1], arv, valor);
                        break;
                    }
               }
            }
            if (metade(arv) && arv != raiz && vazia(arv) !=1 ) { //testa se esta da capacidade minima
                underSplit(arv, pai);
            }
        }
    }
}

void apagar(Arvb *arv){ // Apagar toda a Árvore
    if (vaziaOrdem(arv)) {
        printf("nao iniciada\n");
    } else {
        if (vazia(arv)) {
            printf("nao possue elementos\n");
        } else {
            if (vaziaOrdem(arv->filhos[0])) {
                free(arv);
            } else {
                for (int i = 0; i < arv->tamanho; i++) {
                    apagar(arv->filhos[i]);
                }
                if(arv == raiz)
                    arv->tamanho = 0;
                else
                    free(arv);
            }
        }
    }
}

#pragma endregion

#pragma region Funções de Imprimir

void printLista(int *list, int tamanho){ // Imprime uma Página
    for (int i = 0; i < tamanho; i++){
        printf("%d ", list[i]);
    }
}

void printArv(Arvb *arv, int altura) { // Imprime a Árvore toda
    if (vaziaOrdem(arv)) {
        printf("nao iniciada a arvore\n");
    } else {
        if (vazia(arv)) {
            printf("vazia\n");
        } else {
            if(arv == raiz)
                printf("raiz%*s", 10, "");
            else
                printf("filho[%d]%*s", altura, altura*3 +10, "|");
            printLista(arv->itens, arv->tamanho);
            printf("\n");
            if(vaziaOrdem(arv->filhos[0]) == 0)
                for (int i = 0; i <= arv->tamanho; i++) {
                    printArv(arv->filhos[i], altura+1);
                }
            altura++;
        }
    }
}

void printVetor(Arvb *arv) { // Função ñ utilizada
    if (vaziaOrdem(arv)) {
        printf("nao inicializada\n");
    } else {
        if (vazia(arv)) {
            printf("sem elementos\n");
        } else {
            printLista(arv->itens, arv->tamanho);
            // if(arv==raiz)
            //     printf("\n");
            if (vaziaOrdem(arv->filhos[0]) == 0) {
                for (int i = 0; i<= arv->tamanho; i++) {
                    printVetor(arv->filhos[i]);
                }
            }
        }
    }
}

#pragma endregion

#pragma region Função Menu

void menu(int *b) {
    char escolha = '0';
    while (escolha != 'N') {
        printf("Escolha uma opcão\n");
        printf("1-inserir\n");
        printf("2-inserir aleatorio\n");
        printf("3-imprimir\n");
        printf("4-pesquizar\n");
        printf("5-remover\n");
        printf("6-limpar a tela\n");
        printf("7-apagar arvore\n");
        printf("8-sair\n");
        scanf("%c", &escolha);
        switch (escolha) {
            case '1':
                printf("digite um numero\n");
                int numero;
                scanf("%d", &numero);
                inserir(raiz, NULL, numero);
                break;
            case '2':
                printf("digite a quantidade de numeros\n");
                int quantidade;
                scanf("%d", &quantidade);
                srand(time(NULL));
                for (int i = 1; i < quantidade; i++) {
                    inserir(raiz, NULL, rand()%(i*10));
                }
                break;
            case '3':printArv(raiz, 0);
                break;
            case '4':
                printf("Digite o numero que quer pesquisar\n");
                int pesquisar;
                scanf("%d", &pesquisar);
                pesquisa(raiz, pesquisar);
                break;
            case '5': 
                printf("Digite o item para ser removido\n");
                int valor;
                scanf("%d", &valor);
                removerArv(raiz, NULL, valor);
                break;
            case '6': system("clear"); break;
            case '7': apagar(raiz); break;
            case '8':
            printf("saindo\n");
                *b=1;
                exit(0);
            break;
            default:
                printf("Opcao invalida\n");
                scanf("%c", &escolha);
        }
        scanf("%c", &escolha);
    }
}

#pragma endregion
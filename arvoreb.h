#ifndef ARVORE_B
#define ARVORE_B

#define ORDEM 9 // Ordem da Árvore

/*
    Estrutura da Árvore
*/

typedef struct pagina {
    int tamanho;
    int itens[ORDEM-1];
    struct pagina *filhos[ORDEM];
} Arvb;

Arvb *raiz;

/*
    Funções
*/

// Funções Iniciais
Arvb *criarArv();
void menu(int *b);

// Funções de Imprimir
void printArv(Arvb *arv,int altura); // Imprime a Árvore
void printVetor(Arvb *arv); // Não está sendo utilizado no código ...
void printFormat(Arvb *arv,int altura); // Função não existe

// Funções de Inserção
void inserir(Arvb *arv,Arvb *pai,int valor);

// Funções de Remoção
void remover(Arvb *arv,Arvb *pai,int valor);

// Funções de Pesquisa
Arvb* pesquisa(Arvb *arv,int valor);

// Funções de Limpar
void apagar(Arvb *);


#endif
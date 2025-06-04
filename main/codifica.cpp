#include <stdio.h>
#include <stdlib.h>
#include <conio2.h>
#include <string.h>
#include <math.h>
#include <ctype.h>


struct arvore{
    struct arvore *esq, *dir;
    int freq;
    char simbolo;
};

struct forest{
    struct forest *prox;
    struct arvore *tree;
};

struct listareg{
    struct listareg *ant, *prox;
    int freq;
    char palavra[50], simbolo, huffcode[22];
};

struct listaarq{
    int freq;
    char palavra[50], simbolo, huffcode[22];
};

typedef union {
    unsigned char byte;
    struct {
        unsigned b0 :1;
        unsigned b1 :1;
        unsigned b2 :1;
        unsigned b3 :1;
        unsigned b4 :1;
        unsigned b5 :1;
        unsigned b6 :1;
        unsigned b7 :1;
    } bits;
} Byte;

typedef struct arvore Tree;
typedef struct listareg ListaReg;
typedef struct forest Forest;
typedef struct listaarq ListaArq;

#include "Pilha.h"

// ------------ Outras Funções ------------

Tree *criaFolha(char simbolo,int freq){
    Tree *novo;
    novo = (Tree *)malloc(sizeof(Tree));

    novo->freq = freq;
    novo->simbolo = simbolo;
    novo->esq = NULL;
    novo->dir = NULL;

    return novo;
}

ListaReg *criaNo(char palavra[]){
    unsigned  static char contadorSimbolo = 1;
    ListaReg *novo;
    novo = (ListaReg *)malloc(sizeof(ListaReg));

    strcpy(novo->palavra,palavra);
    novo->freq = 1;
    novo->simbolo = contadorSimbolo++;
    novo->ant = NULL;
    novo->prox = NULL;

    return novo;
}

Forest *criaForest(){
    Forest *novo;
    novo = (Forest *)malloc(sizeof(Forest));
    novo->tree = NULL;
    novo->prox = NULL;

}

int pontuacao(char aux){
    return aux == '.' || aux == ',' || aux == '!' || aux == '?' || aux == ' ' || aux == '\0' || aux == '(' || aux == ')';
}

ListaReg *procuraPalavra(ListaReg *lista, char palavra[]){
    ListaReg *aux = lista;

    while(aux != NULL && strcmp(aux->palavra,palavra) != 0){
        aux = aux->prox;
    }
    return aux;
}

int numForest(Forest *list){
    int i = 0;

    while(list != NULL){
        i++;
        list = list->prox;
    }
    return i;
}

ListaReg *achaSimbolo(ListaReg *lista, char simbolo){

    while(lista != NULL && lista->simbolo != simbolo){
        lista = lista->prox;
    }
    return lista;
}


// ------------ Funções p/ Exibir ------------

void exibeLista(ListaReg *lista) {
    ListaReg *atual = lista;
    while (atual != NULL) {
        printf("Palavra: \"%s\" | Frequencia: %d | Simbolo: %u | HuffmanCode: %s\n", atual->palavra, atual->freq, atual->simbolo, atual->huffcode);
        atual = atual->prox;
    }
}

void exibeForest(Forest *lista) {
    Forest *atual = lista;
    while (atual != NULL) {
        printf("Frequencia: %d | Simbolo: %d\n", atual->tree->freq, atual->tree->simbolo);
        atual = atual->prox;
    }
}

void exibeHuff(Tree *raiz, int *n){

    if (raiz != NULL){
        if ((*n) == -1){
            printf("Arvore de Huffman: \n");
        }
        (*n)++;
        exibeHuff(raiz->dir, n);
        for (int i = 0; i < 5 * (*n); i++){ printf(" ");}
        if (raiz->simbolo == '-'){
            printf("(%c,%d)\n",raiz->simbolo,raiz->freq);
        }else{
            printf("(%d,%d)\n", raiz->simbolo, raiz->freq);
        }
        exibeHuff(raiz->esq, n);
        (*n)--;
    }
}

void exibeArq(char nomeArq[]){
    ListaArq regArq;
    FILE *ptrArq = fopen(nomeArq, "rb");
    if (ptrArq != NULL){
        fread(&regArq, sizeof(ListaArq), 1, ptrArq);
        while(!feof(ptrArq)){
            printf("%d \"%s\" %d %s\n",regArq.simbolo, regArq.palavra, regArq.freq, regArq.huffcode);
            fread(&regArq, sizeof(ListaArq), 1, ptrArq);
        }
        fclose(ptrArq);
    }
    else{
        printf("Erro ao abrir o arquivo!\n");
    }
}

// ------------ Função Ordena ------------

void ordenaFloresta(Forest **tree){
    Forest *aux = *tree, *compare;
    Tree *guarda;

    while(aux != NULL){

        compare = aux->prox;
        while(compare != NULL){

            if(aux->tree->freq > compare->tree->freq){
                
                guarda = aux->tree;
                aux->tree = compare->tree;
                compare->tree = guarda;

            }
            compare = compare->prox;
        }
        aux = aux->prox;
    }
}

// ------------ Funções de Inserir e criar Estruturas ------------

//insere palavra nova palavra na lista, mas se ela já existir na lista, aumenta sua frequência
void insereLista(ListaReg **lista, char palavra[]){

    ListaReg *aux, *novo;
    if(*lista == NULL){
        *lista = criaNo(palavra);
    }
    else{
        aux = procuraPalavra(*lista, palavra);
        if(aux == NULL){
            aux = *lista;

            while(aux->prox != NULL){
                aux = aux->prox;
            }
            novo = criaNo(palavra);
            aux->prox = novo;
            novo->ant = aux;
        }
        else{
            aux->freq++;
        }
    }
}

//cria lista de palavras separadas 
void criaLista(ListaReg **lista, char frase[]){
    char palavra[50];
    int i, j;

    for(j=0,i=0; i <= strlen(frase); i++){

        if(!pontuacao(frase[i])){
            palavra[j++] = tolower(frase[i]);
        }
        else{
            if(frase[i] == ' '){
                insereLista(lista, " ");
            }
            if(j > 0) {
                palavra[j] = '\0';
                insereLista(lista, palavra);
                
                palavra[0] = '\0';
                j = 0;
            }
        }
    }
    
}

//logica para criar arvore
void criaArvore(Tree **tree, Forest **forest){
    Forest *aux = *forest, *prox;
    Tree *novo;
    while(numForest(aux) > 0 && aux->prox != NULL){
        prox = aux->prox;

        novo = criaFolha('-',aux->tree->freq + prox->tree->freq);
        novo->esq = aux->tree;
        novo->dir = prox->tree;
        
        aux->prox = prox->prox;
        aux->tree = novo;

        free(prox);
        ordenaFloresta(forest);
    }
    *tree = aux->tree;
}

//começo do algoritimo, para cada registro da lista, cria uma folha e uma arvore que aponta pra ela
void criaAllFolhas(Tree **tree, ListaReg **lista, Forest **forest){
    ListaReg *aux;
    Forest *antF,*auxF;

    aux = *lista;
    while(aux != NULL){
        if(*forest == NULL){
            *forest = criaForest();
            (*forest)->tree = criaFolha(aux->simbolo,aux->freq);
            antF = *forest;
        }
        else{
            auxF = criaForest();
            auxF->tree = criaFolha(aux->simbolo,aux->freq);
            antF->prox = auxF;

            antF = auxF;
        }
        aux = aux->prox;
    }
    ordenaFloresta(forest);
}

//------------------------------

void getCodTree(Tree *raiz, ListaReg **lista){
    Pilha *P;
    PilhaStr caminho;
    ListaReg *aux;
    Tree *no;

    char temp[100],atual[100];

    inicializar(&P);
    inicializarStr(&caminho);

    push(&P, raiz);
    pushStr(&caminho, ""); //Começa com string vazia

    while (!pilhaVazia(P)) {
        
        pop(&P, &no);
        popStr(&caminho, atual);
        
        // Se for folha, salva o codigo
        if (no->esq == NULL && no->dir == NULL) {
            aux = achaSimbolo(*lista, no->simbolo);
            if (aux != NULL) {
                strcpy(aux->huffcode, atual);
            }
        }

        // Direita adiciona '1'
        if (no->dir != NULL) {
            strcpy(temp, atual);
            strcat(temp, "1");
            push(&P, no->dir);
            pushStr(&caminho, temp);
        }

        // Esquerda adiciona '0'
        if (no->esq != NULL) {
            strcpy(temp, atual);
            strcat(temp, "0");
            push(&P, no->esq);
            pushStr(&caminho, temp);
        }
    }
}


void bitabit(char bin[]){
    int i, len;
    Byte ub;
    len = strlen(bin);
    FILE *ptrArq = fopen("codificada.dat","wb");
    if (ptrArq != NULL) {
        for(i = 0; i < len;){
            ub.bits.b0 = bin[i++];
            ub.bits.b1 = bin[i++];
            ub.bits.b2 = bin[i++];
            ub.bits.b3 = bin[i++];
            ub.bits.b4 = bin[i++];
            ub.bits.b5 = bin[i++];
            ub.bits.b6 = bin[i++];
            ub.bits.b7 = bin[i++];

            fwrite(&ub.byte, sizeof(unsigned char), 1, ptrArq);
        }
    fclose(ptrArq);
    }
    else
        printf("Erro ao abrir o arquivo!\n");
}

void gravarListaArq(ListaReg *lista) {
    ListaReg *atual = lista;
    ListaArq regArq;

    FILE *ptrArq = fopen("registros.dat", "wb");
    if (ptrArq != NULL) {

        while (atual != NULL) {
            regArq.freq = atual->freq;
            regArq.simbolo = atual->simbolo;
            strcpy(regArq.palavra, atual->palavra);
            strcpy(regArq.huffcode,atual->huffcode);

            fwrite(&regArq, sizeof(regArq), 1, ptrArq);
            atual = atual->prox;
        }

        fclose(ptrArq);
    }
    else
        printf("Erro ao abrir o arquivo!\n");
}


ListaReg *procuraText(ListaReg *lista, char palavra[]){
    while(lista != NULL && strcmp(lista->palavra, palavra)){
        lista = lista->prox;
    }
    return lista;
}

void obtemCod(ListaReg *lista, char frase[],char codHuff[]){
    char palavra[50];
    ListaReg *aux;
    int i, j, len;
    
    for(j=0,i=0; i <= strlen(frase); i++){

        if(!pontuacao(frase[i])){
            palavra[j++] = tolower(frase[i]);
        }
        else{
            if(j > 0) {
                palavra[j] = '\0';
                aux = procuraText(lista, palavra);
                if(aux != NULL){
                    strcat(codHuff,aux->huffcode);
                }
                palavra[0] = '\0';
                j = 0;
            }
            if(frase[i] == ' '){
                aux = procuraText(lista, " ");
                if(aux != NULL){
                    strcat(codHuff,aux->huffcode);
                }
            }
        }
    }

    len = strlen(codHuff);

    while(len % 8 != 0){
        codHuff[len++] = '0';
    }
    codHuff[len] = '\0';
}

int main(){
    ListaReg *lista = NULL;
    Tree *tree = NULL;
    Forest *forest = NULL;
    int n = -1;
    char codHuff[500] = {"\0"};

    char texto[] = "Sao Paulo Paulo e Libertadores internacional, Sao Paulo Paulo historia titulos e impacto mundial. Clube consecutivos Brasil e titulos ou internacional, Libertadores ou Libertadores Sao Paulo e Paulo Copa Sul Supercopa Libertadores ou Mundial e Mundial ou Mundial e Copa Conmebol Copa consecutivos Conmebol Master Recopa Recopa internacional Campeonato Brasileiro pontos corridos, Supercopa Brasil tradicao e Rogerio Ceni goleiro gols historia futebol mundial Libertadores Mundial talentos ou Kaka Hernanes e Lucas Moura consecutivos Casemiro. Sao Paulo tradicao conquistas conquistas internacional Sao Paulo Clube titulos Copa Brasil, Campeonato Paulista Sao Paulo rapidamente futebol brasileiro Campeonato Paulista, Sao Paulo consecutivos Campeonato Paulista Sao Paulo Clube Clube Sao Paulo impacto internacional titulos nacionais internacionais. Internacional Brasil titulos CONMEBOL Sao Paulo Clube Libertadores Libertadores Mundial consecutivos Mundial Mundial Copa Sul Supercopa Libertadores Recopa Recopa Copa Conmebol Copa Master Conmebol. Internacional Campeonato Brasileiro Campeonato Brasileiro Campeonato Brasileiro Campeonato Brasileiro Campeonato Brasileiro Campeonato Brasileiro pontos corridos Copa Brasil Supercopa Brasil consecutivos tradicao Rogerio Ceni goleiro gols futebol Libertadores Mundial Sao Paulo talentos formou Kaka Hernanes Lucas Moura Casemiro. Historia conquistas futebol brasileiro internacional Sao Paulo Clube esportivas instituicoes esportivas pais Sao Paulo esportivas Sao Paulo Sao Paulo Sao Paulo Paulista rapidamente futebol brasileiro titulos nacionais internacionais clube Brasil consecutivos Sao Paulo Copa Libertadores Libertadores Libertadores Mundial Mundial Mundial Supercopa consecutivos Libertadores Copa Sul Conmebol Master Recopa Recopa Campeonato consecutivos Brasileiro Brasileiro consecutivos e Brasileiro Brasileiro Brasileiro Brasileiro consecutivos pontos corridos Supercopa Brasil tradicao Rogerio Ceni consecutivos ou historia futebol mundial Libertadores Mundial Sao Paulo talentos Kaka Hernanes Lucas Moura Casemiro. Clube internacional impacto historia conquistas Sao Paulo consecutivos Brasil ou Sao Paulo tradicao e internacional clube Brasil titulos Libertadores Mundial Mundial Copa Sul Supercopa Libertadores Recopa Recopa consecutivos Copa Conmebol Copa Master Campeonato goleiro Brasileiro consecutivos Brasileiro Brasileiro Brasileiro consecutivos pontos corridos consecutivos e Supercopa Brasil Rogerio Ceni goleiro historia futebol mundial Libertadores Mundial Sao Paulo talentos formou Kaka Hernanes Lucas Moura Casemiro. Clube Sao Paulo historia conquistas futebol brasileiro internacional Sao Paulo Clube instituicoes esportivas pais.";

    //char frase[] = "                    Sao                       ";
    char frase[] = "Sao Paulo Paulo e titulos internacional nacionais e historia kaka historia ou e futebol brasileiro Kaka Hernanes Lucas Moura e Casemiro";

    //Texto gravado em arq
    criaLista(&lista,texto);
    criaAllFolhas(&tree, &lista, &forest);
    criaArvore(&tree,&forest);
    getCodTree(tree,&lista);
    exibeHuff(tree, &n);
    gravarListaArq(lista);
    exibeLista(lista);
    //Codifica a Frase
    obtemCod(lista,frase,codHuff);
    bitabit(codHuff);
    //exibeArq("registros.dat");
    getche();
    return 0;
}
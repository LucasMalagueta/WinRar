#include <stdio.h>
#include <stdlib.h>
#include <conio2.h>
#include <string.h>
#include <math.h>
#include <ctype.h>


struct tree{
    char simbolo;
    struct tree *esq, *dir;
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

typedef struct tree Tree;
typedef struct listaarq ListaArq;

#include "Pilha.h"

Tree *criaNo(char simbolo){
    Tree *novo;
    novo = (Tree*)malloc(sizeof(Tree));

    novo->simbolo = simbolo;
    novo->esq = NULL;
    novo->dir = NULL;
    return novo;
}

void exibeHuff(Tree *raiz, int *n){

    if (raiz != NULL){
        if ((*n) == -1){
            printf("Arvore de Huffman: \n");
        }
        (*n)++;
        exibeHuff(raiz->dir, n);
        for (int i = 0; i < 5 * (*n); i++){ printf(" ");}
        if (raiz->simbolo == '0'){
            printf("(-)\n");
        }
        else{
            printf("(%d)\n", raiz->simbolo);
        }
        exibeHuff(raiz->esq, n);
        (*n)--;
    }
}

void montaArvore(Tree **raiz, char codHuff[], char simbolo){
    Tree *aux;
    int i;
    
    if(*raiz == NULL){
        *raiz = aux = criaNo('0');
    }
    aux = *raiz;
    for(i = 0; codHuff[i] != '\0'; i++){
        
        if(codHuff[i] == '0'){
            if(aux->esq == NULL){
                aux->esq = criaNo('0');
            }
            aux = aux->esq; 
        }
        else if(codHuff[i] == '1'){
            if(aux->dir == NULL){
                aux->dir = criaNo('0');
            }
            aux = aux->dir;
        }
    }
    
    aux->simbolo = simbolo;
}

void leArqCodificado(Tree **raiz){
    ListaArq lista;
    char cod[9];
    FILE *ptrarq = fopen("registros.dat","rb");

    if(ptrarq != NULL){
        fread(&lista,sizeof(ListaArq),1,ptrarq);

        while(!feof(ptrarq)){
            montaArvore(raiz, lista.huffcode, lista.simbolo);
            fread(&lista,sizeof(ListaArq),1,ptrarq);
        }
        fclose(ptrarq);
    }
}

void printaPalavra(char simbolo){
    ListaArq reg;
    FILE *ptrreg = fopen("registros.dat","rb");

    if(ptrreg != NULL){
        fread(&reg,sizeof(ListaArq),1,ptrreg);
        while(!feof(ptrreg) && reg.simbolo != simbolo)
            fread(&reg,sizeof(ListaArq),1,ptrreg);

        if(!feof(ptrreg)){
            printf("%s", reg.palavra);

            if (strcmp(reg.palavra, " ") != 0)
                printf(" ");
        }
        fclose(ptrreg);
    }
}

void procuraSimboloArq(Tree *raiz){
    Tree *aux;
    Byte ub;
    FILE *ptrfra = fopen("codificada.dat","rb");

    if(ptrfra != NULL){

        fseek(ptrfra,0,2);
        int tamanho = ftell(ptrfra)*8, i;
        int rec[tamanho + 1];
        fseek(ptrfra,0,0);
        
        i = 0;
        while(fread(&ub.bits, sizeof(unsigned char), 1, ptrfra)){
    
            rec[i++] = ub.bits.b0;
            rec[i++] = ub.bits.b1;
            rec[i++] = ub.bits.b2;
            rec[i++] = ub.bits.b3;
            rec[i++] = ub.bits.b4;
            rec[i++] = ub.bits.b5;
            rec[i++] = ub.bits.b6;
            rec[i++] = ub.bits.b7;
        }
        rec[i] = 3;

        aux = raiz;
        for(i = 0; rec[i] != 3; i++){
            if(aux->esq == NULL && aux->dir == NULL){
                printaPalavra(aux->simbolo);
                aux = raiz;
            }
            else if(rec[i] == 0 && aux->esq != NULL){
                aux = aux->esq;
            }
            else if(rec[i] == 1 && aux->dir != NULL){
                aux = aux->dir;
            }
        }
        // for(i = 0;rec[i] != 3;i++){
        //     printf("%d",rec);
        // }
    }
    else{
        printf("Erro ao abrir o arquivo\n");
    }
}

int main(){
    Tree *raiz = NULL;
    int n = -1;

    leArqCodificado(&raiz);
    exibeHuff(raiz,&n);
    procuraSimboloArq(raiz);
    getche();
}
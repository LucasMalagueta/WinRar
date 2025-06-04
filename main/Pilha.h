#define PILHA_H
#define TAM_MAX 100

typedef struct lista Lista;

struct pilha{
    Tree* itens[TAM_MAX];
    int prof;
    int topo;
};
typedef struct pilha Pilha;

typedef struct {
    char itens[TAM_MAX][100]; // Pilha de strings
    int topo;
} PilhaStr;

void inicializar(Pilha **p){
    *p = (Pilha*)malloc(sizeof(Pilha));
    (*p)->topo = -1;
    (*p)->prof = 0;
}

char pilhaVazia(Pilha *p){
    return p->topo == -1;
}

char pilhaCheia(Pilha *p){
    return p->topo == TAM_MAX - 1;
}

void push(Pilha **p, Tree *valor){
    if (!pilhaCheia(*p)) {
        (*p)->itens[++((*p)->topo)] = valor;
        (*p)->prof++;
    }
}

void pop(Pilha **p, Tree **valor){
    if (!pilhaVazia(*p)) {
        *valor = (*p)->itens[((*p)->topo)--];
        (*p)->prof--;
    }
}

void inicializarStr(PilhaStr *p) {
    p->topo = -1;
}

char pilhaVaziaStr(PilhaStr *p) {
    return p->topo == -1;
}

void pushStr(PilhaStr *p, char valor[]) {
    if (p->topo < TAM_MAX - 1) {
        strcpy(p->itens[++(p->topo)], valor);
    }
}

void popStr(PilhaStr *p, char valor[]) {
    if (!pilhaVaziaStr(p)) {
        strcpy(valor, p->itens[(p->topo)--]);
    }
}

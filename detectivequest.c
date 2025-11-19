#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Sala {
    char nome[50];
    char pista[100];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

typedef struct PistaNode {
    char pista[100];
    struct PistaNode *esq;
    struct PistaNode *dir;
} PistaNode;

typedef struct HashItem {
    char pista[100];
    char suspeito[50];
    struct HashItem *prox;
} HashItem;

#define HASH_SIZE 50
HashItem *hash[HASH_SIZE];

int hashFunction(char *str) {
    int soma = 0;
    for(int i = 0; str[i]; i++) soma += str[i];
    return soma % HASH_SIZE;
}

void inserirNaHash(char *pista, char *suspeito) {
    int idx = hashFunction(pista);
    HashItem *novo = malloc(sizeof(HashItem));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->prox = hash[idx];
    hash[idx] = novo;
}

char* encontrarSuspeito(char *pista) {
    int idx = hashFunction(pista);
    HashItem *atual = hash[idx];
    while(atual) {
        if(strcmp(atual->pista, pista) == 0) return atual->suspeito;
        atual = atual->prox;
    }
    return NULL;
}

PistaNode* inserirPista(PistaNode *raiz, char *pista) {
    if(!raiz) {
        PistaNode *novo = malloc(sizeof(PistaNode));
        strcpy(novo->pista, pista);
        novo->esq = novo->dir = NULL;
        return novo;
    }
    if(strcmp(pista, raiz->pista) < 0)
        raiz->esq = inserirPista(raiz->esq, pista);
    else
        raiz->dir = inserirPista(raiz->dir, pista);
    return raiz;
}

void exibirPistas(PistaNode *raiz) {
    if(!raiz) return;
    exibirPistas(raiz->esq);
    printf("%s\n", raiz->pista);
    exibirPistas(raiz->dir);
}

Sala* criarSala(char *nome, char *pista) {
    Sala *s = malloc(sizeof(Sala));
    strcpy(s->nome, nome);
    strcpy(s->pista, pista);
    s->esquerda = s->direita = NULL;
    return s;
}

void explorarSalas(Sala *salaAtual, PistaNode **raizPistas) {
    if(!salaAtual) return;
    printf("Você está em: %s\n", salaAtual->nome);
    if(strlen(salaAtual->pista) > 0) {
        printf("Você encontrou uma pista: %s\n", salaAtual->pista);
        *raizPistas = inserirPista(*raizPistas, salaAtual->pista);
    }
    printf("Escolha: esquerda(e), direita(d) ou sair(s): ");
    char escolha;
    scanf(" %c", &escolha);
    if(escolha == 'e') explorarSalas(salaAtual->esquerda, raizPistas);
    else if(escolha == 'd') explorarSalas(salaAtual->direita, raizPistas);
}

int contarPistas(PistaNode *raiz, char *suspeito) {
    if(!raiz) return 0;
    char *s = encontrarSuspeito(raiz->pista);
    int cont = (s && strcmp(s, suspeito) == 0) ? 1 : 0;
    return cont + contarPistas(raiz->esq, suspeito) + contarPistas(raiz->dir, suspeito);
}

int main() {
    for(int i=0;i<HASH_SIZE;i++) hash[i]=NULL;
    Sala *hall = criarSala("Hall de Entrada", "");
    Sala *sala1 = criarSala("Sala de Estar", "pegada de sapato");
    Sala *sala2 = criarSala("Cozinha", "faca suja");
    Sala *sala3 = criarSala("Biblioteca", "livro fora do lugar");
    Sala *sala4 = criarSala("Jardim", "pegadas na lama");

    hall->esquerda = sala1;
    hall->direita = sala2;
    sala1->esquerda = sala3;
    sala2->direita = sala4;

    inserirNaHash("pegada de sapato", "Sr. Silva");
    inserirNaHash("faca suja", "Sra. Oliveira");
    inserirNaHash("livro fora do lugar", "Sr. Silva");
    inserirNaHash("pegadas na lama", "Sr. Costa");

    PistaNode *raizPistas = NULL;
    explorarSalas(hall, &raizPistas);

    printf("\nPistas coletadas em ordem:\n");
    exibirPistas(raizPistas);

    printf("\nQuem você acusa? ");
    char suspeito[50];
    scanf(" %[^\n]", suspeito);

    int cont = contarPistas(raizPistas, suspeito);
    if(cont >= 2) printf("Parabéns! As pistas confirmam sua acusação.\n");
    else printf("Desculpe, não há pistas suficientes para acusar %s.\n", suspeito);

    return 0;
}

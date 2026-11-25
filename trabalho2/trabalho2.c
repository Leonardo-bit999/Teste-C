#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>

#define TAM 10

#include "trabalho2.h"

/*
 * Cada posicao do vetor principal aponta para uma estrutura auxiliar.
 * A estrutura auxiliar controla seu vetor de inteiros, sua capacidade
 * total e a quantidade de elementos efetivamente armazenados.
 */
struct EstruturaAuxiliar {
    int *numeros;
    int capacidade;
    int quantidade;
};

/* Unica variavel global do arquivo: vetor principal com 10 ponteiros. */
struct EstruturaAuxiliar *vetorPrincipal[TAM] = {NULL};

/* Prototipos de funcoes auxiliares internas. */
static void ordenarVetor(int vetor[], int quantidade);
static int totalElementos(void);
static void liberarTudo(void);
static int tamanhoAlocavel(int capacidade);

/* Verifica se a posicao informada pelo usuario esta entre 1 e TAM. */
int ehPosicaoValida(int posicao)
{
    if (posicao < 1 || posicao > TAM)
        return POSICAO_INVALIDA;

    return SUCESSO;
}

/* Ordena um vetor usando Bubble Sort. */
static void ordenarVetor(int vetor[], int quantidade)
{
    int i;
    int j;
    int temporario;

    for (i = 0; i < quantidade - 1; i++) {
        for (j = 0; j < quantidade - 1 - i; j++) {
            if (vetor[j] > vetor[j + 1]) {
                temporario = vetor[j];
                vetor[j] = vetor[j + 1];
                vetor[j + 1] = temporario;
            }
        }
    }
}

/* Soma a quantidade de elementos armazenados em todas as auxiliares. */
static int totalElementos(void)
{
    int i;
    int total = 0;

    for (i = 0; i < TAM; i++) {
        if (vetorPrincipal[i] != NULL)
            total += vetorPrincipal[i]->quantidade;
    }

    return total;
}

/* Verifica se a quantidade solicitada pode ser multiplicada por sizeof(int). */
static int tamanhoAlocavel(int capacidade)
{
    if (capacidade < 1)
        return 0;

    return (size_t) capacidade <= SIZE_MAX / sizeof(int);
}

/* Libera todas as estruturas auxiliares e deixa o vetor principal vazio. */
static void liberarTudo(void)
{
    int i;

    for (i = 0; i < TAM; i++) {
        if (vetorPrincipal[i] != NULL) {
            free(vetorPrincipal[i]->numeros);
            vetorPrincipal[i]->numeros = NULL;

            free(vetorPrincipal[i]);
            vetorPrincipal[i] = NULL;
        }
    }
}

/* Funcao complementar declarada no cabecalho fornecido pelo professor. */
void dobrar(int *x)
{
    if (x != NULL && (*x <= INT_MAX / 2) && (*x >= INT_MIN / 2))
        *x = (*x) * 2;
}

/*
Objetivo: criar estrutura auxiliar na posicao 'posicao',
com tamanho 'tamanho'.

Retorno (int):
    SUCESSO - criado com sucesso
    JA_TEM_ESTRUTURA_AUXILIAR - ja tem estrutura na posicao
    POSICAO_INVALIDA - posicao invalida para estrutura auxiliar
    SEM_ESPACO_DE_MEMORIA - sem espaco de memoria
    TAMANHO_INVALIDO - o tamanho deve ser maior ou igual a 1
*/
int criarEstruturaAuxiliar(int posicao, int tamanho)
{
    int indice;
    struct EstruturaAuxiliar *novaEstrutura;

    if (ehPosicaoValida(posicao) != SUCESSO)
        return POSICAO_INVALIDA;

    if (tamanho < 1 || !tamanhoAlocavel(tamanho))
        return TAMANHO_INVALIDO;

    indice = posicao - 1;

    if (vetorPrincipal[indice] != NULL)
        return JA_TEM_ESTRUTURA_AUXILIAR;

    novaEstrutura = (struct EstruturaAuxiliar *) malloc(sizeof(struct EstruturaAuxiliar));
    if (novaEstrutura == NULL)
        return SEM_ESPACO_DE_MEMORIA;

    novaEstrutura->numeros = (int *) malloc((size_t) tamanho * sizeof(int));
    if (novaEstrutura->numeros == NULL) {
        free(novaEstrutura);
        return SEM_ESPACO_DE_MEMORIA;
    }

    novaEstrutura->capacidade = tamanho;
    novaEstrutura->quantidade = 0;

    vetorPrincipal[indice] = novaEstrutura;

    return SUCESSO;
}

/*
Objetivo: inserir numero 'valor' em estrutura auxiliar da posicao 'posicao'.

Retorno (int):
    SUCESSO - inserido com sucesso
    SEM_ESPACO - nao tem espaco
    SEM_ESTRUTURA_AUXILIAR - nao tem estrutura auxiliar
    POSICAO_INVALIDA - posicao invalida para estrutura auxiliar
*/
int inserirNumeroEmEstrutura(int posicao, int valor)
{
    int indice;
    struct EstruturaAuxiliar *estrutura;

    if (ehPosicaoValida(posicao) != SUCESSO)
        return POSICAO_INVALIDA;

    indice = posicao - 1;
    estrutura = vetorPrincipal[indice];

    if (estrutura == NULL)
        return SEM_ESTRUTURA_AUXILIAR;

    if (estrutura->quantidade >= estrutura->capacidade)
        return SEM_ESPACO;

    estrutura->numeros[estrutura->quantidade] = valor;
    estrutura->quantidade++;

    return SUCESSO;
}

/*
Objetivo: excluir o ultimo numero da estrutura auxiliar da posicao 'posicao'.
Esta e uma exclusao logica: a capacidade da estrutura nao diminui.

Retorno (int):
    SUCESSO - excluido com sucesso
    ESTRUTURA_AUXILIAR_VAZIA - estrutura vazia
    SEM_ESTRUTURA_AUXILIAR - nao tem estrutura auxiliar
    POSICAO_INVALIDA - posicao invalida para estrutura auxiliar
*/
int excluirNumeroDoFinaldaEstrutura(int posicao)
{
    int indice;
    struct EstruturaAuxiliar *estrutura;

    if (ehPosicaoValida(posicao) != SUCESSO)
        return POSICAO_INVALIDA;

    indice = posicao - 1;
    estrutura = vetorPrincipal[indice];

    if (estrutura == NULL)
        return SEM_ESTRUTURA_AUXILIAR;

    if (estrutura->quantidade == 0)
        return ESTRUTURA_AUXILIAR_VAZIA;

    estrutura->quantidade--;

    return SUCESSO;
}

/*
Objetivo: excluir a primeira ocorrencia do numero 'valor' da estrutura auxiliar
na posicao 'posicao'. Os numeros posteriores sao movidos para as posicoes
anteriores. Esta e uma exclusao logica: a capacidade nao diminui.

Retorno (int):
    SUCESSO - excluido com sucesso
    ESTRUTURA_AUXILIAR_VAZIA - estrutura vazia
    SEM_ESTRUTURA_AUXILIAR - nao tem estrutura auxiliar
    NUMERO_INEXISTENTE - numero nao existe
    POSICAO_INVALIDA - posicao invalida para estrutura auxiliar
*/
int excluirNumeroEspecificoDeEstrutura(int posicao, int valor)
{
    int indice;
    int i;
    int j;
    struct EstruturaAuxiliar *estrutura;

    if (ehPosicaoValida(posicao) != SUCESSO)
        return POSICAO_INVALIDA;

    indice = posicao - 1;
    estrutura = vetorPrincipal[indice];

    if (estrutura == NULL)
        return SEM_ESTRUTURA_AUXILIAR;

    if (estrutura->quantidade == 0)
        return ESTRUTURA_AUXILIAR_VAZIA;

    for (i = 0; i < estrutura->quantidade; i++) {
        if (estrutura->numeros[i] == valor) {
            for (j = i; j < estrutura->quantidade - 1; j++)
                estrutura->numeros[j] = estrutura->numeros[j + 1];

            estrutura->quantidade--;
            return SUCESSO;
        }
    }

    return NUMERO_INEXISTENTE;
}

/*
Objetivo: retornar os numeros da estrutura auxiliar da posicao 'posicao'.
Os numeros devem ser armazenados em vetorAux.

Retorno (int):
    SUCESSO - recuperado com sucesso
    SEM_ESTRUTURA_AUXILIAR - nao tem estrutura auxiliar
    POSICAO_INVALIDA - posicao invalida para estrutura auxiliar
*/
int getDadosEstruturaAuxiliar(int posicao, int vetorAux[])
{
    int indice;
    int i;
    struct EstruturaAuxiliar *estrutura;

    if (ehPosicaoValida(posicao) != SUCESSO)
        return POSICAO_INVALIDA;

    indice = posicao - 1;
    estrutura = vetorPrincipal[indice];

    if (estrutura == NULL)
        return SEM_ESTRUTURA_AUXILIAR;

    if (vetorAux == NULL && estrutura->quantidade > 0)
        return SEM_ESPACO_DE_MEMORIA;

    for (i = 0; i < estrutura->quantidade; i++)
        vetorAux[i] = estrutura->numeros[i];

    return SUCESSO;
}

/*
Objetivo: retornar os numeros ordenados da estrutura auxiliar da posicao
'posicao'. Os numeros devem ser armazenados em vetorAux, sem alterar a
ordem original dos elementos na estrutura auxiliar.
*/
int getDadosOrdenadosEstruturaAuxiliar(int posicao, int vetorAux[])
{
    int retorno;
    int indice;

    retorno = getDadosEstruturaAuxiliar(posicao, vetorAux);
    if (retorno != SUCESSO)
        return retorno;

    indice = posicao - 1;
    ordenarVetor(vetorAux, vetorPrincipal[indice]->quantidade);

    return SUCESSO;
}

/*
Objetivo: retornar os numeros de todas as estruturas auxiliares.
Os numeros devem ser armazenados em vetorAux.
*/
int getDadosDeTodasEstruturasAuxiliares(int vetorAux[])
{
    int i;
    int j;
    int indiceVetor = 0;
    int total = totalElementos();

    if (total == 0)
        return TODAS_ESTRUTURAS_AUXILIARES_VAZIAS;

    if (vetorAux == NULL)
        return SEM_ESPACO_DE_MEMORIA;

    for (i = 0; i < TAM; i++) {
        if (vetorPrincipal[i] != NULL) {
            for (j = 0; j < vetorPrincipal[i]->quantidade; j++)
                vetorAux[indiceVetor++] = vetorPrincipal[i]->numeros[j];
        }
    }

    return SUCESSO;
}

/*
Objetivo: retornar os numeros ordenados de todas as estruturas auxiliares.
Os numeros devem ser armazenados em vetorAux, sem alterar os vetores originais.
*/
int getDadosOrdenadosDeTodasEstruturasAuxiliares(int vetorAux[])
{
    int total = totalElementos();
    int retorno;

    if (total == 0)
        return TODAS_ESTRUTURAS_AUXILIARES_VAZIAS;

    retorno = getDadosDeTodasEstruturasAuxiliares(vetorAux);
    if (retorno != SUCESSO)
        return retorno;

    ordenarVetor(vetorAux, total);

    return SUCESSO;
}

/*
Objetivo: modificar a capacidade da estrutura auxiliar da posicao 'posicao'
para capacidade atual + 'novoTamanho'. A capacidade final deve ser >= 1.
Elementos que nao couberem apos uma diminuicao sao descartados.
*/
int modificarTamanhoEstruturaAuxiliar(int posicao, int novoTamanho)
{
    int indice;
    long long capacidadeCalculada;
    int novaCapacidade;
    int *novosNumeros;
    struct EstruturaAuxiliar *estrutura;

    if (ehPosicaoValida(posicao) != SUCESSO)
        return POSICAO_INVALIDA;

    indice = posicao - 1;
    estrutura = vetorPrincipal[indice];

    if (estrutura == NULL)
        return SEM_ESTRUTURA_AUXILIAR;

    capacidadeCalculada = (long long) estrutura->capacidade + (long long) novoTamanho;

    if (capacidadeCalculada < 1 || capacidadeCalculada > INT_MAX)
        return NOVO_TAMANHO_INVALIDO;

    novaCapacidade = (int) capacidadeCalculada;
    if (!tamanhoAlocavel(novaCapacidade))
        return NOVO_TAMANHO_INVALIDO;

    novosNumeros = (int *) realloc(estrutura->numeros,
                                   (size_t) novaCapacidade * sizeof(int));
    if (novosNumeros == NULL)
        return SEM_ESPACO_DE_MEMORIA;

    estrutura->numeros = novosNumeros;
    estrutura->capacidade = novaCapacidade;

    if (estrutura->quantidade > novaCapacidade)
        estrutura->quantidade = novaCapacidade;

    return SUCESSO;
}

/*
Objetivo: retornar a quantidade de elementos preenchidos da estrutura auxiliar.

Retorno (int):
    POSICAO_INVALIDA - posicao invalida
    SEM_ESTRUTURA_AUXILIAR - sem estrutura auxiliar
    ESTRUTURA_AUXILIAR_VAZIA - estrutura auxiliar vazia
    Um numero int > 0 correspondente a quantidade de elementos preenchidos
*/
int getQuantidadeElementosEstruturaAuxiliar(int posicao)
{
    int indice;

    if (ehPosicaoValida(posicao) != SUCESSO)
        return POSICAO_INVALIDA;

    indice = posicao - 1;

    if (vetorPrincipal[indice] == NULL)
        return SEM_ESTRUTURA_AUXILIAR;

    if (vetorPrincipal[indice]->quantidade == 0)
        return ESTRUTURA_AUXILIAR_VAZIA;

    return vetorPrincipal[indice]->quantidade;
}

/* Retorna a capacidade da estrutura auxiliar ou um codigo de erro. */
int getCapacidadeEstruturaAuxiliar(int posicao)
{
    int indice;

    if (ehPosicaoValida(posicao) != SUCESSO)
        return POSICAO_INVALIDA;

    indice = posicao - 1;

    if (vetorPrincipal[indice] == NULL)
        return SEM_ESTRUTURA_AUXILIAR;

    return vetorPrincipal[indice]->capacidade;
}

/*
Objetivo: montar uma lista encadeada com cabecote contendo todos os numeros
presentes em todas as estruturas auxiliares.

Retorno (No*):
    NULL, caso nao tenha nenhum numero ou ocorra falha de alocacao
    No*, ponteiro para o inicio da lista com cabecote
*/
No *montarListaEncadeadaComCabecote(void)
{
    int i;
    int j;
    No *cabecote;
    No *ultimo;

    if (totalElementos() == 0)
        return NULL;

    cabecote = (No *) malloc(sizeof(No));
    if (cabecote == NULL)
        return NULL;

    cabecote->conteudo = 0;
    cabecote->prox = NULL;
    ultimo = cabecote;

    for (i = 0; i < TAM; i++) {
        if (vetorPrincipal[i] == NULL)
            continue;

        for (j = 0; j < vetorPrincipal[i]->quantidade; j++) {
            No *novoNo = (No *) malloc(sizeof(No));

            if (novoNo == NULL) {
                destruirListaEncadeadaComCabecote(&cabecote);
                return NULL;
            }

            novoNo->conteudo = vetorPrincipal[i]->numeros[j];
            novoNo->prox = NULL;
            ultimo->prox = novoNo;
            ultimo = novoNo;
        }
    }

    return cabecote;
}

/* Copia para vetorAux os valores dos nos, ignorando o cabecote. */
void getDadosListaEncadeadaComCabecote(No *inicio, int vetorAux[])
{
    int i = 0;
    No *atual;

    if (inicio == NULL || vetorAux == NULL)
        return;

    atual = inicio->prox;
    while (atual != NULL) {
        vetorAux[i] = atual->conteudo;
        i++;
        atual = atual->prox;
    }
}

/* Destroi a lista encadeada e deixa o ponteiro recebido como NULL. */
void destruirListaEncadeadaComCabecote(No **inicio)
{
    No *atual;
    No *proximo;

    if (inicio == NULL || *inicio == NULL)
        return;

    atual = *inicio;
    while (atual != NULL) {
        proximo = atual->prox;
        free(atual);
        atual = proximo;
    }

    *inicio = NULL;
}

/*
Objetivo: inicializar o programa.
Carrega, quando existir, os dados gravados em dados_trabalho2.bin.
Formato: para cada posicao, grava capacidade; se capacidade > 0,
 grava quantidade e, em seguida, os elementos armazenados.
*/
void inicializar(void)
{
    int i;
    FILE *arquivo;
    int arquivoInvalido = 0;

    liberarTudo();

    arquivo = fopen("dados_trabalho2.bin", "rb");
    if (arquivo == NULL)
        return;

    for (i = 0; i < TAM; i++) {
        int capacidade = 0;
        int quantidade = 0;
        struct EstruturaAuxiliar *estrutura;

        if (fread(&capacidade, sizeof(int), 1, arquivo) != 1) {
            arquivoInvalido = 1;
            break;
        }

        if (capacidade == 0)
            continue;

        if (capacidade < 1 || !tamanhoAlocavel(capacidade)) {
            arquivoInvalido = 1;
            break;
        }

        if (fread(&quantidade, sizeof(int), 1, arquivo) != 1 ||
            quantidade < 0 || quantidade > capacidade) {
            arquivoInvalido = 1;
            break;
        }

        estrutura = (struct EstruturaAuxiliar *) malloc(sizeof(struct EstruturaAuxiliar));
        if (estrutura == NULL) {
            arquivoInvalido = 1;
            break;
        }

        estrutura->numeros = (int *) malloc((size_t) capacidade * sizeof(int));
        if (estrutura->numeros == NULL) {
            free(estrutura);
            arquivoInvalido = 1;
            break;
        }

        estrutura->capacidade = capacidade;
        estrutura->quantidade = quantidade;

        if (quantidade > 0 &&
            fread(estrutura->numeros, sizeof(int), (size_t) quantidade, arquivo) != (size_t) quantidade) {
            free(estrutura->numeros);
            free(estrutura);
            arquivoInvalido = 1;
            break;
        }

        vetorPrincipal[i] = estrutura;
    }

    fclose(arquivo);

    /* Evita manter uma carga parcial caso o arquivo esteja corrompido. */
    if (arquivoInvalido)
        liberarTudo();
}

/*
Objetivo: finalizar o programa, salvar os dados e liberar toda a memoria.
Se nao for possivel abrir o arquivo para gravacao, a memoria ainda e liberada.
*/
void finalizar(void)
{
    int i;
    int gravacaoOk = 1;
    FILE *arquivo = fopen("dados_trabalho2.bin", "wb");

    if (arquivo != NULL) {
        for (i = 0; i < TAM && gravacaoOk; i++) {
            int capacidade = 0;
            int quantidade = 0;

            if (vetorPrincipal[i] != NULL) {
                capacidade = vetorPrincipal[i]->capacidade;
                quantidade = vetorPrincipal[i]->quantidade;
            }

            if (fwrite(&capacidade, sizeof(int), 1, arquivo) != 1) {
                gravacaoOk = 0;
                break;
            }

            if (capacidade > 0) {
                if (fwrite(&quantidade, sizeof(int), 1, arquivo) != 1) {
                    gravacaoOk = 0;
                    break;
                }

                if (quantidade > 0 &&
                    fwrite(vetorPrincipal[i]->numeros, sizeof(int),
                           (size_t) quantidade, arquivo) != (size_t) quantidade) {
                    gravacaoOk = 0;
                    break;
                }
            }
        }

        if (fclose(arquivo) != 0)
            gravacaoOk = 0;

        if (!gravacaoOk)
            fprintf(stderr, "Aviso: ocorreu um erro ao gravar dados_trabalho2.bin.\n");
    } else {
        fprintf(stderr, "Aviso: nao foi possivel abrir dados_trabalho2.bin para gravacao.\n");
    }

    liberarTudo();
}

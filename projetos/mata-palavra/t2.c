// l123b - t2
// programa para digitação de palavras
// Luiza Manoelle
// 2023-12-10

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include "tecla.h"
#include "tela.h"

#define TEMPO 30
#define NUM_PALAVRAS 920
#define TAMANHO_PALAVRA 16
#define LIN 10

// declaração das funções
bool preenche_matriz(int lin, char matriz[lin][TAMANHO_PALAVRA]);
int seleciona_palavra(int t, char v[t][TAMANHO_PALAVRA], char letra);
int reordena_matriz(int lin, char matriz[lin][TAMANHO_PALAVRA], int posicao);
void exibe_jogo(double tempo_restante, int lin, char matriz[lin][TAMANHO_PALAVRA], int posicao_selecionada);
void processa_tecla(int lin, char Mat2[lin][TAMANHO_PALAVRA], int *posicao, int *linhas_restantes);
bool testa(char *palavra);
void remove_primeira_letra(char v[], char letra);
void jogo();

int main()
{
    //inicia a tela e teclado
    tela_ini();
    tecla_ini();
    
    srand(time(0));

//inicia o jogo
    jogo();

//encerra a tela e o teclado
    tecla_fim();
    tela_fim();

    return 0;
}

void jogo()
{
    int lin = LIN;
    int linhas_restantes = lin;
    char Mat2[LIN][TAMANHO_PALAVRA];
    // posição inicial da palavra selecionada (-1 significa nenhuma palavra selecionada)
    int posicao = -1;
    double inicio = tela_relogio();
    double tempo_restante = TEMPO;

    // preenchimento da matriz com palavras válidas
    if (preenche_matriz(lin, Mat2))
    {
        // loop principal do jogo

        while (tempo_restante > 0 && linhas_restantes > 0)
        {
            // calcula o tempo restante.
            tempo_restante = TEMPO - (tela_relogio() - inicio);
            // exibe o jogo na tela com o tempo restante
            exibe_jogo(tempo_restante, lin, Mat2, posicao);
            // processamento da tecla pressionada pelo jogador
            processa_tecla(lin, Mat2, &posicao, &linhas_restantes);
        }
    }
}

bool preenche_matriz(int lin, char matriz[lin][TAMANHO_PALAVRA])
{
    //variavel de controle
    int indexPalavra;
    //matriz que ira armazenar todas as palavras do arquivo
    char Mat[NUM_PALAVRAS][TAMANHO_PALAVRA];

    srand(time(NULL));

    //testa se o arquivo foi aberto
    FILE *file = fopen("palavras.txt", "r");
    if (file == NULL)
    {
        perror("Erro ao abrir o arquivo");
        return false;
    }

    //lê as palavras do arquivo para a matriz
    for (int j = 0; j < NUM_PALAVRAS; j++)
    {
        //vai preenchendo a matriz e verificando se é valido;
        if (fgets(Mat[j], TAMANHO_PALAVRA, file) == NULL)
        {
            fprintf(stderr, "Erro ao ler do arquivo");
            fclose(file);
            return false;
        }
    }

//fecha o arquivo
    fclose(file);

    // preenche a matriz recebida como parametro com palavras válidas
    for (int i = 0; i < lin; i++)
    {
        do
        {
            indexPalavra = rand() % NUM_PALAVRAS; // escolhe um índice aleatório
            strcpy(matriz[i], Mat[indexPalavra]); // copia a palavra para a matriz
        } while (!testa(matriz[i]));              // testa se a palavra é válida
    }

    return true;
}

//função para testar se a palavra é válida
bool testa(char *palavra)
{
    //for que percorre até o ultimo caracter da palavra (nulo)
    for (int i = 0; palavra[i] != '\0'; i++)
    {
        if ((palavra[i] < 'a' || palavra[i] > 'z') && (palavra[i] != '\n'))
        {
            return false; // retorna falso se a palavra contém caracteres inválidos
        }
        if (palavra[i] >= 'A' && palavra[i] <= 'Z')
        {
            palavra[i] += 'a' - 'A'; // converte letras maiúsculas para minúsculas
        }
    }
    return true; // retorna verdadeiro se a palavra é válida
}


int seleciona_palavra(int t, char v[t][TAMANHO_PALAVRA], char letra)
{
    for (int i = 0; i < t; i++)
    {
        if (v[i][0] == letra)
        {
            return i; // retorna a posição da palavra se a letra corresponder à primeira letra da palavra
        }
    }
    return -1; // retorna -1 se a letra não foi encontrada
}

int reordena_matriz(int lin, char matriz[lin][TAMANHO_PALAVRA], int posicao)
{
    // determina o índice da última posição da matriz
    int ultima_posicao = lin - 1;
    // verifica se a posição é válida para reordenar a matriz
    if (posicao >= 0 && posicao < lin - 1)
    {
        // variável temporária para armazenar a palavra selecionada
        char palavra_selecionada[TAMANHO_PALAVRA];
        // copia a palavra selecionada para a variável temporária
        strcpy(palavra_selecionada, matriz[posicao]);

//realoca todas as palavras após a posicao
        for (int i = posicao; i < ultima_posicao; i++)
        {
            // substitui a palavra na posição 'i' pela palavra na posição 'i + 1'
            strcpy(matriz[i], matriz[i + 1]);
        }
        // coloca a palavra selecionada na última posição da matriz
        strcpy(matriz[ultima_posicao], palavra_selecionada);

        return ultima_posicao;
    }
    else if (posicao == lin - 1)
    {
        return posicao; // se a posição já for a última, retorna a própria posição
    }
    return -1; // se a posição for inválida, retorna -1
}

void exibe_jogo(double tempo_restante, int lin, char matriz[lin][TAMANHO_PALAVRA], int posicao_selecionada)
{
    tela_limpa();

    // pega o número de linhas e colunas da tela
    int nlin = tela_nlin();
    int ncol = tela_ncol();
    // calcula a posição central das linhas para exibição
    int linhas_centro = (nlin - LIN) / 2;

    for (int i = 0; i < lin; i++)
    {
        // calcula a posição central das colunas para exibição
        int colunas_centro = (ncol - strlen(matriz[i])) / 2;
        tela_lincol(linhas_centro + i, colunas_centro);

//se for a palavra selecionada muda de cor
        if (i == posicao_selecionada)
        {
            tela_cor_letra(0, 255, 0);
        }
        else
        {
            tela_cor_letra(0, 0, 255); //se nao fica normal
        }
//exibe a matriz
        printf("%s\n", matriz[i]);
    }
//exibição do relogio
    tela_cor_letra(255, 255, 255);
    int linha_relogio = linhas_centro + lin + 2; //calcula logo abaixo da matriz
    int colunas_centro_relogio = (ncol - strlen("Tempo restante: 0.0s")) / 2; //calcula o tamanho da frase pra centralizar
    tela_lincol(linha_relogio, colunas_centro_relogio);
    if (tempo_restante < 5)
    {
        tela_cor_letra(200, 0, 0); //se tiver pouco tempo fica em vermelho
    }
    printf("Tempo restante: %.1fs", tempo_restante);

    tela_atualiza();
}

void processa_tecla(int lin, char Mat2[lin][TAMANHO_PALAVRA], int *posicao, int *linhas_restantes)
{
    char l; // letra digitada

    while (true)
    {
        l = tecla_le_char(); // armazena a tecla pressionada pelo jogador
        if (l != '\0')
        {
            if (*posicao < 0)
            {
                *posicao = seleciona_palavra(lin, Mat2, l);      // retorna a posição que está a palavra selecionada
                *posicao = reordena_matriz(lin, Mat2, *posicao); // coloca a palavra selecionada para a última linha da matriz
            }
            if (*posicao >= 0)
            {
                remove_primeira_letra(Mat2[*posicao], l); //remove as letras
                if (Mat2[*posicao][0] == '\0')
                {
                    *linhas_restantes -= 1; // decrementa o número de linhas restantes na matriz quando a palavra for totalmente digitada
                    *posicao = -1;
                }
                break;
            }
        }
        else
        {
            // se nenhum caractere foi digitado, retorna
            return;
        }
    }
}

void remove_primeira_letra(char v[], char letra)
{
    //pega o tamanho do vetor que foi passado
    int tam = strlen(v);
    //se for a primeira letra 
    if (v[0] == letra)
    {
        //variaveis de auxilio
        int i = 0;
        char x;
        //percorre toda a palavra 
        while (v[i] != '\0')
        {
            x = v[i];  // armazena o caractere atual em 'x'
            v[i] = v[i + 1]; // move o próximo caractere para a posição inicial
            v[i + 1] = x; // coloca o caractere armazenado ('x') na próxima posição
            //se chegou no final quebra imediatamente
            if (v[i] == '\0')
            {
                break;
            }

            i++;
        }
        //coloca o caractere nulo na última posição para reduzir o tamanho da string
        v[tam - 1] = '\0';

        //se a string tiver só um caracter e foi digitada a primeira letra já, "encerra" a string
        if (v[1] == '\0')
        {
            v[0] = '\0';
        }
    }
}

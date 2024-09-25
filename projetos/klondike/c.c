#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "assert.h"
#include "tela.h"
#include "tecla.h"

#define MAX_CHAR_CMD 2

//estruturas
typedef enum {
  ouros,
  copas,
  espadas,
  paus
} naipe_t;

typedef enum { 
  as = 1,
  dois = 2,
  tres = 3,
  quatro = 4,
  cinco = 5,
  seis = 6,
  sete = 7,
  oito = 8,
  nove = 9,
  dez = 10,
  valete = 11,
  dama = 12,
  rei =13
} valor_t;

typedef enum {
  vermelho,
  preto
} cor_t;

typedef struct {
  valor_t valor;
  naipe_t naipe;
  bool aberta; //true aberta, false fechada.
} carta_t;

typedef struct {
  int n_cartas;
  carta_t cartas[52];
} pilha_t;

typedef struct {
    int x, y; // coordenadas x e y da pilha
} Coordenadas;

typedef struct {
    pilha_t monte;
    pilha_t descarte;
    pilha_t saida[4];
    pilha_t principal[7];
    bool terminado;
    int pontos;
    double tempo_inicio;
    double tempo_fim;

    //coordenadas para as pilhas
    Coordenadas coordenadas_monte;
    Coordenadas coordenadas_descarte;
    Coordenadas coordenadas_saida[4];
    Coordenadas coordenadas_principal[7];

char comando[MAX_CHAR_CMD + 1];
bool ultimo_comando_ok;
} jogo_t;

//funções
bool pilha_vazia(pilha_t *p);
bool pilha_cheia(pilha_t *p);
void empilha(carta_t c, pilha_t *p);
carta_t *topo(pilha_t *p);
carta_t remove_topo(pilha_t *p);
void move(pilha_t *origem, pilha_t *destino, int num_cartas);
void abre_carta_topo(pilha_t *p);
void fecha_todas_cartas(pilha_t *p);
void esvazia_pilha(pilha_t *p);
void gera_baralho(pilha_t *p);
void embaralha_cartas(pilha_t *p);
int numero_cartas_na_pilha(pilha_t *p);
int numero_cartas_fechadas(pilha_t *p);
int numero_cartas_abertas(pilha_t *p);
carta_t retorna_carta(pilha_t *p, int posicao);
carta_t cria_carta(valor_t valor, naipe_t naipe);
naipe_t obter_naipe(carta_t carta);
valor_t obter_valor(carta_t carta);
cor_t obter_cor(carta_t carta);
void descreve_carta(carta_t c, char **descricao);

void inicializa_jogo(jogo_t *jogo);
bool pode_mover_para_saida(jogo_t *jogo, int num_pilha, carta_t carta);
bool pode_mover_para_pilha_jogo(jogo_t *jogo, int pilha_destino, carta_t carta);
bool pode_mover_tantas_cartas(jogo_t *jogo, int pilha_origem, int num_cartas);
bool ganhou(jogo_t *jogo);

bool abre_carta(jogo_t *jogo);
bool recicla_descarte(jogo_t *jogo);
bool move_descarte_para_saida(jogo_t *jogo, int num_saida);
bool move_descarte_para_pilha_jogo(jogo_t *jogo, int num_pilha);
bool move_pilha_jogo_para_saida(jogo_t *jogo, int num_pilha_jogo, int num_pilha_saida);
bool move_saida_para_pilha_jogo(jogo_t *jogo, int num_saida, int num_pilha);
bool move_tantas_cartas_pilha_para_pilha(jogo_t *jogo, int pilha_origem, int pilha_destino, int num_cartas);
bool move_pilha_para_pilha(jogo_t *jogo, int pilha_origem, int pilha_destino);
bool faz_uma_jogada(jogo_t *jogo, char *jogada);

void desenha_local(int linha, int coluna);
void desenha_carta_fechada(int linha, int coluna);
void desenha_carta_aberta(int linha, int coluna, carta_t carta);
void desenha_pilha_inicio(int lin, int col, pilha_t *pilha);
void desenha_pilha_aberta(int lin, int col, pilha_t *pilha);

double bonus(double tempo_inicio, double tempo_fim, double pontos_da_jogada) ;

void inicializa_coordenadas(jogo_t *jogo);
void desenha_tela(jogo_t *jogo);
void desenha_extras(jogo_t *jogo);
void desenha_tela_completa(jogo_t *jogo);

void processa_entrada_pelo_teclado(jogo_t *jogo);

void jogo();


int main() {

  tela_ini();
  tecla_ini(); 
  srand((unsigned)time(NULL));

  jogo();

  tecla_fim(); // Restaurando as configurações originais do terminal
  tela_fim();
  return 0;
}

////////////////////// part 1 ////////////////////////////

bool pilha_vazia(pilha_t *p) {
    return p->n_cartas == 0;
}

bool pilha_cheia(pilha_t *p){
  if (p->n_cartas == 13)  return true; // verifica se a pilha atingiu o máximo de 13 cartas
  else return false;
}

void empilha(carta_t c, pilha_t *p) {
    if (pilha_vazia(p)) {
        c.aberta = true; 
    } else {
        // verifica o status da carta no topo da pilha
        carta_t topo_pilha = *topo(p);
        c.aberta = topo_pilha.aberta; //mantem o status da nova carta igual ao do topo
    }
    p->cartas[p->n_cartas] = c;
    p->n_cartas++;
}

carta_t *topo(pilha_t *p) {
    if (pilha_vazia(p)) {
        return NULL; //retorna NULL se a pilha estiver vazia
    } else {
        return &(p->cartas[p->n_cartas - 1]); //retorna um ponteiro para a carta no topo da pilha
    }
}

carta_t remove_topo(pilha_t *p){
  carta_t c = *topo(p);
  p->n_cartas--;
  return c;
}

void move(pilha_t *origem, pilha_t *destino, int num_cartas) {
  assert(!pilha_vazia(origem));

  pilha_t auxiliar = {0}; //pilha auxiliar pra conseguir mover as cartas na ordem correta
  carta_t carta_mover; //carta auxiliar para guardar a carta que sera movida

  for (int i = 0; i < num_cartas; i++) {
      carta_mover = remove_topo(origem); //remove a carta do topo da pilha de origem e armazena ela na carta auxiliar
      carta_mover.aberta = true; //deixa a carta aberta
      empilha(carta_mover, &auxiliar); //empilha a carta na pilha auxiliar
  }
  if (!pilha_vazia(origem)) {
      origem->cartas[origem->n_cartas - 1].aberta = true;
  }
  //esvazia a pilha auxiliar e empilha as cartas na pilha de destino
  while (!pilha_vazia(&auxiliar)) {
      carta_mover = remove_topo(&auxiliar);
      empilha(carta_mover, destino);
  }
}

void abre_carta_topo(pilha_t *p) {
  assert(!pilha_vazia(p));

  carta_t *carta_topo = topo(p);
  carta_topo->aberta = true; 
}

void fecha_todas_cartas(pilha_t *p) {
  for (int i = 0; i < p->n_cartas; i++) {
    p->cartas[i].aberta = false; 
  }
}

void esvazia_pilha(pilha_t *p) {
  p->n_cartas = 0; 
}

void gera_baralho(pilha_t *p) {
    esvazia_pilha(p);

    carta_t nova_carta;

    for (naipe_t n = ouros; n <= paus; n++) {
        for (valor_t v = as; v <= rei; v++) {
            nova_carta = cria_carta(v, n);
            empilha(nova_carta, p);
        }
    }
}

void embaralha_cartas(pilha_t *p) {

  int num_cartas = p->n_cartas;

  for (int i = num_cartas - 1; i > 0; i--) {
    int j = rand() % (i + 1); 

    carta_t temp = p->cartas[i];
    p->cartas[i] = p->cartas[j];
    p->cartas[j] = temp;
  }
}

int numero_cartas_na_pilha(pilha_t *p) {
  return p->n_cartas; 
}

int numero_cartas_fechadas(pilha_t *p) {
  int cartas_fechadas = 0;

  for (int i = 0; i < p->n_cartas; i++) {
    if (!p->cartas[i].aberta) {
      cartas_fechadas++; 
    }
  }

  return cartas_fechadas; 
}

int numero_cartas_abertas(pilha_t *p) {
  int cartas_abertas = 0;

  for (int i = 0; i < p->n_cartas; i++) {
    if (p->cartas[i].aberta) {
      cartas_abertas++; 
    }
  }

  return cartas_abertas; 
}

carta_t retorna_carta(pilha_t *p, int posicao) {
  assert(posicao >= 0 && posicao < p->n_cartas);
  return p->cartas[posicao]; 
}

carta_t cria_carta(valor_t valor, naipe_t naipe) {
    carta_t nova_carta;
    nova_carta.valor = valor;
    nova_carta.naipe = naipe;
    nova_carta.aberta = false;

    return nova_carta;
}

naipe_t obter_naipe(carta_t carta) {
  return carta.naipe;
}

//retorna o valor de uma carta
valor_t obter_valor(carta_t carta) {
  return carta.valor;
}

cor_t obter_cor(carta_t carta) {
  if (carta.naipe == ouros || carta.naipe == copas) {
    return vermelho;
  } else {
    return preto;
  }
}

//**descrição é usado para acessar o valor apotando por outro ponteiro
void descreve_carta(carta_t c, char **descricao) {
  switch (c.valor) {
    //se o valor da carta for um as, é alocada uma cópia da string "A" e atribuída ao ponteiro apontado por descricao.
    case as:     *descricao = strdup("A"); break;
    case valete: *descricao = strdup("J"); break;
    case dama:   *descricao = strdup("Q"); break;
    case rei:    *descricao = strdup("K"); break;
    default: {
      *descricao = malloc(12); //aloca espaço suficiente para armazenar um número de 10 dígitos + 1 para o naipe
      sprintf(*descricao, "%d", c.valor);
      break;
    }
  }

  //armazena o tamanho da descrição
  size_t descricao_tam = strlen(*descricao);
  *descricao = realloc(*descricao, descricao_tam + 5); // 5 caracteres para o naipe

  switch (c.naipe) {
    //une o naipe com o desenho do naipe.
    case copas:   strcat(*descricao, "\u2665 "); break;
    case ouros:   strcat(*descricao, "\u2666 "); break;
    case paus:    strcat(*descricao, "\u2663 "); break;
    case espadas: strcat(*descricao, "\u2660 "); break;
  }
}
///////////////////////////// part 2////////////////////////

void inicializa_jogo(jogo_t *jogo) {
    // esvazia todas as pilhas
    esvazia_pilha(&(jogo->monte));
    esvazia_pilha(&(jogo->descarte));
    for (int i = 0; i < 4; i++) {
        esvazia_pilha(&(jogo->saida[i]));
    }
    for (int i = 0; i < 7; i++) {
        esvazia_pilha(&(jogo->principal[i]));
    }

    // preenche o monte com o baralho inteiro (fechadas)
    gera_baralho(&(jogo->monte));

    embaralha_cartas(&(jogo->monte));

    // Move cartas do monte para as pilhas principais
    for (int i = 0; i < 7; i++) {
        move(&(jogo->monte), &(jogo->principal[i]), i + 1);
    }

    //fecha todas as cartas exceto a do topo em cada pilha principal
    for (int i = 0; i < 7; i++) {
        fecha_todas_cartas(&(jogo->principal[i]));
        abre_carta_topo(&(jogo->principal[i]));
    }
  fecha_todas_cartas(&(jogo->monte));

   jogo->pontos = 0;
   jogo->tempo_inicio = tela_relogio();
   inicializa_coordenadas(jogo);

}

bool pode_mover_para_saida(jogo_t *jogo, int num_pilha, carta_t carta) {
    if (num_pilha < 0 || num_pilha >= 4) {
        return false;
    }

    pilha_t *pilha_saida = &jogo->saida[num_pilha];

    if (pilha_vazia(pilha_saida) && obter_valor(carta) == 1) {
        return true; //se a pilha de saída estiver vazia e a carta for um Ás
    } else if (!pilha_vazia(pilha_saida)) {
        carta_t topo_pilha_saida = *topo(pilha_saida);
        if (obter_naipe(carta) == obter_naipe(topo_pilha_saida) &&
            obter_valor(carta) == obter_valor(topo_pilha_saida) + 1) {
            return true; //se o naipe e o valor da carta são compatíveis com a pilha de saída
        }
    }

    return false; //caso contrário, não é possível mover a carta para a pilha de saída
}

bool pode_mover_para_pilha_jogo(jogo_t *jogo, int pilha_destino, carta_t carta) {
  if (pilha_destino < 0 || pilha_destino>= 7) {
      return false;
  }
    pilha_t *pilha_destino_jogo = &jogo->principal[pilha_destino];

    if (pilha_vazia(pilha_destino_jogo)) {
        if (obter_valor(carta) == 13) {
            return true;
        }
    } else {
        carta_t carta_topo_destino = *topo(pilha_destino_jogo);
        cor_t cor_carta_topo = obter_cor(carta_topo_destino);
        cor_t cor_carta = obter_cor(carta);

        if (cor_carta_topo != cor_carta && obter_valor(carta) + 1 == obter_valor(carta_topo_destino)) {
            return true;
        }
    }

    return false;
}

bool pode_mover_tantas_cartas(jogo_t *jogo, int pilha_origem, int num_cartas) {
    assert(!pilha_vazia(&jogo->principal[pilha_origem]));
    pilha_t *pilha_origem_jogo = &jogo->principal[pilha_origem];

    if (num_cartas > pilha_origem_jogo->n_cartas) {
        return false; 
    }

    int cartas_abertas = numero_cartas_abertas(pilha_origem_jogo);

    return cartas_abertas >= num_cartas;
}

bool ganhou(jogo_t *jogo) {
    for (int i = 0; i < 4; i++) {
        if (numero_cartas_na_pilha(&jogo->saida[i]) != 13) {
            return false; //se uma pilha de saída não tem 13 cartas, o jogador ainda não ganhou
        }
    }
    return true; //todas as quatro pilhas de saída têm 13 cartas, o jogador ganhou
}

////////////////////////// part 3 //////////////////////////////////////////////////////

bool abre_carta(jogo_t *jogo) {
    if (pilha_vazia(&jogo->monte)) {
        return false; //não é possível mover se o monte estiver vazio
    }

    carta_t carta_monte_topo = remove_topo(&jogo->monte);
    empilha(carta_monte_topo, &jogo->descarte);
    abre_carta_topo(&jogo->descarte);
    return true; //a jogada foi bem sucedida
}

bool recicla_descarte(jogo_t *jogo) {
    if (pilha_vazia(&jogo->descarte) || !pilha_vazia(&jogo->monte)) {
        return false;
    }
  int num = jogo->descarte.n_cartas;
  carta_t carta_mover;
  for (int i = 0; i < num; i++) {
      carta_mover = remove_topo(&jogo->descarte); 
      empilha(carta_mover, &jogo->monte); //empilha a carta para manter a ordem do monte.
  }
    fecha_todas_cartas(&jogo->monte);
    jogo->pontos = 0;
    return true; //a jogada foi bem sucedida
}

bool move_descarte_para_saida(jogo_t *jogo, int num_saida) {
    if (num_saida < 0 || num_saida > 3 || pilha_vazia(&jogo->descarte)) {
        return false; 
    }

    if (!pilha_vazia(&jogo->descarte)) {
        carta_t carta_descarte_topo = *topo(&jogo->descarte);
        if (pode_mover_para_saida(jogo, num_saida, carta_descarte_topo)) {
            move(&jogo->descarte, &jogo->saida[num_saida], 1);
            return true;
          jogo->tempo_fim = tela_relogio();
          jogo->tempo_inicio = tela_relogio();
          jogo->pontos += 15;
        } else return false;
    }
  return false;
}

bool move_descarte_para_pilha_jogo(jogo_t *jogo, int num_pilha) {
    if (num_pilha < 0 || num_pilha > 6 || pilha_vazia(&jogo->descarte)) {
        return false; 
    }

    if (!pilha_vazia(&jogo->descarte)) {
        carta_t carta_descarte_topo = *topo(&jogo->descarte);
        if (pode_mover_para_pilha_jogo(jogo, num_pilha, carta_descarte_topo)) {
            move(&jogo->descarte, &jogo->principal[num_pilha], 1);

         double tempo_inicio = jogo->tempo_inicio;
         double tempo_fim = jogo->tempo_fim;
          tempo_fim = tela_relogio();
            jogo->pontos += 10 + bonus(tempo_inicio, tempo_fim, 10);
            jogo->tempo_inicio = tela_relogio();
            return true; 
        } else {
            return false;
        }
    }
  return false;
}

bool move_pilha_jogo_para_saida(jogo_t *jogo, int num_pilha_jogo, int num_pilha_saida) {
    if (num_pilha_jogo < 0 || num_pilha_jogo > 6 || num_pilha_saida < 0 || num_pilha_saida > 3) {
        return false;
    }

    pilha_t *pilha_jogo_origem = &jogo->principal[num_pilha_jogo];
    pilha_t *pilha_saida_destino = &jogo->saida[num_pilha_saida];

    if (pilha_vazia(pilha_jogo_origem)) {
        return false; 
    }

    carta_t carta_topo_jogo = *topo(pilha_jogo_origem);
    if (pode_mover_para_saida(jogo, num_pilha_saida, carta_topo_jogo)) {
      jogo->tempo_fim = tela_relogio();
        move(pilha_jogo_origem, pilha_saida_destino, 1); // move uma carta da pilha do jogo para a pilha de saída
      jogo->pontos += 15;
      if(numero_cartas_abertas(pilha_jogo_origem) == 1){
        double tempo_inicio = jogo->tempo_inicio;
        double tempo_fim = jogo->tempo_fim;
          tempo_fim = tela_relogio();
        jogo->pontos += 20 + bonus(tempo_inicio, tempo_fim, 20);
      }
      jogo->tempo_inicio = tela_relogio();
        return true; 
    } else {
        return false; 
    }
}

bool move_saida_para_pilha_jogo(jogo_t *jogo, int num_saida, int num_pilha) {
    if (num_saida < 0 || num_saida > 3 || num_pilha < 0 || num_pilha > 6) {
        return false;
    }

    if (!pilha_vazia(&jogo->saida[num_saida])) {
        carta_t carta_saida_topo = *topo(&jogo->saida[num_saida]);
        if (pode_mover_para_pilha_jogo(jogo, num_pilha, carta_saida_topo)) {
          jogo->tempo_fim = tela_relogio();
            move(&jogo->saida[num_saida], &jogo->principal[num_pilha], 1);
            jogo->pontos -= 15;
          jogo->tempo_inicio = tela_relogio();
            return true; 
        } else {
            return false; 
        }
    }

    return false; //caso a pilha de saída esteja vazia
}

bool move_tantas_cartas_pilha_para_pilha(jogo_t *jogo, int pilha_origem, int pilha_destino, int num_cartas) {
    if (num_cartas <= 0 || pilha_origem < 0 || pilha_origem > 6 || pilha_destino < 0 || pilha_destino > 6) {
        return false;
    }

    pilha_t *pilha_origem_jogo = &jogo->principal[pilha_origem];
    pilha_t *pilha_destino_jogo = &jogo->principal[pilha_destino];

    if(num_cartas > 0){
    move(pilha_origem_jogo, pilha_destino_jogo, num_cartas);
      jogo->tempo_fim = tela_relogio();
      if(numero_cartas_abertas(pilha_origem_jogo) == 1){
        jogo->pontos += 20 + bonus(jogo->tempo_inicio, jogo->tempo_fim, 20);
      }
      jogo->tempo_inicio = tela_relogio();
    return true;
    } else return false;
}

bool move_pilha_para_pilha(jogo_t *jogo, int pilha_origem, int pilha_destino) {
    if (pilha_origem < 0 || pilha_origem > 6 || pilha_destino < 0 || pilha_destino > 6) {
        return false;
    }

  int cartas = 0;

    pilha_t *pilha_origem_jogo = &jogo->principal[pilha_origem];

  if(pilha_vazia(pilha_origem_jogo)){
    return false;
  }

  for(int i = numero_cartas_fechadas(pilha_origem_jogo); i < numero_cartas_na_pilha(pilha_origem_jogo); i++){
    if(pode_mover_para_pilha_jogo(jogo, pilha_destino, pilha_origem_jogo->cartas[i])){
      cartas = numero_cartas_na_pilha(pilha_origem_jogo) - i;
      break;
    }
  }

return move_tantas_cartas_pilha_para_pilha(jogo, pilha_origem, pilha_destino, cartas);
}

bool faz_uma_jogada(jogo_t *jogo, char *jogada)
{
  //ve se o jogador digitou alguma coisa
    if (jogada == NULL || jogada[0] == '\0')
    {
        return false; 
    }
//variaveis de controle
    int pilha_origem, pilha_destino;

//jogadas
    switch (jogada[0])
    {
//quando a primeira letra é m ou M
    case 'm':
    case 'M':
        //se for seguido de 'p', 'P', ou só m realiza a jogada de abrir uma carta
        if (jogada[1] == 'p' || jogada[1] == 'P' || jogada[1] == '\0')
        {

            return abre_carta(jogo);
        }
        break;
//quando a primeira letra é p ou P
    case 'p':
    case 'P':
        //se for seguido de 'm', 'M', ou só p realiza a jogada de reciclar o descarte
        if (jogada[1] == 'm' || jogada[1] == 'M' || jogada[1] == '\0')
        {   
            return recicla_descarte(jogo);
        }
          //se for de 'pa' até 'pd'
        else if (jogada[1] >= 'a' && jogada[1] <= 'd')
        {  
            //armazena qual a saida
            pilha_destino = jogada[1] - 'a';
          //realiza a jogada de mover carta do descarte para a saida
            return move_descarte_para_saida(jogo, pilha_destino);
        }
          //se for de 'p1' a 'p7'
        else if (jogada[1] >= '1' && jogada[1] <= '7')
        {  
          //armazena qual a pilha
            pilha_destino = jogada[1] - '1';
          //realiza a jogada de mover carta do descarte para a pilha do jogo
            return move_descarte_para_pilha_jogo(jogo, pilha_destino);
        }
        break;

      //quando a primeira jogada for de '1' a '7'
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
      //se for seguida de 'a' - 'd'
        if (jogada[1] >= 'a' && jogada[1] <= 'd')
        { 
          //armazena pilhas
            pilha_destino = jogada[1] - 'a';
            pilha_origem = jogada[0] - '1';
          //faz a jogada de mover a carta de uma pilha para uma saida
            return move_pilha_jogo_para_saida(jogo, pilha_origem, pilha_destino);
        }
        // se for seguida de '1' - '7' faz a jogada de mover entre pilhas 
        else if (jogada[1] >= '1' && jogada[1] <= '7')
        { 
            pilha_origem = jogada[0] - '1';
            pilha_destino = jogada[1] - '1';
            return move_pilha_para_pilha(jogo, pilha_origem, pilha_destino);
        }
        break;

      //quando a primeira jogada for de 'a' a 'd'
    case 'a':
    case 'b':
    case 'c':
    case 'd':
      //se for seguida de '1' - '7'
        if (jogada[1] >= '1' && jogada[1] <= '7')
        { 
            pilha_destino = jogada[0] - 'a';
            pilha_origem = jogada[1] - '1';
          //realiza a jogada de mover da saida para uma pilha do jogo
            return move_saida_para_pilha_jogo(jogo, pilha_origem, pilha_destino);
        }
        break;

      case 'f':
      case 'F':
          jogo->terminado = true;
          return true; 
    }

    return false; // caso não corresponda a nenhuma jogada válida
}

////////////////////////////////////// part 4 //////////////////////////////////////

void desenha_local(int linha, int coluna){
    tela_lincol(linha, coluna);

    printf("\u2554"); //canto superior esquerdo
    for (int i = 0; i < 7; ++i) printf("\u2550"); // linha horizontal superior
    printf("\u2557\n"); // canto superior direito

    for (int i = 0; i < 4; ++i) {
        tela_lincol(linha + i + 1, coluna); // muda para a próxima linha
        printf("\u2551       \u2551\n"); // espaço para a carta
    }

    tela_lincol(linha + 5, coluna);
    printf("\u255A"); // canto inferior esquerdo
    for (int i = 0; i < 7; ++i) printf("\u2550"); // linha horizontal inferior
    printf("\u255D\n"); // canto inferior direito
}

void desenha_carta_fechada(int linha, int coluna) {
    tela_lincol(linha, coluna); // Define a posição inicial do desenho

    // Define a cor do fundo como branco (código ANSI para branco)
    tela_cor_fundo(245, 240, 245);

    // Define a cor do texto como azul
    tela_cor_letra(100, 0, 245);

    // Desenho de um local para carta fechada
    printf("\u2554"); // Canto superior esquerdo
    for (int i = 0; i < 7; ++i) printf("\u2550"); // Linha horizontal superior
    printf("\u2557\n"); // Canto superior direito

    for (int i = 0; i < 4; ++i) {
        tela_lincol(linha + i + 1, coluna); // Muda para a próxima linha
        printf("\u2551\u265A\u265A\u265A\u265A\u265A\u265A \u2551\n"); // Espaço para a carta
    }

    tela_lincol(linha + 5, coluna);
    printf("\u255A"); // Canto inferior esquerdo
    for (int i = 0; i < 7; ++i) printf("\u2550"); // Linha horizontal inferior
    printf("\u255D\n"); // Canto inferior direito

    // Retorna a cor ao normal após o desenho
    tela_cor_normal();
}


void desenha_carta_aberta(int linha, int coluna, carta_t carta) {
    //pega a descricao
    char *descricao;
    descreve_carta(carta, &descricao);

    // define a posição inicial do desenho
    tela_lincol(linha, coluna);

    // define as cores para desenho da carta
    tela_cor_fundo(255, 255, 255); // fundo branco
    if (obter_cor(carta) == vermelho) {
        tela_cor_letra(255, 0, 0); // letra vermelha para cartas vermelhas
    } else {
        tela_cor_letra(0, 0, 0); // letra preta para outras cartas
    }

    // desenha o contorno da carta
    printf("\u250C\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2510\n"); // linha superior

  tela_lincol(linha + 1, coluna);
      if(obter_valor(carta) == 10){
        printf("\u2502  %s \u2502\n", descricao);
      } else
  printf("\u2502  %s  \u2502\n", descricao);

  tela_lincol(linha + 2, coluna);
  for (int i = 0; i < 3; ++i) {
      tela_lincol(linha + i + 2, coluna); // muda para a próxima linha
      printf("\u2502       \u2502\n"); // espaço para a carta
  }

  tela_lincol(linha + 5, coluna);
    printf("\u2514\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2518\n"); // linha inferior

    // retorna a cor ao normal após o desenho
    tela_cor_normal();

    // libera a memória alocada para a descrição da carta
    free(descricao);
}

void desenha_pilha_inicio(int lin, int col, pilha_t *pilha) {
    if (pilha_vazia(pilha)) {
        desenha_local(lin, col);
    } else {
        carta_t carta_topo = *topo(pilha);
        if (carta_topo.aberta) {
            desenha_carta_aberta(lin, col, carta_topo);
        } else {
            desenha_carta_fechada(lin, col);
        }
    }
}


void desenha_pilha_aberta(int lin, int col, pilha_t *pilha) {
    if (pilha_vazia(pilha)) {
        // se a pilha estiver vazia, desenhe o local vazio
        desenha_local(lin, col);
    } else {
        // desenha as cartas com uma distância de 2 linhas
        for (int i = 0; i < pilha->n_cartas - 1; i++) {
            if (pilha->cartas[i].aberta) {
                // se a carta estiver aberta, desenha a carta aberta
                desenha_carta_aberta(lin + i * 2, col, pilha->cartas[i]);
            } else {
                // se a carta estiver fechada, desenha a carta fechada
                desenha_carta_fechada(lin + i * 2, col);
            }
        }
        // desenha a carta do topo
        if (pilha->cartas[pilha->n_cartas - 1].aberta) {
            // carta do topo está aberta
            desenha_carta_aberta(lin + (pilha->n_cartas - 1) * 2, col, pilha->cartas[pilha->n_cartas - 1]);
        } else {
            // carta do topo está fechada
            desenha_carta_fechada(lin + (pilha->n_cartas - 1) * 2, col);
        }
    }
}

///////////////////// part 5 /////////////////////

double bonus(double tempo_inicio, double tempo_fim, double pontos_da_jogada) {

    double tempo_jogada = tempo_fim - tempo_inicio;
    double bonus = 0.0;

    if (tempo_jogada < 7.0) {
        bonus = (7.0 - tempo_jogada) / 7.0 * 3.0 * pontos_da_jogada;
    }

    return bonus;
}



/////////////////////////part 6 /////////////////////////////////

void inicializa_coordenadas(jogo_t *jogo) {

    // define as coordenadas para o monte e descarte
    jogo->coordenadas_monte.x = 5;   jogo->coordenadas_monte.y = 5;
    jogo->coordenadas_descarte.x = 5; jogo->coordenadas_descarte.y = 18;

    // coordenadas iniciais para as saidas
    for (int i = 0; i < 4; i++) {
        jogo->coordenadas_saida[i].x = 5; 
        jogo->coordenadas_saida[i].y = 35 + (i * 10); 
    }

    // Coordenadas iniciais para as pilhas principais
    for (int i = 0; i < 7; i++) {
        jogo->coordenadas_principal[i].x = 15; 
        jogo->coordenadas_principal[i].y = 5 + (i * 10); 
    }
}

void desenha_tela(jogo_t *jogo) {

    // desenha o monte e descarte
    desenha_pilha_inicio(jogo->coordenadas_monte.x, jogo->coordenadas_monte.y, &jogo->monte); 
    desenha_pilha_inicio(jogo->coordenadas_descarte.x, jogo->coordenadas_descarte.y, &jogo->descarte); 

    // desenha as saídas
    for (int i = 0; i < 4; i++) {
      desenha_pilha_inicio(jogo->coordenadas_saida[i].x, jogo->coordenadas_saida[i].y, &jogo->saida[i]);
    }

    //desenha as pilhas principais
    for (int i = 0; i < 7; i++) {
        desenha_pilha_aberta(jogo->coordenadas_principal[i].x, jogo->coordenadas_principal[i].y, &jogo->principal[i]);
    }
}

void desenha_extras(jogo_t *jogo) {
  for (int i = 0; i < 4; i++) {
    tela_lincol(4, 39 + (i * 10));  // posição acima das pilhas de saída
    printf("%c", 'a' + i);
  }

  tela_lincol(4, 9);  // posição acima do monte
  printf("m");

  tela_lincol(4, 22);  // posição acima do descarte
  printf("p");

  for (int i = 0; i < 7; i++) {
    tela_lincol(14, 9 + (i * 10));  // posições acima das pilhas principais
    printf("%d", i + 1);
  }

  // mostrando a pontuação
  tela_lincol(1, 60);
  printf("Pontuação: %d", jogo->pontos);

  tela_lincol(1, 10); // Posição para exibir as jogadas
  printf("Jogada:");
  printf(" %s", jogo->comando);
}

void desenha_tela_completa(jogo_t *jogo) {


  tela_limpa();

    desenha_tela(jogo);
    desenha_extras(jogo);
  tela_atualiza();

}

///////////////////////////part 7 /////////////////////////////////

void processa_entrada_pelo_teclado(jogo_t *jogo)
{
  char tecla;

  tecla = tecla_le_char(); // ou tela_tecla();
  if (tecla == '\0') return;

  int nchar = strlen(jogo->comando);

  switch (tecla) {
    case '\b':
      if (nchar > 0) {
        jogo->comando[--nchar] = '\0';
      }
      break;
    case '\n':
      if (nchar > 0) {
        jogo->ultimo_comando_ok = faz_uma_jogada(jogo, jogo->comando);
        jogo->comando[0] = '\0';
      }
      break;
    default:
      if (nchar < MAX_CHAR_CMD) {
        jogo->comando[nchar++] = tecla;
        jogo->comando[nchar] = '\0';
      }
      break;
  }
}

///////////////////////// part 8 //////////////////////////

void jogo() {


  jogo_t jogo;
  inicializa_jogo(&jogo);

  jogo.terminado = false; 
  jogo.ultimo_comando_ok = true; 

  while (!jogo.terminado) {
    processa_entrada_pelo_teclado(&jogo);
  desenha_tela_completa(&jogo);
  if( ganhou(&jogo)){
  jogo.terminado = true;
   }
  }

}

////////////////////////fim//////////////////////////////////////
# Klondike

## Objetivos do Projeto:
  **Desenvolver um jogo de paciência:** O projeto envolve a implementação de um jogo de paciência com funcionalidades como movimentação de cartas entre pilhas principais e pilhas de saída.

  **Controlar as cartas e pontuação:** O jogo gerencia cartas em diferentes pilhas (principal, monte, descarte, e saída), atualiza o status do jogo com base nas jogadas e controla a pontuação do jogador.

  **Interação com o jogador:** O jogo deve permitir que o jogador insira comandos por meio do teclado para fazer jogadas e mover cartas.
Interface gráfica via terminal: O código utiliza uma biblioteca de controle de tela (possivelmente tela.h) para desenhar as pilhas de cartas e outros elementos na tela, além de atualizar o display conforme as ações do jogador.

  **Concluir o jogo:** O jogo termina quando todas as condições de vitória forem alcançadas (como mover todas as cartas para as pilhas de saída) ou quando o jogador não tiver mais movimentos possíveis.

## Requisitos do Projeto:
  **Manipulação de Cartas:** O jogo deve manipular pilhas de cartas, permitindo que o jogador mova cartas de uma pilha para outra, respeitando regras específicas (por exemplo, mover uma carta para a pilha de saída apenas se ela seguir a ordem correta de naipe e valor).
  
  **Controle de Teclado:** O jogador deve inserir comandos pelo teclado, e o programa deve processar essas entradas corretamente, permitindo movimentos como:
    - Mover cartas entre pilhas.
    - Verificar se o movimento é válido com base no estado das pilhas.
    
  **Atualização Visual:** O programa deve constantemente redesenhar o estado do jogo na tela, exibindo as pilhas de cartas, o monte, o descarte, as pilhas de saída e a pontuação.
  
  **Pontuação e Temporizador:** O jogo deve calcular e exibir a pontuação, bem como controlar o tempo de jogo desde o início.
  
 **Verificação de Vitória:** O jogo precisa identificar quando o jogador atingiu a condição de vitória (por exemplo, ao organizar todas as cartas nas pilhas de saída).

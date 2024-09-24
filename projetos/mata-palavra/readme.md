# Mata Palavras #

## Objetivos ##
Este projeto tem como objetivo desenvolver um jogo de digitação onde o jogador precisa digitar palavras corretamente dentro de um tempo limitado. O jogo visa aprimorar as habilidades de digitação dos usuários, proporcionando uma experiência interativa e desafiadora. As palavras aparecem em uma tela e o jogador deve digitá-las corretamente antes que o tempo acabe.

## Os principais objetivos do jogo incluem: ##

Treinamento de digitação rápida e precisa: O jogador deve selecionar e digitar palavras corretamente, melhorando a velocidade de digitação.
Pressão do tempo: O jogador tem um tempo limitado para digitar todas as palavras da tela, incentivando o desenvolvimento de habilidades sob pressão.
Interface visual intuitiva: O jogo apresenta uma interface limpa, com destaque para a palavra selecionada e o tempo restante.
Requisitos do Projeto:
Matriz de palavras: O jogo deve carregar um conjunto de palavras de um arquivo de texto (palavras.txt) e exibir uma seleção dessas palavras em uma matriz na tela.
Seleção e remoção de palavras: O jogador deve ser capaz de selecionar uma palavra ao digitar sua primeira letra e, em seguida, remover cada letra sucessivamente ao continuar digitando.
Validação de palavras: Somente palavras válidas (compostas por letras minúsculas de a a z) devem ser exibidas e manipuladas.
Temporização: O jogo possui um tempo limite de 30 segundos. A contagem regressiva deve ser exibida na tela e mudar de cor (vermelho) quando restarem menos de 5 segundos.
Exibição gráfica: O programa usa as bibliotecas tela.h e tecla.h para controlar a exibição do jogo e a captura de teclas, permitindo que o jogador interaja com o jogo através de uma interface visual.
Reordenação da matriz: Quando uma palavra é selecionada, ela deve ser movida para a última linha da matriz. Palavras que forem completamente digitadas são removidas e substituídas.
Finalização do jogo: O jogo termina quando o tempo acaba ou quando todas as palavras foram completamente digitadas.

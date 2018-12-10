# Ant Collony
## Trabalho da disciplina SSC0713 - Sistemas Evolutivos e Aplicados à Robótica
Implementação de um algoritmo de Inteligêcia de Enxame e algoritmo Evolutivo
###Descrição
Cada um dos 12 ninhos gerará formigas que saem da posição inicial definida no arquivo ui.c com o objetivo de chegar à posição final definida no arquivo ui.c. O número de formigas é definido no arquivo ui.c.

Devido a presença de ruído armazena-se as notas obtidas pelos ninhos nas avaliações anteriores e utiliza-se a média das últimas notas para definir o melhor de todos. Para alterar a quantidade de últimas notas armazenadas pode-se alterar o valor para MEMORY definido no arquivo ui.c.

As informações de todas as gerações são impressas no terminal assim como em um arquivo de saída denomindado output.txt. A cada 50 gerações é foraçada a escrita no arquivo de saída.

### Pré Requisitos
Instalação da biblioteca:
 * [glut](https://www.opengl.org/resources/libraries/glut/)
### Comandos Da Interface Gráfica
Teclas com funções definidas quando a interface gráfica está aberta:
 * **g -** Ativar/Desativar Interface Gráfica;
 * **p -** Parar/Continuar Execução quando a próxima geração acabar;
 * **q -** Salvar e sair.

### Parâmetros Do Programa
Parâmetros aceitos pelo programa:
 * **_-i inputFile_ :** nome do aquivo de entrada com os valores para iniciar a população;
 * **_-ph pheromone_ :** quantidade de feromônio dos ninhos, se utilizado não evolui esse gene;
 * **_-mx maximumPheromone_ :** quantidade máxima de feromônio em uma posição do campo, se utilizado não evolui esse gene;
 * **_-ig ignoreChance_ :** chance de ignorar a quantidade de feromônio no campo, valor de 0 a 100, se utilizado não evolui esse gene;
 * **_-tn turnChance_ :** chance de ao encontrar uma borda virar 180 graus, valor de 0 a 100, se utilizado não evolui esse gene;
 * **_-at antena1 antena2_ :** formato das antenas valores de 0 a 255, se utilizado não evolui esse gene.

 A ordem em que os parâmetros são utilizados influencia na inicialização dos ninhos. Caso o arquivo de entrada seja passado após os outros parâmetros vale o valor no arquivo. Caso os parâmetros sejam passados após o arquivo, os valores defindos nos parâmetros sobrepõe os valores do arquivo. Passar o arquivo como parâmetro não influecia genes podem evoluir.
### Formato Do Arquivo De entrada
Assim como no arquivo de modelo cada valor deve estar em uma linha, os valores são na seguinte ordem: feromônio,quantidade máxima de feromônio, antena 1, antena 2, chance de ignorar, chance de virar.

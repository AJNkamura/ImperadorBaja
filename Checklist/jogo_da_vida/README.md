# Jogo da vida de Conway
- É modelo matemático que simula a evolução de células em uma matriz baseada em regras simples:
    - Uma célular viva morre de solidão de tiver menos que 2 vizinhas;
    - Uma célula viva morre por superpopulação se tiver mais que 3 vizinhas;
    - Uma célular viva sobrevive se tiver duas ou 3 vizinhas;
    - Uma célular morta revive se tiver exatamente 3 vizinhas;


Este repositório contém o código em C e arquivos .txt com as matrizes originais para iniciar o jogo. 
A ideia é que existam 2 matrizes: uma matriz "atual" e uma matriz "cópia". A matriz cópia é construida com base na "atual" quando se aplica as regras do jogo. Após ser impressa no console, a matriz "cópia" é copiada de volta para a matriz "atual", tornando-se o novo estado da matriz para a próxima iteração.

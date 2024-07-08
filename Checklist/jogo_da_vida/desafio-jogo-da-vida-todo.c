#include <stdio.h>
#include <stdio.h>

#define ARQUIVO_PADRAO "teste02.txt" // arquivo contendo o padrão inicial
#define ITERACOES 20
#define CLEAR "cls"
#define WINDOWS

int **alocaMatriz(int nl, int nc);
void destroiMatriz(int **m, int nl);
int ** leMatrizDoArquivo(char *nome, int *nl, int *nc);
void imprime_matriz(int **m, int nl, int nc);
void copia_matriz(int **fonte, int **dest, int l, int c);
int n_vizinhas(int **matriz, int i, int j);
void jogo(int **matriz, int nl, int nc, int iter);


int main()
{
    int **m, nl, nc;

    //Preenchendo a matriz conforme o conteúdo da matriz presente no txt
    m = leMatrizDoArquivo(ARQUIVO_PADRAO, &nl, &nc);
    jogo(m, nl, nc, ITERACOES);

    return 0;
}

/*
 * Funcao que executa uma quantidade determinada de vezes as regras do jogo
 * Parâmetros: int **matriz: referencia para a matriz de dados
 *             int nl, int nc: dimensão da matriz
 *             int iter: quantidade de iteracoes do jogo
 *
 * Valor de retorno: quantidade de celulas vivas na vizinhanca do ponto de coordenada [i][j]
*/
void jogo(int **matriz, int nl, int nc, int iter)
{
    int **copia = alocaMatriz(nl, nc),
                i, j, k,
                vizinhas;

    /* É necessario copiar a matriz original para que as comparações não sejam feitas com o novo
       padrão gerado.*/
    copia_matriz(matriz, copia, nl, nc);

    for(k=0; k<iter; k++)
    {
        for(i=1;i<nl-1;i++) //percorre linhas ignorando as bordas
        {
            for(j=1;j<nc-1;j++) ////percorre colunas ignorando as bordas
            {
                //TODO: complemente as condicoes de acordo com a regra B3/S23 [DONE!]
                vizinhas = n_vizinhas(matriz,i,j);
                if(matriz[i][j]==1 && (vizinhas > 3 || vizinhas <2 )) //morre
                    copia[i][j] = 0;
                else if(matriz[i][j]==0 && vizinhas == 3) //resuscita
                    copia[i][j] = 1;
            }
        }
        /* É necessrio copiar a nova matriz gerada(chamada de "cópia") para que ela seja usada como
           a matriz "original" nas comparações da próxima iteração.*/
        copia_matriz(copia, matriz, nl, nc);

        system(CLEAR);
        imprime_matriz(matriz, nl, nc);
       // #ifdef WINDOWS
        Sleep (500);
        //#else
        //sleep (1);
        //#endif
    }
}

/*
 * Funcao que retorna o numero de vizinhas vivas da celula na posicao [i][j]
 * Parâmetros: int **matriz: referencia para a matriz de dados
 *             int i, int j: coordenadas do ponto de interesse
 *
 * Valor de retorno: quantidade de celulas vivas (ie, com valor == 1) na vizinhanca
 *                   do ponto de coordenada [i][j]
*/
int n_vizinhas(int **matriz, int i, int j)
{
	int vizinhas_vivas = 0, x, y;

	//loop itera pelas 9 celulas no entorno da celula atual (incluindo ela mesma)
	for(x = -1; x <= 1; x++)
		for(y = -1; y <= 1; y++)
			/*como as celulas vivas sao 1 e as mortas sao 0,
			a soma destas posicoes consiste na quantidade de
			celulas vivas nesta região 3 x 3*/
			vizinhas_vivas += matriz[i+x][j+y];

	return vizinhas_vivas - matriz[i][j]; 	/*desconta a celula atual da contagem
	                                         (pois se for viva não deve contabilizar)*/
}

/*
* Função que aloca dinamicamente uma matriz de inteiros de nl linhas por nc colunas
*/
int** alocaMatriz(int nl, int nc)
{
    int i;
    int** matriz;

    matriz = (int**) malloc (nl * (sizeof(int*)));
    //retorna o end. da 1a pos. do vetor de ponteiros alocado

    /*percorre o vetor de ponteiros alocando um "vetor linha" p/ cada posicao
      e preenchendo com end de mem. de 1a pos. de cada vetor alcoado.*/
    for(i=0; i<nl; i++)
    {
        matriz[i] = (int*) malloc (nc * sizeof(int));
    }

    return matriz;
}

/*
* Função que desaloca memoria
*/
void destroiMatriz(int **m, int nl)
{
    int i;

    //percorre o vetor de ponteiros para desalocar cada "vetor linha" criado
    for(i=0; i<nl; i++)
    {
        free(m[i]);
    }

    //desaloca o vetor de ponteiros
    free(m);
}

/*
 * Funcao que le o conteudo de um arquivo txt e carrega os dados em uma matriz
 * Parâmetros: char *nome: string com o nome do arquivo txt
 *             int *nl, int *nc: retorno por referência quantidade de linhas e colunas
 *
 * O arquivo txt tem a seguinte formatação:
 * - na primeira linha, dois inteiros representando o número de linhas (nl) e colunas (nc), respectivamente
 * - depois, nl linhas com nc colunas com inteiros representando os dados de teste
 *
 * Valor de retorno: referencia para uma matriz alocada dinamicamente cujo conteudo foi
 *                   carregado do arquivo .txt
*/
int ** leMatrizDoArquivo(char *nome, int *nl, int *nc)
{
    int i, j;
    FILE *fp;
    int** m;

    // abre o arquivo e lê a quantidade de linhas e colunas
    fp = fopen(nome, "r");
    fscanf(fp,"%d ", nl);
    fscanf(fp,"%d ", nc);

    // aloca dinamicamente a matriz para armazenar as informações
    m = alocaMatriz(*nl, *nc);

    // lê as informações do arquivo txt
    for(i=0; i<*nl; i++)
        for(j=0; j<*nc; j++)
            fscanf(fp,"%d ", &m[i][j]);

    fclose(fp); //fecha o arquivo
    return m;
}

/*
 * Funcao que imprime o conteudo da matriz cuja referencia é recebida por parametro
 * Parâmetros: int **m: referencia para a matriz
 *             int nl, int nc: quantidade de linhas e colunas que serão acessadas
 *
 * Valor de retorno: ---
*/
void imprime_matriz(int **m, int nl, int nc)
{
    int i, j;
    for(i=0; i<nl; i++){
        for(j=0; j<nc; j++)
            if(m[i][j]==1)
                printf("*");
            else
                printf(" ");

        printf("\n");
    }
}

/*
 * Funcao que copia o conteudo da matriz fonte para a matriz destino
 * Parâmetros: int **fonte: matriz de origem
 *             int **dest: matriz de destino
 *             int nl, int nc: quantidade de linhas e colunas que serão acessadas
 *
 * Valor de retorno: ---
*/
void copia_matriz(int **fonte, int **dest, int nl, int nc)
{
    int i, j;

    for(i=0; i<nl; i++)
        for(j=0; j<nc; j++)
            dest[i][j] = fonte[i][j];
}


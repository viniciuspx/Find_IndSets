#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

/******************************************************************************************************************/

int SIZE_SETS[64] = {0};

int SIZE_ALL[64] = {0};
int SIZE_ALPHA[64] = {0};
int SIZE_I[64] = {0};
int SIZE_M[64] = {0};

int buffer = 0;

//Função para zerar um vetor

void zero_SIZE(int *v){

    int i;

    for (i = 0; i < 64; ++i)
    {
        v[i] = 0;
    }

}


// Função para imprimir um vetor

void print_SIZE(int *v, char set){

    int i;

    printf("\n\n");

    switch(set){
        case 's':
            printf("ALL SETS SIZES: \n");
            break;
        case 'a':
            printf("ALL ALPHA(G) SIZES: \n");
            break;
        case 'i':
            printf("ALL I(G) SIZES: \n");
            break;
        case 'm':
            printf("ALL M_i SIZES: \n");
            break;
    }

    for (i = 0; i < 64; i++)
    {
        if(v[i] != 0 ) printf("| Size: %d | Times: %d|\n", i,v[i]);
    }
}

//  Função utilizada para imprimir uma combinação
//  Passando uma mascara como argumento

void print_mask(long long mask){
    int i;
    for(i = 0; i < 64 ; i++){
        if(mask & (1LL << i)) printf("%d ", i);
    }
    printf("\n");
}

//  Função que imprime a matriz de adjacencia
//  Para questões de validação

void print_mat(int **matriz, int vertices){
	int i, j;
	for (i = 0; i < vertices; i++)
	{
		for (j = 0; j < vertices; j++)
		{
			printf("%d ", matriz[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

//  Função que gera uma Matriz nova e preenche a mesma com 0
//  Retorna um ponteiro duplo para a matriz

int **generate_m(int vertices){

    int i, j;

    int **m = (int**)malloc(vertices*sizeof(int*));

    for(i=0; i<vertices; i++)
    {
        m[i] = (int*)malloc(vertices*sizeof(int));
        for(j=0; j<vertices; j++)
        {
            m[i][j] = 0;
        }
    }

    return m;
}

int **mult_k2(int **m, int vertices){

    int **p, i, j;

    p = generate_m(vertices*2);

    for (i = 0; i < vertices; ++i)
    {
        for (j = 0; j < vertices; ++j)
        {
            if(m[i][j] == 1){
                p[i][j] = 1;
                p[i+vertices][j+vertices] = 1;
            }
        }
    }

    for (i = 0; i < vertices; ++i)
    {
        p[i][i+vertices] = p[vertices+i][i] = 1;
    }

    return p;
}

int **random_graph(int vertices){

    int **m, r_vert, i, j;

    srand(time(NULL));

    m = generate_m(vertices);

    for (i = 0; i < vertices; ++i)
    {
        for (j = 0; j < vertices; ++j)
        {
            if(i != j){
                m[i][j] = rand()%2;
            }
        }
    }

    return m;
}

//  Função que recebe como argumentos o ponteiro da matriz, a mascara de bits (conjunto / combinação)
//  e o numero de vertices, retorna 1 se o conjunto for independente e 0 se não

int test_ind(long long mask, int **matriz, int vertices){

    int i, j;

    for(i = 0 ;i < 64 ; i++){
        if(mask & (1LL << i)){
           	if (i > vertices) break;
           	for(j = 0; j < 64; j++)
            {
                if((mask & (1LL << j)))
                    if(matriz[i-1][j-1] == 1)
                        return 0;

            }
    	}
	}

    return 1;
}

//  Função que recebe como argumentos o ponteiro da matriz, a mascara de bits (conjunto / combinação)
//  e o numero de vertices, e retorna 1 se o conjunto for dominante ou 0 se não

int test_dom(long long mask, int **matriz, int vertices){

    int i, j, count;

    for(i = 0 ; i < vertices ; i++) matriz[i][i] = 1;

    for(i = 0 ; i < vertices ; i++)
    {
        count = 0;

        for(j = 0; j< 64 ; j++)
        {
            if(mask & (1LL << j)){

                if(matriz[i][j-1] == 1){
                    count = 1;
                       break;
                }

            }
        }

        if (count == 0) return 0;
    }

    for(i = 0 ; i < vertices ; i++) matriz[i][i] = 0;

    return 1;
}

//  Função que passados o ponteiro de uma matriz e a quantidade de vertices (int tam),
//  gera combinações (mascaras), e passa elas pelas funções de checar a dominacia e independencia
//  as combinações são geradas sem o 0


int test_max(int **matriz, int tam){

    int count_m = 0;

    long long  mask, j, i, v, size, a, w;

    long long limit = (1LL << (tam + 1));

    for(mask = 0; mask <= limit; mask++)
    {
        int size = __builtin_popcountll(mask);
        int flag = 0;

        for (i = 0; i < 64; ++i)
        {
        	if( mask & (1LL << i) && !i)
        	{
        		flag = 1;
        		break;
        	}
        }

        if(flag)
        	continue;

        if(size <= tam)
        {
           if(test_dom(mask,matriz,tam)){
             if(test_ind(mask,matriz,tam)){

                for (v = 0; v < 64; v++)
                {
                    if(mask & (1LL << i)) size++;
                }

                SIZE_SETS[size]++;
				        SIZE_ALL[size]++;

                print_mask(mask);
             }
           }
        }
    }

    //print_SIZE(SIZE_SETS,'s');

    for (i = 0; i < 64; ++i)
    {
        if(SIZE_SETS[i] != 0) count_m++;
    }

    SIZE_M[count_m]++;

    for (a = 63; a >= 0 ; a--)
    {
        if(SIZE_SETS[a] != 0){
            SIZE_ALPHA[a]++;
            break;
        }
    }



    for ( w = 0; w < 64; w++)
    {
        if (SIZE_SETS[w] != 0)
        {
            SIZE_I[w]++;
            if(w == 5){
            //  print_mat(matriz,tam);
            }
            break;
        }
    }



    zero_SIZE(SIZE_SETS);

    return 0;
}

// Função dada o ponteiro duplo da matriz, o numero de vertices e o numero de arestas
// lê as informações da matriz

void read_m(int **m, int vertices, int edges, int method){

    int v1, v2, i, j;

    char trash;

    if(method == 1 || method == 0){

    printf("Insert the sets (Edges): \n\n");

        for(i = 0; i < edges; i++)
        {
            printf("%d => (v1,v2) = ",i+1);
            scanf("%d%c%d",&v1,&trash,&v2);

            //printf("v1 == > %d v2 == > %d trash == > %c\n",v1,v2,trash);

            if(v1 > vertices || v2 > vertices)
            {
                printf("Vertex is not in the set of vertices.\n");
                printf("Not accepted values at last entry: v1 = %d v2 = %d\n",v1,v2);
                printf("Re-insert ... \n\n");
                i--;
            }

            else m[v1-1][v2-1] = m[v2-1][v1-1] = 1;
        }
    }

    if (method == 3){

        printf("Insert de Matrix.\n");

        for (i = 0; i < vertices; ++i)
        {
            for (j = 0; j < vertices; ++j)
            {
                scanf("%d",&m[i][j]);
            }
        }
    }

}

/******************************************************************************************************************/

void run(int choice){

    int **m, **p, vertices, edges;

    if(choice == 0 || choice == 1 || choice == 3){

	   printf("Number of vertices: \n");
	   scanf("%d",&vertices);

	   printf("Number of edges: \n");
	   scanf("%d",&edges);

	   m = generate_m(vertices);

	   read_m(m,vertices,edges,choice);

    }

    if(choice == 0 || choice == 3) {

        printf("\n\n\n");

        //print_mat(m,vertices); //Imprime a Matriz de Adjacencia do Grafo

        test_max(m,vertices);
    }

    if(choice == 1) {

        p = mult_k2(m,vertices);

        printf("\n\n\n");

        print_mat(p,vertices*2); //Imprime a Matriz de Adjacencia do Grafo

        test_max(p,vertices*2);
    }

    if (choice == 2){

        printf("Number of vertices.\n");
        scanf("%d",&vertices);
        m = random_graph(vertices);

        print_mat(m,vertices);

        test_max(m,vertices);
    }
}

void file_read(int n){

    int **m, i, j;

    m = generate_m(n);

    for (i = 0; i < n; ++i)
        {
            for (j = 0; j < n; ++j)
            {
                scanf("%d",&m[i][j]);
            }
        }

     buffer++;

     printf("%d graph\n", buffer);

     test_max(m,n);

}

void file_read_K2(int n){

     int **m, i, j, **p;

     m = generate_m(n);

     for (i = 0; i < n; ++i)
         {
             for (j = 0; j < n; ++j)
             {
                 scanf("%d",&m[i][j]);
             }
         }

      buffer++;

 	    p = mult_k2(m,n);
	    free(m);
      //print_mat(p,n*2);
      test_max(p,n*2);
      printf("%d graph\n", buffer);

 }

void menu(){

    printf("\n");
    printf("Enter an option to find all maximal independent sets of that Graph.\n");
    printf("Choose Wisely.\n\n");

    printf("1 - Read Graph.\n");
    printf("2 - Read Graph and multiply by K2. (Cartesian Product)\n");
    printf("3 - Read Graph using Adjancy Matrix.\n");
    printf("4 - Generate a random Graph.\n");
    printf("5 - Print Alpha and I of all graphs tested.\n");
    printf("0 - Exit\n");
    printf("\n\n");
}


/******************************************************************************************************************/


void main(int argc, int *argv[]){

    system("clear");

    int choice = -1;

    while(choice != 0){

        menu();

    	scanf("%d", &choice);

    	switch(choice){
            case 1:
                run(0);
            break;
            case 2:
                run(1);
            break;
            case 3:
                run(3);
            break;
            case 4:
                run(2);
            break;
            case 5:
                print_SIZE(SIZE_ALPHA,'a');
                print_SIZE(SIZE_I,'i');
                print_SIZE(SIZE_M,'m');
                print_SIZE(SIZE_ALL,'s');
            break;
            case 6:
                file_read(5);
            break;
            case 7:
                file_read_K2(10);
            break;
            case 0:
                system("clear");
                exit(0);
            break;
    	   }
        }

}

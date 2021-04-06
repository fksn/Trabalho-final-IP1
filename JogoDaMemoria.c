#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
struct mat
{
    int nl, nc;
    int **m;
} typedef Mat;
int dif;
Mat * aloca_matriz(int nl, int nc);
int bot_chute(Mat * A, Mat * B, Mat * bot_mem1, Mat * bot_mem2, Mat * Ac, Mat * Bc, int Dif, int nl, int nc);
void gera_matriz(Mat * M, int nl, int nc);
void libera_matriz(Mat * M);
void posicao_frente(Mat * M, int rand1, int rand2, int nl, int nc, int i);
void posicao_atras(Mat * M, int rand1, int rand2, int nl, int nc, int i);
void print_mat(Mat * A, Mat * Ac, int nl, int nc);
void print_mat_parcial(Mat * A, Mat * Ac, int nl, int nc, int a1, int a2);
void bot_memoria_chute(Mat * bot_mem, int l, int c, int dif);
void bot_reduz(Mat * bot_mem, int nl, int nc);
void quem_ganhou(int pontuacao_vc, int pontuacao_bot);
void bot_posicao_frente(Mat * Ac, int * vet, int rand1, int rand2, int nl, int nc);
int salvar_arq(Mat * A, Mat * B, Mat * Ac, Mat * Bc, Mat * Bot_Mem1, Mat * Bot_Mem2, char save[32], int pontuacao_vc, int pontuacao_bot);
void abre_arq(Mat * A, Mat * B, Mat * Ac, Mat * Bc, Mat * Bot_Mem1, Mat * Bot_Mem2, char save[32], FILE * arq, int nl, int nc);
int main()
{
    srand (time(NULL));
    Mat * A = NULL; //Matriz da primeira parte do quebra cabeça
    Mat * B = NULL; //Matriz da segunda parte do quebra cabeça
    Mat * Ac = NULL; //Matriz que guarda se um elemento do par já foi achado
    Mat * Bc = NULL; // Matriz que guarda se um elemento do segundo par já foi achado
    Mat * Bot_Mem1 = NULL;  //Matriz que guarda a memoria do bot em relação a primeira matriz
    Mat * Bot_Mem2 = NULL;  //Matriz que guarda a memoria do bot em relação a seguda metriz
    FILE * arq = NULL; //Arquivo para abrir/salvar os dados
    int inicio, l, c, nl, nc, a1 = 1, a2, b1, b2, cont = 0, printl, printc, traco, pontuacao[2], op = 0, teste, salvar, ns;
    char save[32], str[4];
    printf("Deseja iniciar novo jogo ou carregar um jogo?\n0 - Novo jogo\n1 - Carregar ultimo jogo\n");
    scanf("%d", &inicio);
    while (inicio != 0 && inicio != 1)
    {
        printf("Deseja iniciar novo jogo ou carregar um jogo?\n0 - Novo jogo\n1 - Carregar ultimo jogo\n");
        scanf("%d", &inicio);
    }
    if (inicio == 0)
    {
        cont = 0; //Contagem de pares já achados
        pontuacao[0] = 0; //Sua pontuação
        pontuacao[1] = 0; //Pontuação do bot
        printf("Qual a dificuldade?\n0 - Fácil\n1 - Médio\n2 - Difícil\n");
        scanf("%d", &dif);
        while (dif < 0 || dif > 2)
        {
            printf("Qual a dificuldade?\n0 - Fácil\n1 - Médio\n2 - Difícil\n");
            printf("Digite um número entre 0 e 2\n");
            scanf("%d", &dif);
        }
        printf("Quantas linhas e quantas colunas?\n");
        scanf("%d %d", &l, &c);
        while ( l <= 2 || c <= 2)
        {
            printf("Digite um número maior que 2\n");
            scanf("%d %d", &l, &c);
        }
        //Alocação e geração aleatoria da matriz A e B
        A = aloca_matriz(l, c);
        gera_matriz(A, l, c);
        B = aloca_matriz(l, c);
        gera_matriz(B, l, c);
        //Alocação das matrizes de memoria do bot e a de verificar se o par já foi achado
        Ac = aloca_matriz(l, c);
        Bc = aloca_matriz(l, c);
        Bot_Mem1 = aloca_matriz(l, c);
        Bot_Mem2 = aloca_matriz(l, c);
    }
    else
    {
        printf("Digite o nome do arquivo a ser aberto:\n");
        scanf("%s", save);
        arq = fopen(save, "rb"); //Abre o arquivo escolhido
        if (arq == NULL)
        {
            printf("Erro ao abrir o arquivo\n");
            libera_matriz(Bot_Mem1);
            libera_matriz(Bot_Mem2);
            libera_matriz(A);
            libera_matriz(B);
            libera_matriz(Ac);
            libera_matriz(Bc);
            return 0;
        }
        printf("%s-\n", str);
        fread(str, sizeof(char), 3, arq); 
        printf("%s-\n", str);
        if (strcmp(str, "MAT") != 0) //Verifica se "MAT" é p inicio do arquivo, verificando se o arquivo é correto
        {
            printf("Arquivo invalido\n");
            fclose(arq);
            libera_matriz(Bot_Mem1);
            libera_matriz(Bot_Mem2);
            libera_matriz(A);
            libera_matriz(B);
            libera_matriz(Ac);
            libera_matriz(Bc);
            return 0;
        }
        fread(&dif, sizeof(int), 1, arq); //Lê a dificuldade escolhida
        fread(&pontuacao[0], sizeof(int), 1, arq); //Lê a pontuação que você tinha feito
        fread(&(pontuacao[1]), sizeof(int), 1, arq); //Lê a pontuação que o bot tinha feito
        fread(&l, sizeof(int), 1, arq); //Lê a quantidade de linhas 
        fread(&c, sizeof(int), 1, arq); //Lê a quantidade de colunas
        //Aloca todas as matrizes
        A = aloca_matriz(l, c);
        B = aloca_matriz(l, c);
        Ac = aloca_matriz(l, c);
        Bc = aloca_matriz(l, c);
        Bot_Mem1 = aloca_matriz(l, c);
        Bot_Mem2 = aloca_matriz(l, c);
        abre_arq(A, B, Ac, Bc, Bot_Mem1, Bot_Mem2, save, arq, l, c); //Preenche as matrizes de acordo com o arquivo
        cont = pontuacao[0] + pontuacao[1];
    }
    while (cont < l*c && op == 0)
    {
        system("tput reset"); //Limpa o terminal
        print_mat(A, Ac, l, c);
        for ( traco = 0; traco < c*3+2; traco++)
        {
            printf("-");
        }
        printf("\n\n");
        print_mat(B, Bc, l, c);
        printf("Sua pontuação: %d Pontuação do bot: %d\n\n", pontuacao[0], pontuacao[1]);
        printf("Digite a linha e a coluna da primeira matriz:\n");
        scanf("%d %d", &a1, &a2); 
        while (a1 <= 0 || a1 > l || a2 <= 0 || a2 > l || Ac->m[a1-1][a2-1] == 1)
        {
            printf("Digite a linha e a coluna da primeira matriz: \nLinha deve ser entre 1 e %d\nColuna deve ser entre 1 e %d\n", l, c); //Palpite sobre primeira matriz
            scanf("%d %d", &a1, &a2);
        }
        bot_memoria_chute(Bot_Mem1, a1, a2, dif);
        print_mat_parcial(A, Ac, l, c, a1-1, a2-1);
        printf("Digite a linha e a coluna da segunda matriz:\n");
        scanf("%d %d", &b1, &b2); 
        while ( b1 <= 0 || b1 > l ||b2 <= 0 || b2 > c || Bc->m[b1-1][b2-1] == 1)
        {
            printf("Digite a linha e a coluna da primeira matriz: \nLinha deve ser entre 1 e %d\nColuna deve ser entre 1 e %d\n", l, c); //Palpite sobre segnda matriz
            scanf("%d %d", &b1, &b2);
        }
        bot_memoria_chute(Bot_Mem2, b1, b2, dif); //Armazena o chute do usuario na memoria do bot
        print_mat_parcial(B, Bc, l, c, b1-1, b2-1);
        if (A->m[a1-1][a2-1] == B->m[b1-1][b2-1])
        {
            cont++;
            pontuacao[0] = pontuacao[0] + 1;
            Ac->m[a1-1][a2-1] = 1;
            Bc->m[b1-1][b2-1] = 1;
            if (cont ==  l*c)
            {
                break;
            }
        }
        teste = bot_chute(A, B, Bot_Mem1, Bot_Mem2, Ac, Bc, dif, l, c); //Palpite do bot
        if(teste == 1)
        {
            printf("\nO Bot acertou\n");
            cont++;
            pontuacao[1] = pontuacao[1] + 1;
        }
        if (dif == 1)
        {
            bot_reduz(Bot_Mem1, l, c);
            bot_reduz(Bot_Mem2, l, c);
        }
        printf("\n");
        printf("Deseja sair? \n0 - Não\n1 - Sim\n");
        scanf("%d", &op);
    }
    if (op == 0)
    {
        quem_ganhou(pontuacao[0], pontuacao[1]);
    }
    else
    {
        printf("Deseja salvar?\n0 - Não\n1 - Sim\n");
        scanf("%d", &salvar);
        if (salvar == 1)
        {
            printf("Qual o nome do arquivo que deseja salvar?\n");
            scanf("%s%*c", save);
            ns = salvar_arq(A, B, Ac, Bc, Bot_Mem1, Bot_Mem2, save, pontuacao[0], pontuacao[1]);
            while (ns == 1)
            {
                printf("Esso ao salvar o arquivo, digite outro nome:\n");
                scanf("%s%*c", save);
                ns = salvar_arq(A, B, Ac, Bc, Bot_Mem1, Bot_Mem2, save, pontuacao[0], pontuacao[1]);
            }
        }
    }
    //tira a alocação das matrizes alocadas anteriormente
    libera_matriz(Bot_Mem1);
    libera_matriz(Bot_Mem2);
    libera_matriz(A);
    libera_matriz(B);
    libera_matriz(Ac);
    libera_matriz(Bc);
    return 0;
}
// Função para alocar matrizez.
Mat * aloca_matriz(int nl, int nc) 
{
    int i;
    Mat * M = NULL;
    M = (Mat *) malloc(sizeof(Mat));
    M->nl = nl;
    M->nc = nc;

    M->m = (int **) malloc(nl * sizeof(int *));
    for ( i = 0; i < nl; i++)
    {
        M->m[i] = (int*) calloc( nc, sizeof(int));
    }
    return M;
}
// Função para liberar matrizes
void libera_matriz(Mat * M)
{
    int i;
    for ( i = 0; i < M->nl ;i++)
    {
        free(M->m[i]);
    }
    free(M->m);
    free(M);
}
// Função que coloca valores aleatorios na matriz
void gera_matriz(Mat * M, int nl, int nc)
{
    int i, rand1, rand2, aux = 0;
    for ( i = 1; i <= nl*nc; i++)
    {
        
        rand1 = rand() % nl;
        rand2 = rand() % nc;
        // printf("rand1 %d rand2 %d num %d\n", rand1, rand2, i);
        if (M->m[rand1][rand2] != 0)
        {
            if (aux == 0)
            {
                posicao_frente(M, rand1, rand2, nl, nc, i);
                aux = 1;
            }
            else
            {
                posicao_atras(M, rand1, rand2, nl, nc, i);
                aux = 0;
            }
        }
        else
        {
            M->m[rand1][rand2] = i;
        }
    }
    
}
// Função que vai pra proxima posição verificar se esta vazia
void posicao_frente(Mat * M, int rand1, int rand2, int nl, int nc, int num)
{
    if (M->m[rand1][rand2] == 0)
    {
        M->m[rand1][rand2] = num;
        return;
    } 
    else
    {
        if ( rand2 == nc-1 )
        {
            if (rand1 == nl-1)
            {
                posicao_frente(M, 0, 0, nl, nc, num);
            }
            else
            {
                posicao_frente(M, rand1+1, 0, nl, nc, num);
            }
        }
        else
        {
            posicao_frente(M, rand1, rand2+1, nl, nc, num);
        }
    }
    return;
}
// Função que vai pra proxima posição verificar se esta vazia
void posicao_atras(Mat * M, int rand1, int rand2, int nl, int nc, int num)
{
    if (M->m[rand1][rand2] == 0)
    {
        M->m[rand1][rand2] = num;
        return;
    } 
    else
    {
       if ( rand2 == 0)
        {
            if (rand1 == 0)
            {
                posicao_atras(M, nl-1, nc-1, nl, nc, num);
            }
            else
            {
                posicao_atras(M, rand1-1, nc-1, nl, nc, num);
            }
        }
        else
        {
            posicao_atras(M, rand1, rand2-1, nl, nc, num);
        } 
    }
    return;
}
// Imprime matriz com posições já acertadas anteriormente
void print_mat(Mat * A, Mat * Ac, int nl, int nc)
{
    int printl, printc;
    for (printc = 0; printc <= nc; printc++)
    {
        if (printc != 0)
        {
            printf("%2d ", printc);
        }
        else
        {
            printf("   ");
        }
    }
    printf("\n");
    for (printl = 0; printl < nl; printl++)
    {
        for (printc = -1; printc < nc; printc++)
        {
            if (printc!= -1)
            {
                if (Ac->m[printl][printc] == 0)
                {
                    printf(" # ");
                }
                else
                {
                    printf("%2d ", A->m[printl][printc]);
                }
            }
            else
            {
                printf("%2d ", printl+1);
            }
        }
        printf("\n");
    }
    printf("\n");
}
// Imprime matriz com posições já acertadas anteriormente e o chute atual
void print_mat_parcial(Mat * A, Mat * Ac, int nl, int nc, int a1, int a2)
{
    int printl, printc;
    for (printc = 0; printc <= nc; printc++)
    {
        if (printc != 0)
        {
            printf("%2d ", printc);
        }
        else
        {
            printf("   ");
        }
    }
    printf("\n");

    for (printl = 0; printl < nl; printl++)
    {
        for (printc = -1; printc < nc; printc++)
        {
            if (printc!= -1)
            {
                 if (printl == a1 && printc == a2)
                {
                    printf("%2d ", A->m[printl][printc]);
                }
                else
                {
                    if (Ac->m[printl][printc] == 0)
                    {
                        printf(" # ");
                    }
                    else
                    {
                        printf("%2d ", A->m[printl][printc]);
                    }
                }
            }
            else
            {
                printf("%2d ", printl+1);
            }
        }
        printf("\n");
    }
    printf("\n");

    // for (printl = 0; printl < nl; printl++)
    // {
    //     for (printc = 0; printc < nc; printc++)
    //     {
    //         if (printl == a1 && printc == a2)
    //         {
    //             printf("%2d ", A->m[printl][printc]);
    //         }
    //         else
    //         {
    //             if (Ac->m[printl][printc] == 0)
    //             {
    //                 printf(" # ");
    //             }
    //             else
    //             {
    //                 printf("%2d ", A->m[printl][printc]);
    //             }
    //         }
    //     }
    //     printf("\n");
    // }
    // printf("\n");
}
// Matriz que guarda os campos que o bot se lembra
void bot_memoria_chute(Mat * bot_mem, int l, int c, int dif)
{
    if (dif == 0)
    {
        return;
    }
    else
    {
        if (dif == 1)
        {
            bot_mem->m[l-1][c-1] = 5;
            return;
        }
        else
        {
            bot_mem->m[l-1][c-1] = 1;
            return;
        }
    }
}
// Função que faz o chute do bot
int bot_chute(Mat * A, Mat * B, Mat * bot_mem1, Mat * bot_mem2, Mat * Ac, Mat * Bc, int Dif, int nl, int nc)
{
    // srand (time(NULL));
    int rand1 = 0, rand2 = 0, rand3 = 0, rand4 = 0, l, c, l2, c2, vet[2] = {0,0};
    if (Dif == 0)
    {
        rand1 = rand() % nl;
        rand2 = rand() % nc;
        rand3 = rand() % nl;
        rand4 = rand() % nc;
        printf("Tentativa do Bot, linha: %d Coluna: %d\n", rand1+1, rand2+1);
        print_mat_parcial(A, Ac, nl, nc, rand1, rand2);
        printf("Tentativa do Bot, linha: %d Coluna: %d\n", rand3+1, rand4+1);
        print_mat_parcial(B, Bc, nl, nc, rand3, rand4);
        if (A->m[rand1][rand2] == B->m[rand3][rand4] && Ac->m[rand1][rand2] == 0 && Bc->m[rand3][rand4] == 0)
        {
            Ac->m[rand1][rand2] = 1;
            Bc->m[rand3][rand4] = 1;
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        if (Dif == 1)
        {
            for ( l = 0; l < nl; l++)
            {
                for (c = 0; c < nc; c++)
                {
                    if (bot_mem1->m[l][c] != 0)
                    {
                        for ( l2 = 0; l2 < nl; l2++)
                        {
                            for (c2 = 0; c2 < nc; c2++)
                            {
                                if (bot_mem2->m[l2][c2] != 0)
                                {
                                    if ((A->m[l][c] == B->m[l2][c2]) && (Ac->m[l][c] == 0) && (Bc->m[l2][c2] == 0))
                                    {
                                        Ac->m[l][c] = 1;
                                        Bc->m[l2][c2] = 1;
                                        printf("Tentativa do Bot, linha: %d Coluna: %d\n", l+1, c+1);
                                        print_mat_parcial(A, Ac, nl, nc, l, c);
                                        printf("Tentativa do Bot, linha: %d Coluna: %d\n", l2+1, c2+1);
                                        print_mat_parcial(B, Bc, nl, nc, l2, c2);
                                        return 1;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            rand1 = rand() % nl;
            rand2 = rand() % nc;
            rand3 = rand() % nl;
            rand4 = rand() % nc;
            if (Ac->m[rand1][rand2] == 1)
            {
                bot_posicao_frente(Ac, vet, rand1, rand2, nl, nc);
                rand1 = vet[0];
                rand2 = vet[1];
            }
            if (Ac->m[rand3][rand4] == 1)
            {
                bot_posicao_frente(Ac, vet, rand3, rand4, nl, nc);
                rand3 = vet[0];
                rand4 = vet[1];
            }
            bot_mem1->m[rand1][rand2] = 5;
            bot_mem2->m[rand3][rand4] = 5;
            printf("Tentativa do Bot, linha: %d Coluna: %d\n", rand1+1, rand2+1);
            print_mat_parcial(A, Ac, nl, nc, rand1, rand2);
            printf("Tentativa do Bot, linha: %d Coluna: %d\n", rand3+1, rand4+1);
            print_mat_parcial(B, Bc, nl, nc, rand3, rand4);
            if (A->m[rand1][rand2] == B->m[rand3][rand4] && Ac->m[rand1][rand2] == 0 && Bc->m[rand3][rand4] == 0)
            {
                Ac->m[rand1][rand2] = 1;
                Bc->m[rand3][rand4] = 1;
                return 1;
            }
            else
            {
                return 0;
            }
        }
        else
        {
            for ( l = 0; l < nl; l++)
            {
                for (c = 0; c < nc; c++)
                {
                    if (bot_mem1->m[l][c] != 0)
                    {
                        for ( l2 = 0; l2 < nl; l2++)
                        {
                            for (c2 = 0; c2 < nc; c2++)
                            {
                                if (bot_mem2->m[l2][c2] != 0)
                                {
                                    if ((A->m[l][c] == B->m[l2][c2]) && (Ac->m[l][c] == 0) && (Bc->m[l2][c2] == 0))
                                    {
                                        Ac->m[l][c] = 1;
                                        Bc->m[l2][c2] = 1;
                                        printf("Tentativa do Bot, linha: %d Coluna: %d\n", l+1, c+1);
                                        print_mat_parcial(A, Ac, nl, nc, l, c);
                                        printf("Tentativa do Bot, linha: %d Coluna: %d\n", l2+1, c2+1);
                                        print_mat_parcial(B, Bc, nl, nc, l2, c2);
                                        return 1;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            rand1 = rand() % nl;
            rand2 = rand() % nc;
            rand3 = rand() % nl;
            rand4 = rand() % nc;
            if (Ac->m[rand1][rand2] == 1)
            {
                bot_posicao_frente(Ac, vet, rand1, rand2, nl, nc);
                rand1 = vet[0];
                rand2 = vet[1];
            }
            
            if (Ac->m[rand3][rand4] == 1)
            {
                bot_posicao_frente(Ac, vet, rand3, rand4, nl, nc);
                rand3 = vet[0];
                rand4 = vet[1];
            }
            bot_mem1->m[rand1][rand2] = 5;
            bot_mem2->m[rand3][rand4] = 5;
            printf("Tentativa do Bot, linha: %d Coluna: %d\n", rand1+1, rand2+1);
            print_mat_parcial(A, Ac, nl, nc, rand1, rand2);
            printf("Tentativa do Bot, linha: %d Coluna: %d\n", rand3+1, rand4+1);
            print_mat_parcial(B, Bc, nl, nc, rand3, rand4);
            if (A->m[rand1][rand2] == B->m[rand3][rand4] && Ac->m[rand1][rand2] == 0 && Bc->m[rand3][rand4] == 0)
            {
                Ac->m[rand1][rand2] = 1;
                Bc->m[rand3][rand4] = 1;
                return 1;
            }
            else
            {
                return 0;
            }
        }
    }
}
// Reduz a memoria do bot em uma unidade
void bot_reduz(Mat * bot_mem, int nl, int nc)
{
    int l, c;
    for ( l = 0; l < nl; l++)
    {
        for ( c = 0; c < nc; c++)
        {
            if (bot_mem->m[l][c]>0)
            {
                bot_mem->m[l][c] = bot_mem->m[l][c] - 1;
            }
        }
    }
}
// Imprime o ganhador
void quem_ganhou(int pontuacao_vc, int pontuacao_bot)
{
    if (pontuacao_vc > pontuacao_bot)
    {
        printf("Voce ganhou :(\n");
    }
    else
    {
        if (pontuacao_vc < pontuacao_bot)
        {
            printf("Voce perdeu :)\n");
        }
        else
        {
            printf("Deu empate :|\n");
        }
    }
}
// Com o chute dando em uma posição já utilizada, a função pega a proxima posição vazia
void bot_posicao_frente(Mat * Ac, int * vet, int rand1, int rand2, int nl, int nc)
{
    if (Ac->m[rand1][rand2] == 0)
    {
        vet[0] = rand1;
        vet[1] = rand2;
        return;
    } 
    else
    {
        if ( rand2 == nc-1 )
        {
            if (rand1 == nl-1)
            {
                bot_posicao_frente(Ac, vet, 0, 0, nl, nc);
            }
            else
            {
                bot_posicao_frente(Ac, vet, rand1+1, 0, nl, nc);
            }
        }
        else
        {
            bot_posicao_frente(Ac, vet, rand1, rand2+1, nl, nc);
        }
    }
    return;
}
// Função qu salva os dados no arquivo
int salvar_arq(Mat * A, Mat * B, Mat * Ac, Mat * Bc, Mat * Bot_Mem1, Mat * Bot_Mem2, char save[32], int pontuacao_vc, int pontuacao_bot)
{
    int l, c;
    FILE * arq = NULL;
    char ch[3] = {"MAT"};
    arq = fopen (save, "wb");
    if (arq == NULL)
    {
        printf("Erro ao abrir o arquivo");
        return 0;
    }
    fwrite(&ch, sizeof(char), 3, arq);
    fwrite(&dif, sizeof(int), 1, arq);
    fwrite(&pontuacao_vc, sizeof(int), 1, arq);
    fwrite(&pontuacao_bot, sizeof(int), 1, arq);
    fwrite(&(A->nl), sizeof(int), 1, arq);
    fwrite(&(A->nc), sizeof(int), 1, arq);
    for ( l = 0; l < A->nl; l++)
    {
        for ( c = 0; c < A->nc; c++)
        {
            fwrite(&(A->m[l][c]), sizeof(int), 1, arq);
        }
    }

    for ( l = 0; l < A->nl; l++)
    {
        for ( c = 0; c < A->nc; c++)
        {
            fwrite(&(B->m[l][c]), sizeof(int), 1, arq);
        }
    }

    for ( l = 0; l < A->nl; l++)
    {
        for ( c = 0; c < A->nc; c++)
        {
            fwrite(&(Ac->m[l][c]), sizeof(int), 1, arq);
        }
    }

    for ( l = 0; l < A->nl; l++)
    {
        for ( c = 0; c < A->nc; c++)
        {
            fwrite(&(Bc->m[l][c]), sizeof(int), 1, arq);
        }
    }

    for ( l = 0; l < A->nl; l++)
    {
        for ( c = 0; c < A->nc; c++)
        {
            fwrite(&(Bot_Mem1->m[l][c]) , sizeof(int), 1, arq);
        }
    }

    for ( l = 0; l < A->nl; l++)
    {
        for ( c = 0; c < A->nc; c++)
        {
            fwrite(&(Bot_Mem2->m[l][c]), sizeof(int), 1, arq);
        }
    }
    fclose(arq);
    return 0;
}
// Função que traz dados dos arquivos para as matrizes
void abre_arq(Mat * A, Mat * B, Mat * Ac, Mat * Bc, Mat * Bot_Mem1, Mat * Bot_Mem2, char save[32], FILE * arq, int nl, int nc)
{
    int l, c;
    for ( l = 0; l < nl; l++)
    {
        for ( c = 0; c < nc; c++)
        {
            fread(&(A->m[l][c]), sizeof(int), 1, arq);
        }
        printf("\n");
    }
    for ( l = 0; l < nl; l++)
    {
        for ( c = 0; c < nc; c++)
        {
            fread(&(B->m[l][c]), sizeof(int), 1, arq);
        }
        printf("\n");
    }

    for ( l = 0; l < nl; l++)
    {
        for ( c = 0; c < nc; c++)
        {
            fread(&(Ac->m[l][c]), sizeof(int), 1, arq);
        }
        printf("\n");
    }

    for ( l = 0; l < nl; l++)
    {
        for ( c = 0; c < nc; c++)
        {
            fread(&(Bc->m[l][c]), sizeof(int), 1, arq);
        }
        printf("\n");
    }

    for ( l = 0; l < nl; l++)
    {
        for ( c = 0; c < nc; c++)
        {
            fread(&(Bot_Mem1->m[l][c]) , sizeof(int), 1, arq);
        }
        printf("\n");
    }

    for ( l = 0; l < nl; l++)
    {
        for ( c = 0; c < nc; c++)
        {
            fread(&(Bot_Mem2->m[l][c]), sizeof(int), 1, arq);
        }
        printf("\n");
    }
    fclose(arq);
}

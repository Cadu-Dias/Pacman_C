#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mapa.h"

//cria uma cópia da primeira versão do mapa
void copiamapa(MAPA* destino, MAPA* origem) {
    //as variáveis de linhas e colunas do mapa a ser copiado recebem a quantidade de linhas e colunas do mapa original
    destino->linhas = origem->linhas;
    destino->colunas = origem->colunas;
    //cria a cópia
    alocamapa(destino);

    for (int i = 0; i < origem->linhas; i++) {
        //copia cada posição do mapa original para o mapa de cópia
        strcpy(destino->matriz[i], origem->matriz[i]);
    }
    
}

int ehparede(MAPA* m, int x, int y) {
    //Retorna 0 se nenhuma das posições seguintes for uma parede 
    //Retorna 1 se alguma das posições seguintes for algumas das paredes
    return m->matriz[x][y] == PARADE_VERTICAL || m->matriz[x][y] == PAREDE_HORIZONTAL;
}

//verifica se o elemento numa posição específica é um personagem ou uma parede
int ehpersonagem(MAPA* m, char personagem, int x, int y) {
    //retorna 0 se não for um personagem e 1 se for
    return m->matriz[x][y] == personagem;
}
//verificar se o personagem pode andar, a partir da chamada de outras funções como meio de verificação
int podeandar(MAPA* m, char personagem, int x, int y) {
    return ehvalida(m, x, y) && !ehparede(m, x, y) && !ehpersonagem(m, personagem, x, y);
}

//verifica se a posição existe no mapa
int ehvalida(MAPA* m, int x, int y) {
    if(x >= m->linhas) {
        return 0;
    }
    else if(y >= m->colunas) {
        return 0;
    }
    return 1;
}
//verifica se a posição a ser colocado o personagem é um local o qual é permitido
int ehvazia(MAPA* m, int x, int y) {
    return m->matriz[x][y] == VAZIO;  
}
//muda os elementos presentes em cada posição da matriz
void andandonomapa(MAPA* m, int xorigem, int yorigem, int xdestino, int ydestino) {
    //personagem recebe a posição atual do usuário
    char personagem = m->matriz[xorigem][yorigem];
    //onde era um '.' virá o local onde o personagem aparecerá
    m->matriz[xdestino][ydestino] = personagem;
    //onde o personagem estava vira um '.'
    m->matriz[xorigem][yorigem] = VAZIO;
}

//'c' é o parâmetro que recebe o @
int encontramapa(MAPA* m, POSICAO* p, char c) {
    //acha a posição do pacman
    for(int i = 0; i < m->linhas; i++) {
        for(int j = 0; j < m->colunas; j++) {
            //se achar ele define as posições do pacman
            if(m->matriz[i][j] == c) {
                p->x = i;
                p->y = j;
                return 1;
            }
        }
    }
    return 0;
}
//cria o mapa
void alocamapa(MAPA* m) {
    //cada linha da matriz recebe 5 bytes 
    m->matriz = malloc(sizeof(char*) * m->linhas);
    
    for(int i = 0; i < m->linhas; i++) {
        //cada coluna da matriz recebe ao todo 10 bytes
        m->matriz[i] = malloc(sizeof(char) * (m->colunas + 1));
    }
}
//faz a leitura do arquivo que contém o mapa
void lemapa(MAPA* m) {

    FILE* f;
    //f recebe o arquivo txt
    f = fopen("D:/Alura/Curso_C_1/pacman/mapa.txt", "r");
    //chamado quando ocorreu um erro na leitura
    if(f == 0) {
        printf("Erro na leitura do arquivo");
        exit(1);
    }

    fscanf(f, "%d %d", &(m->linhas), &(m->colunas));
    printf("linhas %d colunas %d\n", m->linhas, m->colunas);
    //cria o mapa
    alocamapa(m);
   
    for (int i =0; i < 5; i++) {
        //cada posição da matriz recebe um elemento presente no arquivo
        fscanf(f, "%s", m->matriz[i]);
    }
    //termina leitura do arquivo
    fclose(f);
}

//libera o espaço de memória utilizado por malloc
void liberamapa(MAPA* m) {
    
    for(int i = 0; i< m->linhas; i++) {
        free(m->matriz[i]);
    }
    
    free(m->matriz);
}




#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "pacman.h"
#include "mapa.h"
#include "ui.h"

MAPA m; //Sendo MAPA o tipo que foi criado para a struct mapa
POSICAO heroi; //Sendo posição as coordenadas do heroi no mapa
int tempilula = 0;


//define a direção que o fantasma irá
int direcaofantasma(int xatual, int yatual, int* xdestino, int* ydestino) {

    //define os possíveis movimentos do herói
    int opcoes[4][2] = {
        {xatual, yatual + 1},
        {xatual + 1, yatual},
        {xatual, yatual - 1},
        {xatual -1, yatual}
    };

    srand(time(0));
    for(int i = 0; i < 10; i++) {
        //posição receberá um valor de 0 a 3
        int posicao = rand() % 4;

        //verifica se o fantasma pode andar
        if(podeandar(&m, FANTASMA, opcoes[posicao][0], opcoes[posicao][1])) {
            //se sim decide-se uma posição aleatória para ele
            *xdestino = opcoes[posicao][0];
            *ydestino = opcoes[posicao][1];

            return 1;
        }
    }
    return 0;
}

//função de criação dos fantasmaas
void fantasmas() {

    MAPA copia;

    //cria-se uma cópia da primeira versão do mapa
    copiamapa(&copia, &m);

    //procura a posição do fantasma
    for (int i=0; i< m.linhas; i++) {
        for (int j =0; j < m.colunas; j++) {

            //se achar o fantasma
            if(copia.matriz[i][j] == FANTASMA) {

                //declara-se as coordenadas dele
                int xdestino;
                int ydestino;
                
                //define para onde o fantasma se moverá
                int encontrou = direcaofantasma(i, j, &xdestino, &ydestino);

                if(encontrou) {
                    //faz com que o fantasma se mova
                    andandonomapa(&m, i, j, xdestino, ydestino);
                }
            }
        }
    }
    liberamapa(&copia);
}

//avisa o usuário que o jogo foi finalizado
int acabou() {
	POSICAO pos;

	int perdeu = !encontramapa(&m, &pos, HEROI);
	int ganhou = !encontramapa(&m, &pos, FANTASMA);

	return ganhou || perdeu;
		
}

int ehdirecao(char direcao) {
    //devolve 1 se for verdadeiro algumas das condições e 0 se todas forem falsas
    return direcao == ESQUERDA || direcao == BAIXO || direcao == CIMA || direcao == DIREITA;
}
void move(char direcao) {
    int x;
    int y;

    //verifica se o a direção do usuário é alguma das direções permitidas pelo programa
    if(!ehdirecao(direcao)) {
        //se for retornado 0 na condição, será executado esse return
        return;
    }

    int proximo_x = heroi.x;
    int proximo_y = heroi.y;

    switch (direcao){
        //esquerda
        case ESQUERDA:
            //y - 1, pois estou indo para esquerda, por exemplo, estou na coluna 10 quando aperto
            //'a' vou para a coluna 9
            proximo_y--;
            break;
        
        //cima
        case CIMA:
            // x-1, pois a linha pois estou indo para cima em que a linha 0 é o topo. Exemplificando, o estou na linha 1
            //quando vou para cima estou indo a linha zero
            proximo_x--;
            break;

        //baixo
        case BAIXO:
            // x+1, pois a linha pois estou indo para baixo em que a linha 0 é o topo, logo eu quero me afastar mais ainda do topo,
            //por isso soma-se o 1
            proximo_x++;
            break;

        //direita
        case DIREITA:
            //y + 1, pois estou indo para direta, por exemplo, estou na coluna 9 quando aperto
            //'d' vou para a coluna 10
            proximo_y++;
            break;

    }

    if(!podeandar(&m, HEROI,proximo_x, proximo_y)) {
        return;
    }
    

    if(ehpersonagem(&m, PILULA, proximo_x, proximo_y)) {
        tempilula = 1;
    }

    //faz com que o herói no mapa
    andandonomapa(&m, heroi.x, heroi.y, proximo_x, proximo_y);
    heroi.x = proximo_x;
    heroi.y = proximo_y;
}

void explodepilula() {
    //chama a função explodepilula2, para incluir 4 direções para a bom, levando em consideração possíveis valores de 
    //x e y
    if(!tempilula) return;

    explodepilula2(heroi.x, heroi.y, 0, 1, 3);
    explodepilula2(heroi.x, heroi.y, 0, -1, 3);
    explodepilula2(heroi.x, heroi.y, 1, 1, 3);
    explodepilula2(heroi.x, heroi.y, -1, 1, 3);

    tempilula = 0;
}

void explodepilula2(int x, int y, int somax, int somay, int qtd) {
    
    if(qtd == 0) {
        return;
    }

    int novox = x + somax;
    int novoy = y + somay;

    if(!ehvalida(&m, novox, novoy)) {
        return;
    }

    if(ehparede(&m, novox ,novoy)) {
        return;
    }
    m.matriz[novox][novoy] = VAZIO;
    //pelo método de recursão se verifica a posição das próxima casa a deireita de fomra sucessitva
    explodepilula2(novox, novoy, somax, somay, qtd - 1);  
}

int main() {
    
    lemapa(&m);
    //acha a posição do pacman
    encontramapa(&m, &heroi, HEROI);
    do
    {
        printf("Tem pílula: %s\n", (tempilula ? "SIM" : "NAO"));
        imprimemapa(&m);

        char comando;
        scanf(" %c", &comando);
        if(ehdirecao(comando)) move(comando);

        if(comando == BOMBA) {
            explodepilula();
            
        }

        fantasmas();

    } while (!acabou());
    
    liberamapa(&m);
}

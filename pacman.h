#ifndef _PACMAN_

#define _PACMAN_

#define CIMA 'w'
#define BAIXO 's'
#define DIREITA 'd'
#define ESQUERDA 'a'
#define BOMBA 'b'


void move(char direcao);
int acabou();
int ehdirecao(char direcao);
void fantasmas();
int direcaofantasma(int xatual, int yatual, int* xdestino, int* ydestino);
void explodepilula2(int x, int y, int somax, int somay, int qtd);
void explodepilula();

#endif
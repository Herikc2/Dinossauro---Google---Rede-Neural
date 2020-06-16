#pragma once

// DEFINES

#define LARGURA_TELA 1200
#define ALTURA_TELA 700
#define FPS 60.0

#define POPULACAO_TAMANHO       2000

#define DINO_BRAIN_QTD_LAYERS   1       /// Quantidade de camadas escondidas na rede neural
#define DINO_BRAIN_QTD_INPUT    6       /// Quantidade de neuronios na camada de entrada
#define DINO_BRAIN_QTD_HIDE     6       /// Quantidade de neuronios nas camadas escondidas
#define DINO_BRAIN_QTD_OUTPUT   2       /// Quantidade de neuronios na camada de saida

#define MODO_JOGO 1 // 0 TREINANDO - 1 JOGANDO

enum {NONE, SAIR};
enum{TREINANDO, JOGANDO};

Dinossauro  Dinossauros[POPULACAO_TAMANHO];
Dinossauro* MelhorDinossauro;

ALLEGRO_DISPLAY* janela = NULL;
ALLEGRO_EVENT_QUEUE* fila_eventos = NULL;
ALLEGRO_FONT* fonte = NULL;
ALLEGRO_TIMER* timer = NULL;
ALLEGRO_BITMAP* folha_sprite = NULL;

int cx1 = -1185;
int cx2 = 0;
int speed = 14; // Speed Cenario
int piso = 600;

int hscore = 0;
int score = 0;

int dAnim = 0; // Anime Dinossauro
int pAnim = 0; // Anime Ptero

int rnd = 0; // Sorteio Obstaculo

/*
float vup = 0.5;
float vely = 0;

bool caindo = true;
bool pulando = false;
bool press_space = false;
bool press_down = false;
*/

int number_nuvens = 5;

int Geracao = 0;
int QuantidadeDinossauros = 0;
int DinossaurosMortos = 0;
double DistanciaAtual = 0, DistanciaRecorde = 0;
int DinossaurosVivos = 0;
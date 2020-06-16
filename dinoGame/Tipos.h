#pragma once

struct obj { int wx, wy, x, y, w, h; };
struct obj
	nuvem[5] = {
		765, 225, 0, 0, 110, 40,
		765, 225, 0, 0, 110, 40,
		765, 225, 0, 0, 110, 40,
		765, 225, 0, 0, 110, 40,
		765, 225, 0, 0, 110, 40,
	},
	dino = { 0, 0, 100, 500, 92, 98 };

struct enemyS { int wx, wy, x, y, w, h; };
struct enemyS
	enemy[9] = {
		0, 218, 1500, 515, 34, 78,
		35, 218,  0, 515, 74, 78,
		110, 218, 0, 515, 112, 78,
		222, 185, 0, 485, 49, 106,
		274, 185, 0, 485, 108, 106,
		383, 185, 0, 485, 166, 106,
		554, 212, 0, 405, 95, 80,
		554, 212, 0, 445, 95, 80,
		554, 212, 0,530, 95, 80,
};

typedef struct Dinossauro
{
	float vup;
	float vely;
	double X, Y;

	bool caindo;
	bool pulando;
	bool press_space;
	bool press_down;
	bool morto;
	bool abaixado;

	int dAnim; // Anime Dinossauro
	int ResetarFitness;

	int TamanhoDNA;
	double* DNA;
	double Fitness;

	RedeNeural* Cerebro;
};

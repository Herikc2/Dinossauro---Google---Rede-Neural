#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_native_dialog.h>

#include <stdio.h>
#include <iostream>
#include <list>

#include "rede_neural.h"
#include "Tipos.h"
#include "Variaveis.h"
#include "Controlador.h"
#include "Movimentar.h"
#include "DNA.h"


void desenharCenario() {
	al_draw_bitmap_region(folha_sprite, 0, 315, 1185, 50, cx1, 575, 0); // Chão 475
	al_draw_bitmap_region(folha_sprite, 0, 315, 1185, 50, cx2, 575, 0); // Chão
	for (int i = 0; i < number_nuvens; i++)
		al_draw_bitmap_region(folha_sprite, nuvem[i].wx, nuvem[i].wy, nuvem[i].w, nuvem[i].h, nuvem[i].x, nuvem[i].y, 0); // Nuvem
}

void desenharDinossauro() {
	for (int i = 0; i < QuantidadeDinossauros; i++)
	{
		if (Dinossauros[i].morto == false)
		{
			if (Dinossauros[i].dAnim++ > 10) dAnim = 0;
			al_draw_bitmap_region(folha_sprite, dino.wx + ((dAnim / 6) * dino.w - 1), dino.wy, dino.w, dino.h, Dinossauros[i].X, Dinossauros[i].Y, 0); // Dinossauro
		}
	}
}

void desenhaInimigo() {
	enemy[rnd].x -= speed;

	if (enemy[rnd].x < (enemy[rnd].w + 5) * (-1)) {
		if(score > 2000) rnd = rand() % 9;
		else rnd = rand() % 6;
		enemy[rnd].x = (rand() % 20) * 100 + LARGURA_TELA - 100;
		if (enemy[rnd].x > 1750) enemy[rnd].x = 1750;
	}

	if (pAnim++ > 26) pAnim = 0;

	al_draw_bitmap_region(folha_sprite, enemy[rnd].wx + (rnd > 5) * (pAnim / 14) * enemy[rnd].w, enemy[rnd].wy, enemy[rnd].w, enemy[rnd].h, enemy[rnd].x, enemy[rnd].y, 0); // Cacto
}

/*
void desenhaGameOver() {
	dAnim = 0;
	dino.wx = 184;
	al_draw_bitmap_region(folha_sprite, 290, 0, 455, 155, 200, 150, 0); // Game Over
	if (press_gover && gover) {
		if (hscore < score)	hscore = score;
		rnd = 0;
		score = 0;
		gover = false;
		/*press_gover = false;		
		press_space = false;
		press_down = false;
		pulando = false;
		caindo = true;
		dino.y = piso - dino.h;
		for (int i = 0; i < 9; i++) enemy[i].x = LARGURA_TELA * 2;
	}	
}
*/

void game() {
	aplicarColisao();
	control();
	score++;
	cx1 -= speed;
	cx2 -= speed;
	for (int i = 0; i < number_nuvens; i++) {
		if (nuvem[i].x-- < -100) {
			nuvem[i].x = (rand() % 20) * 50 + LARGURA_TELA;
			nuvem[i].y = (rand() % 10) * 25 + 150;
		}
	}
	if (cx1 < -1185) {
		cx1 = 0;
		cx2 = 1185;
	}
}

void InicializarNovaPartida() {
	DinossaurosVivos = POPULACAO_TAMANHO;
	DistanciaAtual = 0;
	DinossaurosMortos = 0;
	score = 0;

	for (int i = 0; i < POPULACAO_TAMANHO; i++)
		InicializarDinossauro(i, DNADaVez[i], 300 + (rand() % 200 - 100), 15);
}

void CarregarRede()
{
	FILE* f = fopen("rede", "rb");
	fread(&Dinossauros[0].TamanhoDNA, 1, sizeof(int), f);
	fread(DNADaVez[0], Dinossauros[0].TamanhoDNA, sizeof(double), f);
	fclose(f);
}

void configuracoesInicias() {
	inicializarAllegro();
	AlocarDinossauros();

	DistanciaRecorde = 0;
	Geracao = 0;
	MelhorDinossauro = &Dinossauros[0];

	if (MODO_JOGO == JOGANDO) {
		//CarregarRede();
		Dinossauros[0].Cerebro = RedeNeural_CarregarRede("cerebro");
	}

	InicializarDNA();
	InicializarNovaPartida();
}

void finalizaAllegro() {
	al_destroy_display(janela);
	al_destroy_font(fonte);
	al_destroy_event_queue(fila_eventos);
	al_destroy_timer(timer);
	al_destroy_bitmap(folha_sprite);
}

void desenhaTxts() {
	al_draw_text(fonte, al_map_rgb(0, 0, 0), 1050 - 69, 55, ALLEGRO_ALIGN_CENTER, "HI");
	al_draw_textf(fonte, al_map_rgb(0, 0, 0), 1050 - 25, 80, ALLEGRO_ALIGN_CENTER, "Geracao: %d", Geracao);
	al_draw_textf(fonte, al_map_rgb(0, 0, 0), 1050, 100, ALLEGRO_ALIGN_CENTER, "Populacao: %d", DinossaurosVivos);
	if(!hscore && !score)
		al_draw_text(fonte, al_map_rgb(0, 0, 0), 1050, 35, ALLEGRO_ALIGN_CENTER, "00000 00000");
	else {
		al_draw_textf(fonte, al_map_rgb(0, 0, 0), 1050, 55, ALLEGRO_ALIGN_CENTER, "%d", score);
		if (!hscore) {
			al_draw_text(fonte, al_map_rgb(0, 0, 0), 1130, 55, ALLEGRO_ALIGN_CENTER, "00000");
		}
		else {
			al_draw_textf(fonte, al_map_rgb(0, 0, 0), 1130, 55, ALLEGRO_ALIGN_CENTER, "%d", hscore);
		}
	}
}

void EncerrarPartida()
{
	if (DistanciaAtual > DistanciaRecorde)
	{
		DistanciaRecorde = DistanciaAtual;
	}
	if (score > hscore)
		hscore = score;
}

void VerificarFimDePartida()
{
	if (DinossaurosMortos == POPULACAO_TAMANHO)
	{
		EncerrarPartida();
		RandomMutations();
		InicializarNovaPartida();
	}
}

void ControlarEstadoDinossauros() { // Função responsavel por calcular a decisão da rede neural e aplicar no dinossauro (ou seja, é a função que faz ele pular, abaixar ou usar o aviao)

	int Abaixar = 0, Pular = 0;
	double Saida[10];
	double Entrada[10];

	for (int i = 0; i < QuantidadeDinossauros; i++)
	{
		if (Dinossauros[i].morto == false)
		{
			Entrada[0] = abs(Dinossauros[i].X - enemy[rnd].x);
			Entrada[1] = enemy[rnd].w;
			Entrada[2] = abs(enemy[rnd].y);
			Entrada[3] = enemy[rnd].h;
			Entrada[4] = fabs(speed);
			Entrada[5] = Dinossauros[i].Y;

			RedeNeural_VetorParaEntrada(Dinossauros[i].Cerebro, Entrada);     /// Enviando informações para a rede neural
			RedeNeural_CalcularSaida(Dinossauros[i].Cerebro);                  /// Calculando a decisão da rede
			RedeNeural_VetorParaSaida(Dinossauros[i].Cerebro, Saida);           /// Extraindo a decisão para vetor ''saida''

			if (Saida[0] == 0.0) {
				Pular = 0;
				Dinossauros[i].press_space = false;
			}
			else {
				Pular = 1;
				Dinossauros[i].press_space = true;
			}

			if (Saida[1] == 0.0) {
				Abaixar = 0;
				Dinossauros[i].press_down = false;
			}
			else {
				Abaixar = 1;
				Dinossauros[i].press_down = true;
			}

			/*
			if (Dinossauros[i].Estado == 0) /// Em pé
			{
				Dinossauros[i].SpriteAtual = 0 + Dinossauros[i].Frame;
			}
			if (Dinossauros[i].Estado == 1) /// Deitado
			{
				Dinossauros[i].SpriteAtual = 2 + Dinossauros[i].Frame;
			}
			if (Dinossauros[i].Estado == 2) /// Pulando
			{
				Dinossauros[i].SpriteAtual = 4 + Dinossauros[i].Frame;
			}*/
		}
	}
}

int main()
{
	bool sair = false;
	int desenha = 1;
	configuracoesInicias();

	while (!sair) {
		ALLEGRO_EVENT evento;

		while (!al_is_event_queue_empty(fila_eventos)) {
			al_wait_for_event(fila_eventos, &evento);

			if (leitorTeclas(evento) == SAIR) sair = true;
			timerEvents(evento, desenha);

			if (desenha && al_is_event_queue_empty(fila_eventos)) {
				al_clear_to_color(al_map_rgb(255, 255, 255));

				desenharCenario();				
				desenhaInimigo();
				desenharDinossauro();
				MovimentarDinossauros();

				AtualizarMelhorDinossauro();
				ControlarEstadoDinossauros();
				atualizaDinossaurosVivos();

				desenhaTxts();
				game();

				DistanciaAtual += fabs(speed);
				VerificarFimDePartida();

				al_flip_display();
				desenha = 0;
			}
		}
	}

	if (MODO_JOGO == TREINANDO) {
		//SalvarRedeArquivo();
		RedeNeural_SalvarRede(MelhorDinossauro->Cerebro, "cerebro");
	}

	finalizaAllegro();
}
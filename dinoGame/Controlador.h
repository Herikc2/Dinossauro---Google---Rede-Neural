#pragma once

void error_msg(const char* text) {
	al_show_native_message_box(janela, "ERRO",
		"Ocorreu o seguinte erro e o programa sera finalizado:",
		text, NULL, ALLEGRO_MESSAGEBOX_ERROR);
}

int inicializarAllegro() {
	if (!al_init()) {
		error_msg("Falha ao inicializar a Allegro");
		return 0;
	}

	al_init_font_addon();

	if (!al_init_ttf_addon()) {
		error_msg("Falha ao inicializar add-on allegro_ttf");
		return -1;
	}

	if (!al_init_image_addon()) {
		error_msg("Falha ao inicializar o addon de imagens");
		return 0;
	}

	//inicializa addon do teclado
	if (!al_install_keyboard()) {
		error_msg("Falha ao inicializar o teclado");
		return 0;
	}

	timer = al_create_timer(1.0 / FPS);
	if (!timer) {
		error_msg("Falha ao criar temporizador");
		return 0;
	}

	janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
	if (!janela) {
		error_msg("Falha ao criar janela");
		al_destroy_timer(timer);
		return 0;
	}
	al_set_window_title(janela, "Dinossauro");

	// Carregando o arquivo de fonte
	fonte = al_load_font("arial.ttf", 22, 0);
	if (!fonte) {
		al_destroy_display(janela);
		error_msg("Falha ao carregar fonte");
		return -1;
	}


	fila_eventos = al_create_event_queue();
	if (!fila_eventos) {
		error_msg("Falha ao criar fila de eventos");
		al_destroy_timer(timer);
		al_destroy_display(janela);
		return 0;
	}

	//carrega a folha de sprites na variavel
	folha_sprite = al_load_bitmap("sprites.bmp");
	if (!folha_sprite) {
		error_msg("Falha ao carregar sprites");
		al_destroy_timer(timer);
		al_destroy_display(janela);
		al_destroy_event_queue(fila_eventos);
		return 0;
	}

	//usa a cor rosa como transparencia
	al_convert_mask_to_alpha(folha_sprite, al_map_rgb(255, 0, 255));

	al_register_event_source(fila_eventos, al_get_keyboard_event_source());
	al_register_event_source(fila_eventos, al_get_display_event_source(janela));
	al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
	al_start_timer(timer);

	return 1;
}

bool closeProgram() {
	int resp = al_show_native_message_box(janela, "Fechar",
		"Deseja sair do programa?", "", NULL, ALLEGRO_MESSAGEBOX_YES_NO);

	if (resp == 1) 
		return true;
	else return false;
}

int leitorTeclas(ALLEGRO_EVENT evento) {
		if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch (evento.keyboard.keycode) {
				case ALLEGRO_KEY_ESCAPE:
					if(closeProgram())
						return SAIR;
					break;
				/*case ALLEGRO_KEY_SPACE:
					if (gover)
						press_gover = true;
					else
						press_space = true;
					break;
				case ALLEGRO_KEY_DOWN:
					if (!gover)
						press_down = true;
					break;*/
			}
		}
		/*else if (evento.type == ALLEGRO_EVENT_KEY_UP) { // Soltar tecla
			switch (evento.keyboard.keycode) {
				case ALLEGRO_KEY_SPACE:
					if (gover)
						press_gover = false;
					else
						press_space = false;
					break;
				case ALLEGRO_KEY_DOWN:
					if (!gover)
						press_down = false;
					break;
			}
		}*/
		//se clicou para fechar a janela
		else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			if (closeProgram())
				return SAIR;
		}
	return NONE;
}

void timerEvents(ALLEGRO_EVENT evento, int& desenha) {
	if (evento.type == ALLEGRO_EVENT_TIMER) {
		desenha = 1;
	}
}

void control() {
	for (int i = 0; i < QuantidadeDinossauros; i++)
	{
		if (Dinossauros[i].morto == false)
		{
			Dinossauros[i].pulando = (Dinossauros[i].press_space && !Dinossauros[i].caindo && Dinossauros[i].Y >= piso - dino.h * 2);
			Dinossauros[i].caindo = (!Dinossauros[i].pulando && Dinossauros[i].Y + dino.h < piso);
			if (Dinossauros[i].pulando) Dinossauros[i].vely -= Dinossauros[i].vup;
			if (Dinossauros[i].caindo) Dinossauros[i].vely += Dinossauros[i].vup;

			if (Dinossauros[i].press_down && !Dinossauros[i].pulando && !Dinossauros[i].caindo) {
				dino.wy = 102;
				dino.w = 126;
				dino.h = 59;
				Dinossauros[i].Y = piso;
			}
			else {
				dino.wy = 0;
				dino.w = 92;
				dino.h = 98;
			}

			if (Dinossauros[i].pulando || Dinossauros[i].caindo) {
				Dinossauros[i].Y += Dinossauros[i].vely;
				dino.wx = 184;
				Dinossauros[i].dAnim = 0;
			}
			else {
				Dinossauros[i].vely = 0;
				dino.wx = 0;
			}

			if (Dinossauros[i].Y + dino.h >= piso) Dinossauros[i].Y = piso - dino.h;
		}
	}
}

bool colidir(int Ax, int Ay, int Bx, int By, int Aw, int Ah, int Bw, int Bh) {
	if (Ax + Aw > Bx && Ax < Bx + Bw && Ay + Ah > By && Ay < By + Bh)
		return true;
	else
		return false;		
}

void aplicarColisao() {
	for (int i = 0; i < QuantidadeDinossauros; i++)
	{
		if (Dinossauros[i].morto == false)
		{
			if (colidir(Dinossauros[i].X, Dinossauros[i].Y + 34, enemy[rnd].x, enemy[rnd].y, 70, 32, enemy[rnd].w, enemy[rnd].h) ||
				colidir(Dinossauros[i].X + 44, Dinossauros[i].Y, enemy[rnd].x, enemy[rnd].y, 46, 33, enemy[rnd].w, enemy[rnd].h) ||
				colidir(Dinossauros[i].X + 22, Dinossauros[i].Y + 68, enemy[rnd].x, enemy[rnd].y, 30, 30, enemy[rnd].w, enemy[rnd].h)) {
				DinossaurosMortos++;
				Dinossauros[i].morto = true;
			}
		}
	}
}

double getRandomValue()
{
	return (rand() % 20001 / 10.0) - 1000.0;
	//return (rand()%201/10.0) - 10.0;
	//return (rand()%2001/1000.0) - 1.0;
	//return (rand()%2001/10000.0) - 0.1;

	//return rand()%3 - 1;
}

void AtualizarMelhorDinossauro() {
	if (MelhorDinossauro->morto == true) {
		for (int i = 0; i < POPULACAO_TAMANHO; i++) {
			if (Dinossauros[i].morto == false) {
				MelhorDinossauro = &Dinossauros[i];
				break;
			}
		}
	}
}

void atualizaDinossaurosVivos() {
	int temp_dinossauros_vivos = 0;
	for (int i = 0; i < POPULACAO_TAMANHO; i++) 
		if (Dinossauros[i].morto == false)
			temp_dinossauros_vivos++;
	DinossaurosVivos = temp_dinossauros_vivos;
}

void SalvarRedeArquivo()
{
	double Maior = 0;
	int Indice = 0;
	for (int i = 0; i < POPULACAO_TAMANHO; i++)
	{
		if (Dinossauros[i].Fitness > Maior)
		{
			Maior = Dinossauros[i].Fitness;
			Indice = i;
		}
	}

	char String[1000];
	sprintf(String, "redes\\%.2f - [%d,%d,%d,%d]",
		DistanciaRecorde,
		DINO_BRAIN_QTD_LAYERS,
		DINO_BRAIN_QTD_INPUT,
		DINO_BRAIN_QTD_HIDE,
		DINO_BRAIN_QTD_OUTPUT);

	FILE* f = fopen(String, "wb");
	fwrite(&Dinossauros[Indice].TamanhoDNA, 1, size_t(1), f);
	fwrite(Dinossauros[Indice].DNA, Dinossauros[Indice].TamanhoDNA, sizeof(double), f);
	fclose(f);

}

// ------------------------ DINOSSAURO -------------------------------

void InicializarDinossauro(int Indice, double* DNA, double X, double Y)
{
	Dinossauros[Indice].vup = 0.5;
	Dinossauros[Indice].vely = 0;
	Dinossauros[Indice].X = dino.x;
	Dinossauros[Indice].Y = dino.y;
	Dinossauros[Indice].dAnim = 0;
	Dinossauros[Indice].caindo = true;
	Dinossauros[Indice].pulando = false;
	Dinossauros[Indice].press_space = false;
	Dinossauros[Indice].press_down = false;
	Dinossauros[Indice].morto = false;
	Dinossauros[Indice].abaixado = false;

	if (Dinossauros[Indice].ResetarFitness == 1) {
		Dinossauros[Indice].Fitness = 0;
	}
	else {
		Dinossauros[Indice].morto = true;
		DinossaurosMortos++;
	}

	if (DNA == NULL) { // Inicializa aleatoriamente
		for (int i = 0; i < Dinossauros[Indice].TamanhoDNA; i++)
			Dinossauros[Indice].DNA[i] = getRandomValue();
	}
	else { // Inicializa com o DNA passado pelo parametro
		for (int i = 0; i < Dinossauros[Indice].TamanhoDNA; i++)
			Dinossauros[Indice].DNA[i] = DNA[i];
	}

	ReneNeural_VetorParaCamadas(Dinossauros[Indice].Cerebro, Dinossauros[Indice].DNA);
}

void AlocarDinossauro()
{
	int Tamanho;

	Dinossauros[QuantidadeDinossauros].ResetarFitness = 1;

	Dinossauros[QuantidadeDinossauros].Cerebro = RedeNeural_CriarRedeNeural(DINO_BRAIN_QTD_LAYERS,
		DINO_BRAIN_QTD_INPUT,
		DINO_BRAIN_QTD_HIDE,
		DINO_BRAIN_QTD_OUTPUT);

	Tamanho = RedeNeural_QuantidadePesos(Dinossauros[QuantidadeDinossauros].Cerebro);

	Dinossauros[QuantidadeDinossauros].TamanhoDNA = Tamanho;
	Dinossauros[QuantidadeDinossauros].DNA = (double*)malloc(Tamanho * sizeof(double));

	InicializarDinossauro(QuantidadeDinossauros, NULL, 0, 0);

	QuantidadeDinossauros = QuantidadeDinossauros + 1;
}

void AlocarDinossauros()
{
	for (int i = 0; i < POPULACAO_TAMANHO; i++)
	{
		AlocarDinossauro();
	}
}
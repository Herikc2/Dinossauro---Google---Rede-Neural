#pragma once

double* DNADaVez[POPULACAO_TAMANHO];

void InicializarDNA() {
	int TamanhoDNA = Dinossauros[0].TamanhoDNA;

	for (int i = 0; i < POPULACAO_TAMANHO; i++) {
		DNADaVez[i] = (double*)malloc(TamanhoDNA * sizeof(double));
		for (int j = 0; j < TamanhoDNA; j++)
			DNADaVez[i][j] = getRandomValue();
	}
}

void RandomMutations()
{
	static double RangeRandom = Dinossauros[0].TamanhoDNA;

	Dinossauro* Vetor[POPULACAO_TAMANHO];
	Dinossauro* Temp;

	for (int i = 0; i < POPULACAO_TAMANHO; i++)
		Vetor[i] = &Dinossauros[i];

	for (int i = 0; i < POPULACAO_TAMANHO; i++)
		for (int j = 0; j < POPULACAO_TAMANHO - 1; j++)
			if (Vetor[j]->Fitness < Vetor[j + 1]->Fitness)
			{
				Temp = Vetor[j];
				Vetor[j] = Vetor[j + 1];
				Vetor[j + 1] = Temp;
			}

	int Step = 1;
	for (int i = 0; i < Step; i++) // Clonando individuos
		for (int j = Step + i; j < POPULACAO_TAMANHO; j = j + Step)
			for (int k = 0; k < Vetor[j]->TamanhoDNA; k++)
				Vetor[j]->DNA[k] = Vetor[i]->DNA[k]; // individuo 'j' recebe dna do individuo 'i'

	for (int j = Step; j < POPULACAO_TAMANHO; j++) { // Aplicando random mutations
	
		int tipo;
		int mutations = (rand() % (int)RangeRandom) + 1;

		for (int k = 0; k < mutations; k++) {
			tipo = rand() % 3;

			int indice = rand() % Vetor[j]->TamanhoDNA;
			switch (tipo)
			{
			case 0:
			{
				Vetor[j]->DNA[indice] = getRandomValue(); // Valor Aleatorio

			}   break;
			case 1:
			{
				double number = (rand() % 10001) / 10000.0 + 0.5;
				Vetor[j]->DNA[indice] = Vetor[j]->DNA[indice] * number; // Multiplicação aleatoria

			}   break;
			case 2:
			{
				double number = getRandomValue() / 100.0;
				Vetor[j]->DNA[indice] = Vetor[j]->DNA[indice] + number; // Soma aleatoria

			}   break;
			}
		}
	}

	for (int j = 0; j < POPULACAO_TAMANHO; j++) // Copiando novos DNAs para DNAsDaVez
		for (int k = 0; k < Dinossauros[j].TamanhoDNA; k++)
			DNADaVez[j][k] = Dinossauros[j].DNA[k];

	for (int i = 0; i < POPULACAO_TAMANHO; i++)
		Vetor[i]->ResetarFitness = 1;

	printf("Range Random: %f\n", RangeRandom);
	RangeRandom = RangeRandom * 0.99;
	if (RangeRandom < 20)
	{
		RangeRandom = 20;
	}

	Geracao++;
}
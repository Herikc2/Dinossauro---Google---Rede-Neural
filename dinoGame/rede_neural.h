#pragma once
#pragma warning(disable : 4996)

#include <math.h>
#include <time.h>

#define TAXA_APRENDIZADO    0.1
#define TAXA_PESO_INICIAL   1.0
#define BIAS                1

#define AtivacaoOcultas(X)        relu(X)
#define AtivacaoSaida(X)          relu(X)

typedef struct Neuronio
{
	double* Peso;
	double  Erro;
	double  Saida;
	int QuantidadeLigacoes;
};

typedef struct Camada
{
	Neuronio* Neuronios;
	int QuantidadeNeuronios;
};

typedef struct RedeNeural
{
	Camada  CamadaEntrada;
	Camada* CamadaEscondida;
	Camada  CamadaSaida;
	int QuantidadeEscondidas;
};

double relu(double X)
{
	if (X < 0)
		return 0;
	else
		return X;
}

void ReneNeural_VetorParaCamadas(RedeNeural* Rede, double* Vetor) { // Modificado
	int j = 0;

	for (int i = 0; i < Rede->QuantidadeEscondidas; i++) {
		for (int k = 0; k < Rede->CamadaEscondida[i].QuantidadeNeuronios; k++) {
			for (int l = 0; l < Rede->CamadaEscondida[i].Neuronios[k].QuantidadeLigacoes; l++) {
				Rede->CamadaEscondida[i].Neuronios[k].Peso[l] = Vetor[j];
				j++;
			}
		}
	}

	for (int k = 0; k < Rede->CamadaSaida.QuantidadeNeuronios; k++) {
		for (int l = 0; l < Rede->CamadaSaida.Neuronios[k].QuantidadeLigacoes; l++) {
			Rede->CamadaSaida.Neuronios[k].Peso[l] = Vetor[j];
			j++;
		}
	}
}

void RedeNeural_VetorParaEntrada(RedeNeural* Rede, double* VetorEntrada) {
	for (int i = 0; i < Rede->CamadaEntrada.QuantidadeNeuronios - BIAS; i++) 
		Rede->CamadaEntrada.Neuronios[i].Saida = VetorEntrada[i];
}

int RedeNeural_QuantidadePesos(RedeNeural* Rede) {
	int soma = 0;

	for (int i = 0; i < Rede->QuantidadeEscondidas; i++) 
		for (int j = 0; j < Rede->CamadaEscondida[i].QuantidadeNeuronios; j++) 
			soma += Rede->CamadaEscondida[i].Neuronios[j].QuantidadeLigacoes;

	return soma;
}

void RedeNeural_VetorParaSaida(RedeNeural* Rede, double* VetorSaida) {
	for (int i = 0; i < Rede->CamadaSaida.QuantidadeNeuronios; i++)
		VetorSaida[i] = Rede->CamadaSaida.Neuronios[i].Saida;
}

void RedeNeural_CalcularSaida(RedeNeural* Rede) {
	int k;
	double Somatorio = 0;

	for (int i = 0; i < Rede->CamadaEscondida[0].QuantidadeNeuronios - BIAS; i++) {
		Somatorio = 0;
		for (int j = 0; j < Rede->CamadaEntrada.QuantidadeNeuronios; j++)
			Somatorio += Rede->CamadaEntrada.Neuronios[j].Saida * Rede->CamadaEscondida[0].Neuronios[i].Peso[j];
		Rede->CamadaEscondida[0].Neuronios[i].Saida = AtivacaoOcultas(Somatorio);
	}

	for (k = 1; k < Rede->QuantidadeEscondidas; k++) {
		for (int i = 0; i < Rede->CamadaEscondida[k].QuantidadeNeuronios - BIAS; i++) {
			Somatorio = 0;
			for (int j = 0; j < Rede->CamadaEscondida[k - 1].QuantidadeNeuronios; j++)
				Somatorio += Rede->CamadaEscondida[k - 1].Neuronios[j].Saida * Rede->CamadaEscondida[k].Neuronios[i].Peso[j];
			Rede->CamadaEscondida[k].Neuronios[i].Saida = AtivacaoOcultas(Somatorio);
		}
	}

	for (int i = 0; i < Rede->CamadaSaida.QuantidadeNeuronios; i++) {
		Somatorio = 0;
		for (int j = 0; j < Rede->CamadaEscondida[k - 1].QuantidadeNeuronios; j++)
			Somatorio += Rede->CamadaEscondida[k - 1].Neuronios[j].Saida * Rede->CamadaSaida.Neuronios[i].Peso[j];
		Rede->CamadaSaida.Neuronios[i].Saida = AtivacaoSaida(Somatorio);
	}

}

void RedeNeural_CriarNeuronio(Neuronio* neuronio, int QuantidadeLigacoes) {
	neuronio->QuantidadeLigacoes = QuantidadeLigacoes;
	neuronio->Peso = (double*)malloc(QuantidadeLigacoes * sizeof(double)); // Estudar

	for (int i = 0; i < QuantidadeLigacoes; i++)
	{
		neuronio->Peso[i] = rand() % 2000 - 1000;
	}
	neuronio->Erro = 0;
	neuronio->Saida = 1;
}

RedeNeural* RedeNeural_CriarRedeNeural(int QuantidadeEscondidas, int QtdNeuroniosEntrada, int QtdNeuroniosEscondida, int QtdNeuroniosSaida) {
	int i, j;

	QtdNeuroniosEntrada += BIAS;
	QtdNeuroniosEscondida += BIAS;

	RedeNeural* Rede = (RedeNeural*)malloc(sizeof(RedeNeural)); // Estudar

	// Camada de Entrada
	Rede->CamadaEntrada.QuantidadeNeuronios = QtdNeuroniosEntrada;
	Rede->CamadaEntrada.Neuronios = (Neuronio*)malloc(QtdNeuroniosEntrada * sizeof(Neuronio)); // Estudar

	for (i = 0; i < QtdNeuroniosEntrada; i++)
		Rede->CamadaEntrada.Neuronios[i].Saida = 1.0;

	// Camada Escondida
	Rede->QuantidadeEscondidas = QuantidadeEscondidas;
	Rede->CamadaEscondida = (Camada*)malloc(QuantidadeEscondidas * sizeof(Camada));
	
	for (i = 0; i < QuantidadeEscondidas; i++) {
		Rede->CamadaEscondida[i].QuantidadeNeuronios = QtdNeuroniosEscondida;
		Rede->CamadaEscondida[i].Neuronios = (Neuronio*)malloc(QtdNeuroniosEscondida * sizeof(Neuronio));

		for (j = 0; j < QtdNeuroniosEscondida; j++) {
			if (i == 0)
				RedeNeural_CriarNeuronio(&Rede->CamadaEscondida[i].Neuronios[j], QtdNeuroniosEntrada);
			else
				RedeNeural_CriarNeuronio(&Rede->CamadaEscondida[i].Neuronios[j], QtdNeuroniosEscondida);
		}
	}

	// Camada Saida
	Rede->CamadaSaida.QuantidadeNeuronios = QtdNeuroniosSaida;
	Rede->CamadaSaida.Neuronios = (Neuronio*)malloc(QtdNeuroniosSaida * sizeof(Neuronio));

	for (j = 0; j < QtdNeuroniosSaida; j++)
		RedeNeural_CriarNeuronio(&Rede->CamadaSaida.Neuronios[j], QtdNeuroniosSaida);

	return Rede;
}

RedeNeural* RedeNeural_DestruirRedeNeural(RedeNeural* Rede)
{
	int i, j;

	free(Rede->CamadaEntrada.Neuronios);
	/////////////////////////////////////////////////////////////
	for (j = 0; j < Rede->QuantidadeEscondidas; j++)
	{
		for (i = 0; i < Rede->CamadaEscondida[j].QuantidadeNeuronios; i++)
		{
			free(Rede->CamadaEscondida[j].Neuronios[i].Peso);
		}
		free(Rede->CamadaEscondida[j].Neuronios);
	}
	free(Rede->CamadaEscondida);
	/////////////////////////////////////////////////////////
	for (i = 0; i < Rede->CamadaSaida.QuantidadeNeuronios; i++)
	{
		free(Rede->CamadaSaida.Neuronios[i].Peso);
	}
	free(Rede->CamadaSaida.Neuronios);

	return NULL;
}

RedeNeural* RedeNeural_CarregarRede(const char* String)
{
	int i, j, k;
	FILE* f;
	RedeNeural* Temp;

	int QtdEscondida, QtdNeuroEntrada, QtdNeuroSaida, QtdNeuroEscondida;

	f = fopen(String, "rb");
	if (f != NULL)
	{
		fread(&QtdEscondida, 1, sizeof(int), f);
		fread(&QtdNeuroEntrada, 1, sizeof(int), f);
		fread(&QtdNeuroEscondida, 1, sizeof(int), f);
		fread(&QtdNeuroSaida, 1, sizeof(int), f);

		Temp = RedeNeural_CriarRedeNeural(QtdEscondida, QtdNeuroEntrada, QtdNeuroEscondida, QtdNeuroSaida);

		for (k = 0; k < Temp->QuantidadeEscondidas; k++)
		{
			for (i = 0; i < Temp->CamadaEscondida[k].QuantidadeNeuronios; i++)
			{
				for (j = 0; j < Temp->CamadaEscondida[k].Neuronios[i].QuantidadeLigacoes; j++)
				{
					fread(&(Temp->CamadaEscondida[k].Neuronios[i].Peso[j]), 1, 8, f);
				}
			}
		}
		for (i = 0; i < Temp->CamadaSaida.QuantidadeNeuronios; i++)
		{
			for (j = 0; j < Temp->CamadaSaida.Neuronios[i].QuantidadeLigacoes; j++)
			{
				fread(&(Temp->CamadaSaida.Neuronios[i].Peso[j]), 1, 8, f);
			}
		}

		printf("carregada rede neural");
		fclose(f);
		return Temp;
	}
}

void RedeNeural_SalvarRede(RedeNeural* Temp, const char* String)
{
	int i, j, k;
	FILE* f;

	f = fopen(String, "wb");
	if (f != NULL)
	{
		fwrite(&Temp->QuantidadeEscondidas, 1, sizeof(int), f);
		fwrite(&Temp->CamadaEntrada.QuantidadeNeuronios, 1, sizeof(int), f);
		fwrite(&Temp->CamadaEscondida[0].QuantidadeNeuronios, 1, sizeof(int), f);
		fwrite(&Temp->CamadaSaida.QuantidadeNeuronios, 1, sizeof(int), f);

		for (k = 0; k < Temp->QuantidadeEscondidas; k++)
		{
			for (i = 0; i < Temp->CamadaEscondida[k].QuantidadeNeuronios; i++)
			{
				for (j = 0; j < Temp->CamadaEscondida[k].Neuronios[i].QuantidadeLigacoes; j++)
				{
					fwrite(&Temp->CamadaEscondida[k].Neuronios[i].Peso[j], 1, 8, f);
				}
			}
		}

		for (i = 0; i < Temp->CamadaSaida.QuantidadeNeuronios; i++)
		{
			for (j = 0; j < Temp->CamadaSaida.Neuronios[i].QuantidadeLigacoes; j++)
			{
				fwrite(&Temp->CamadaSaida.Neuronios[i].Peso[j], 1, 8, f);
			}
		}

		printf("rede neural salva");
		fclose(f);
	}
}
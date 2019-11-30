/*
 * dados.c
 *
 *  Created on: 30 de nov de 2019
 *      Author: Matheus e Clovis
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dados.h"

struct dados{
	char encontro[256];
	float altura_media;
	float altura_max;
	float onda_zero;
	float pico_energia;
	float direcao_pico;
	float temperatura_mar;
};

dado_t * criar_dado (dado_t *temp)
{

	dado_t * meu_novo_dado = malloc(sizeof(struct dados));

	if (meu_novo_dado == NULL) {
		perror("Criar dado!\n");
		exit(-2);
	}

	strncpy(meu_novo_dado->encontro, temp->encontro, 128);
	meu_novo_dado->altura_media = temp->altura_media;
	meu_novo_dado->altura_max = temp->altura_max;
	meu_novo_dado->onda_zero = temp->onda_zero;
	meu_novo_dado->pico_energia = temp->pico_energia;
	meu_novo_dado->direcao_pico = temp->direcao_pico;
	meu_novo_dado->temperatura_mar = temp->temperatura_mar;
	return meu_novo_dado;
}


dado_t **ler_dados(char *CoastalDataSystem, int *n_linhas){

	char tempo[256];

	int i=0, linhas = 0;

	FILE *fp = fopen(CoastalDataSystem,"r");

	if (!fp){
		perror("ler_dados_csv");
		exit(-1);
	}

	dado_t temp;

	/* Ignora primeira linha */
	fgets(tempo, 256, fp);

	/* Contar todas as linhas: use while e fgets() */
	while (fgets (tempo, 256, fp) != NULL){
		linhas++;
	}

	/* Alocando mem�ria:
	 * Agora � um vetor de ponteiros */
	dado_t **dados = malloc(sizeof(struct dados*) * linhas);
	if (dados == NULL) {
		perror("ler dados!\n");
		exit(-2);
	}
	rewind(fp);
	/* Ignorando a primeira linha */
	fgets(tempo,256, fp);
	while(fscanf(fp,"%255[^,], %f, %f, %f, %f, %f, %f\n", temp.encontro,
			&temp.altura_media,  &temp.altura_max,&temp.onda_zero,
			&temp.pico_energia,&temp.direcao_pico,&temp.temperatura_mar) == 7){
		/* Cria um novo dado abstrato e armazena a sua refer�ncia */
		dados[i] = criar_dado(&temp);
		i++;
	}
	*n_linhas = linhas;

	fclose(fp);
	return dados;
}

void quick_sort(dado_t **dados, int esq, int dir)
{
	int p;

	if(esq < dir){
		p = particao(dados, esq, dir);
		quick_sort(dados, esq, p);
		quick_sort(dados, p+1, dir);
	}
}
int med_tres(dado_t **dados, int esq, int dir)
{
	int med;

	med = (esq + dir) / 2;

	if(dados[dir]->temperatura_mar < dados[esq]->temperatura_mar)
		swap(dados, esq, dir);

	if(dados[med]->temperatura_mar < dados[esq]->temperatura_mar)
		swap(dados, med, esq);

	if(dados[dir]->temperatura_mar < dados[med]->temperatura_mar)
		swap(dados, dir, med);

	return med;
}

int particao(dado_t **dados, int esq, int dir)
{

	int med = med_tres(dados, esq, dir);
	dado_t *pivot = dados[med];
	int i = esq - 1;
	int j = dir + 1;

	for(;;){
		do{
			i = i + 1;
		}while(dados[i]->temperatura_mar < pivot->temperatura_mar);
		do{
			j = j - 1;
		}while(dados[j]->temperatura_mar > pivot->temperatura_mar);

		if(i >= j)
			return j;
		swap(dados, i, j);
	}
	return j;
}

int maximo(dado_t **dados, int n_linhas){

	int curr = 0;
	int max = 0;
	for(curr = 0; curr < n_linhas; curr++){
		if(dados[curr]->direcao_pico > max){ max = dados[curr]->direcao_pico; }
	}
	return max;
}

void counting_sort(dado_t **dados, int n_linhas){

	int curr = 0;
	int max = maximo(dados, n_linhas);
	dado_t **counting_array = calloc(max, sizeof(struct dados*)); // aloca e zera o vetor
	for(curr = 0; curr < n_linhas; curr ++){
		counting_array[(int)dados[curr]->direcao_pico]->direcao_pico++;
	}
	int num = 0;
	curr = 0;
	while(curr <= n_linhas){
		while(counting_array[num]->direcao_pico > 0){
			dados[curr]->direcao_pico = num;
			counting_array[num]->direcao_pico--;
			curr++;
			if(curr > n_linhas){ break; }
		}
		num++;
	}
}

void imprime_dados(dado_t *dados)
{
	printf("%s, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f\n", imprime_encontro(dados), imprime_altura_media(dados)
			,imprime_altura_max(dados),imprime_onda_zero(dados),imprime_pico_energia(dados),imprime_direcao_pico(dados),imprime_temperatura_mar(dados));
}
char * imprime_encontro(dado_t *dados)
{
	return (dados->encontro);
}

float imprime_altura_media(dado_t *dados)
{
	return (dados->altura_media);
}
float imprime_direcao_pico(dado_t *dados)
{
	return (dados->direcao_pico);
}

float imprime_temperatura_mar(dado_t *dados)
{
	return (dados->temperatura_mar);
}

float imprime_altura_max(dado_t *dados)
{
	return (dados->altura_max);
}

float imprime_onda_zero(dado_t *dados)
{
	return (dados->onda_zero);
}

float imprime_pico_energia(dado_t *dados)
{
	return (dados->pico_energia);
}
void swap(dado_t **dados, int i, int j)
{
	dado_t *temp = dados[i];
	dados[i] = dados[j];
	dados[j] = temp;
}
void liberar_dados(dado_t **dados, int n_linhas)
{
	int i;
	for(i=0;i<n_linhas;i++){
		free(dados[i]);
	}
	free(dados);
}

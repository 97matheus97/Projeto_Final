#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <math.h>
#include "dados.h"

int main()
{
	struct timespec t_1;
	struct timespec t_2;
	int i, j, n_linhas;
	long long tempo_nano, tempo_tnano=0;
	long tempo_seg, tempo_tseg=0;
	for (j=0; j < 1; j++){
		n_linhas = 0;
		dado_t **dados = ler_dados("CoastalDataSystem.csv", &n_linhas);
		clock_gettime(CLOCK_MONOTONIC ,&t_1);
		quick_sort(dados, 0, n_linhas-1);
		clock_gettime(CLOCK_MONOTONIC ,&t_2);
		tempo_nano = ((t_2.tv_nsec) - (t_1.tv_nsec));
		if (tempo_nano < 0 ) {
			t_2.tv_sec = t_2.tv_sec - 1;
			tempo_nano = tempo_nano + 1000000000;
		}
		tempo_seg = (t_2.tv_sec - t_1.tv_sec);
		//printf("Tempo de ordenacao: %lu.%llu segundos\n", tempo_seg, tempo_nano);
		tempo_tseg = tempo_tseg + tempo_seg;
		tempo_tnano = tempo_tnano + tempo_nano;
		/*for (i =0; i < n_linhas; i++){
			imprime_dados(dados[i]);
		}*/
		liberar_dados(dados, n_linhas); //libera��o do malloc.
	}
	printf("Tempo de ordenacao QUICKSORT: %lu.%llu segundos", (tempo_tseg/20), (tempo_tnano/20));
	for (j=0; j < 1; j++){
			n_linhas = 0;
			dado_t **dados = ler_dados("CoastalDataSystem.csv", &n_linhas);
			clock_gettime(CLOCK_MONOTONIC ,&t_1);
			counting_sort(dados, n_linhas);
			clock_gettime(CLOCK_MONOTONIC ,&t_2);
			tempo_nano = ((t_2.tv_nsec) - (t_1.tv_nsec));
			if (tempo_nano < 0 ) {
				t_2.tv_sec = t_2.tv_sec - 1;
				tempo_nano = tempo_nano + 1000000000;
			}
			tempo_seg = (t_2.tv_sec - t_1.tv_sec);
			//printf("Tempo de ordenacao: %lu.%llu segundos\n", tempo_seg, tempo_nano);
			tempo_tseg = tempo_tseg + tempo_seg;
			tempo_tnano = tempo_tnano + tempo_nano;
			for (i =0; i < n_linhas; i++){
				imprime_dados(dados[i]);
			}
			liberar_dados(dados, n_linhas); //libera��o do malloc.
		}
		printf("Tempo de ordenacao COUNTINGSORT: %lu.%llu segundos", (tempo_tseg), (tempo_tnano));
	return 0;
}

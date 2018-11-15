#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<pthread.h>
#include<errno.h>
#include"ui.c"

struct dna{
	unsigned char antena;
	unsigned char pheromone;
	unsigned char max;
};

#define TRUE 1

int start[2] = {0,100};
int end[2] = {50,0};

struct dna dnas[12];
pthread_t evaluation[12];

int mutation = 0.05;
int best = 0;

void initPop(){
	for(int i = 0;i < 12; i++){
		dnas[i].antena = rand() % 256;
		dnas[i].pheromone = rand() % 256;
		dnas[i].max = rand() % 256;
	}
}

void *evaluate(void *arg){
	
}

void selection(){
	int bestFit = 0;
	for(int i = 0; i < 5; i++)
		bestFit += points[best][i];
	for(int i = 0; i < 12; i++){
		if(i != best){
				int fit = 0;
				for(int j = 0; j < 5; j++)
					fit += points[i][j];
				if(fit < bestFit){
					best = i;
					bestFit = fit;
				}
		}
	}
}

void reproduct(){
	for(int i = 0; i < 12; i++){
		if(i != best && points[best][generation % 5] > points[i][generation % 5]){
			dnas[i].pheromone += dnas[best].pheromone;
			dnas[i].pheromone /=2;
			dnas[i].max += dnas[best].max;
			dnas[i].max /=2;
			for(int j = 0; j < 8; j++){
				if((dnas[best].antena >> j & 1) != (dnas[i].antena >> j & 1)){
					dnas[i].antena ^= (rand() % 2)<<j;
				}
			}
			int aux = rand() % 10;
			if(aux == 8){
				dnas[i].pheromone += (-(rand() % 2))*dnas[i].pheromone*mutation;
			}else if(aux == 9){
				dnas[i].max += (-(rand() % 2))*dnas[i].max*mutation;
			}else if((rand() % 100)*0.01 < mutation){
				dnas[i].antena ^= 1<<aux;
			}
		}
	}
}

void printOutput(FILE * file){
	fprintf(file,"generation: %d\nBest: %d\n",generation,best);
	for(int i = 0; i< 12; i++)
		fprintf(file,"id: %d\tpheromone: %d\t max: %d\tantena: %d\n",i,dnas[i].pheromone,dnas[i].max,dnas[i].antena);
}

int main(int argc, char *argv[]){
	counter = argc;
  FILE *file = fopen("output.txt","w");
	pthread_t ui;
	pthread_mutex_init(&pause,NULL);
	pthread_mutex_init(&save_exit,NULL);

  printf("Commands:\ng-Activate/Deactivate Interface\np-Pause/Unpause\nq-Save And Exit\n");
	srand(time(NULL));
	pthread_create(&ui,NULL,UI,argv);

  initPop();

	while(TRUE){
    for(int i = 0; i < 12; i++)
      pthread_create(&evaluation[i],NULL,evaluate,&i);
    for(int i = 0; i < 12; i++)
      pthread_join(evaluation[i],NULL);
    selection();
    printOutput(file);
    reproduct();
		pthread_mutex_lock(&pause);
		pthread_mutex_unlock(&pause);
		int e = pthread_mutex_trylock(&save_exit);
		if(e == EBUSY){
			break;
		}
		pthread_mutex_unlock(&save_exit);
    generation++;
	}

  fclose(file);

	return 0;
}

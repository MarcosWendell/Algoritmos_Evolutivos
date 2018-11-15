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

struct dna dnas[12];
pthread_t evaluation[12];

int mutation = 0.05;
int best = 0;

void initPop(){
	for(int i = 0;i < 12; i++){
		dnas[i].antena = rand() % 256;
		dnas[i].pheromone = 0;
		dnas[i].max = 0;
	}
}

void walk(int nest, int ant){
	int pheromoneEsq = 0, pheromoneDir = 0,x =nests[nest][ant][0] ,y =nests[nest][ant][1];
	int aux = fields[nest][x][y];
	aux += dnas[nest].pheromone;
	if(aux > dnas[nest].max)
		aux = dnas[nest].max;
	fields[nest][x][y] = aux;
	switch(directions[nest][ant]){
		case 0:
			if((dnas[nest].antena & 1) && x >= 1)
				pheromoneEsq += fields[nest][x-1][y];
			if((dnas[nest].antena & 1) && x < 99)
				pheromoneDir += fields[nest][x+1][y];
			if(((dnas[nest].antena<<1) & 1) && x >= 2)
				pheromoneEsq += fields[nest][x-2][y];
			if(((dnas[nest].antena<<1) & 1) && x < 98)
				pheromoneDir += fields[nest][x+2][y];
			if(((dnas[nest].antena<<2) & 1) && x >= 3)
				pheromoneEsq += fields[nest][x-3][y];
			if(((dnas[nest].antena<<2) & 1) && x < 97)
				pheromoneDir += fields[nest][x+3][y];
			if(((dnas[nest].antena<<3) & 1) && x >= 1 && y < 99)
				pheromoneEsq += fields[nest][x-1][y+1];
			if(((dnas[nest].antena<<3) & 1) && x < 99 && y < 99)
				pheromoneDir += fields[nest][x+1][y+1];
			if(((dnas[nest].antena<<4) & 1) && x >= 2 && y < 99)
				pheromoneEsq += fields[nest][x-2][y+1];
			if(((dnas[nest].antena<<4) & 1) && x < 98 && y < 99)
				pheromoneDir += fields[nest][x+2][y+1];
			if(((dnas[nest].antena<<5) & 1) && x >= 3 && y < 99)
				pheromoneEsq += fields[nest][x-3][y+1];
			if(((dnas[nest].antena<<5) & 1) && x < 97 && y < 99)
				pheromoneDir += fields[nest][x+3][y+1];
			if(((dnas[nest].antena<<6) & 1) && x >= 1 && y < 98)
				pheromoneEsq += fields[nest][x-1][y+2];
			if(((dnas[nest].antena<<6) & 1) && x < 99 && y < 98)
				pheromoneDir += fields[nest][x+1][y+2];
			if(((dnas[nest].antena<<7) & 1) && x >= 2 && y < 98)
				pheromoneEsq += fields[nest][x-2][y+2];
			if(((dnas[nest].antena<<7) & 1) && x < 98 && y < 98)
				pheromoneDir += fields[nest][x+2][y+2];

			if(y < 99){
				if(pheromoneEsq > pheromoneDir)
					directions[nest][ant] = 1;
				else
					directions[nest][ant] = 3;
				nests[nest][ant][1] = y + 1;
			}else{
				directions[nest][ant] = 2;
			}
		break;
		case 1:
			if((dnas[nest].antena & 1) && y < 99)
				pheromoneEsq += fields[nest][x][y+1];
			if((dnas[nest].antena & 1) && y >=1)
				pheromoneDir += fields[nest][x][y-1];
			if(((dnas[nest].antena<<1) & 1) && y < 98)
				pheromoneEsq += fields[nest][x][y+2];
			if(((dnas[nest].antena<<1) & 1) && y >= 2)
				pheromoneDir += fields[nest][x][y-2];
			if(((dnas[nest].antena<<2) & 1) && y < 97)
				pheromoneEsq += fields[nest][x][y+3];
			if(((dnas[nest].antena<<2) & 1) && y>=3)
				pheromoneDir += fields[nest][x][y-3];
			if(((dnas[nest].antena<<3) & 1) && x <99 && y <99)
				pheromoneEsq += fields[nest][x+1][y+1];
			if(((dnas[nest].antena<<3) & 1) && x < 99 && y >=1)
				pheromoneDir += fields[nest][x+1][y-1];
			if(((dnas[nest].antena<<4) & 1) && x <99 && y < 98)
				pheromoneEsq += fields[nest][x+1][y+2];
			if(((dnas[nest].antena<<4) & 1) && x < 99 && y >=2)
				pheromoneDir += fields[nest][x+1][y-2];
			if(((dnas[nest].antena<<5) & 1) && x <99 && y < 97)
				pheromoneEsq += fields[nest][x+1][y+3];
			if(((dnas[nest].antena<<5) & 1) && x < 99 && y >=3)
				pheromoneDir += fields[nest][x+1][y-3];
			if(((dnas[nest].antena<<6) & 1) && x <98 && y < 99)
				pheromoneEsq += fields[nest][x+2][y+1];
			if(((dnas[nest].antena<<6) & 1) && x < 98 && y >=1)
				pheromoneDir += fields[nest][x+2][y-1];
			if(((dnas[nest].antena<<7) & 1) && x <98 && y < 98)
				pheromoneEsq += fields[nest][x+2][y+2];
			if(((dnas[nest].antena<<7) & 1) && x < 98 && y >=2)
				pheromoneDir += fields[nest][x+2][y-2];

			if(x < 99){
				if(pheromoneEsq > pheromoneDir)
					directions[nest][ant] = 2;
				else
					directions[nest][ant] = 0;
				nests[nest][ant][0] = x + 1;
			}else{
				directions[nest][ant] = 3;
			}
		break;
		case 2:
			if((dnas[nest].antena & 1) && x < 99)
				pheromoneEsq += fields[nest][x+1][y];
			if((dnas[nest].antena & 1) && x >=1)
				pheromoneDir += fields[nest][x-1][y];
			if(((dnas[nest].antena<<1) & 1) && x < 98)
				pheromoneEsq += fields[nest][x+2][y];
			if(((dnas[nest].antena<<1) & 1) && x >=2)
				pheromoneDir += fields[nest][x-2][y];
			if(((dnas[nest].antena<<2) & 1) && x <97)
				pheromoneEsq += fields[nest][x+3][y];
			if(((dnas[nest].antena<<2) & 1) && x >=3)
				pheromoneDir += fields[nest][x-3][y];
			if(((dnas[nest].antena<<3) & 1) && x <99 && y >=1)
				pheromoneEsq += fields[nest][x+1][y-1];
			if(((dnas[nest].antena<<3) & 1) && x >=1 && y >=1)
				pheromoneDir += fields[nest][x-1][y-1];
			if(((dnas[nest].antena<<4) & 1) && x <98 && y>=1)
				pheromoneEsq += fields[nest][x+2][y-1];
			if(((dnas[nest].antena<<4) & 1) && x >=2 && y >=1)
				pheromoneDir += fields[nest][x-2][y-1];
			if(((dnas[nest].antena<<5) & 1) && x <97 && y >=1)
				pheromoneEsq += fields[nest][x+3][y-1];
			if(((dnas[nest].antena<<5) & 1) && x >=3 && y >=1)
				pheromoneDir += fields[nest][x-3][y-1];
			if(((dnas[nest].antena<<6) & 1) && x <99 && y >=2)
				pheromoneEsq += fields[nest][x+1][y-2];
			if(((dnas[nest].antena<<6) & 1) && x >=1 && y >=2)
				pheromoneDir += fields[nest][x-1][y-2];
			if(((dnas[nest].antena<<7) & 1) && x <98 && y >=2)
				pheromoneEsq += fields[nest][x+2][y-2];
			if(((dnas[nest].antena<<7) & 1) && x >=2 && y >=2)
				pheromoneDir += fields[nest][x-2][y-2];

			if(y > 0){
				if(pheromoneEsq > pheromoneDir)
					directions[nest][ant] = 3;
				else
					directions[nest][ant] = 1;
				nests[nest][ant][1] = y - 1;
			}else{
				directions[nest][ant] = 0;
			}
		break;
		case 3:
			if((dnas[nest].antena & 1) && y >=1)
				pheromoneEsq += fields[nest][x][y-1];
			if((dnas[nest].antena & 1) && y <99)
				pheromoneDir += fields[nest][x][y+1];
			if(((dnas[nest].antena<<1) & 1) && y >=2)
				pheromoneEsq += fields[nest][x][y-2];
			if(((dnas[nest].antena<<1) & 1) && y <98)
				pheromoneDir += fields[nest][x][y+2];
			if(((dnas[nest].antena<<2) & 1) && y >=3)
				pheromoneEsq += fields[nest][x][y-3];
			if(((dnas[nest].antena<<2) & 1) && y<97)
				pheromoneDir += fields[nest][x][y+3];
			if(((dnas[nest].antena<<3) & 1) && x >=1 && y >=1)
				pheromoneEsq += fields[nest][x-1][y-1];
			if(((dnas[nest].antena<<3) & 1) && x >=1 && y <99)
				pheromoneDir += fields[nest][x-1][y+1];
			if(((dnas[nest].antena<<4) & 1) && x >=1 && y >=2)
				pheromoneEsq += fields[nest][x-1][y-2];
			if(((dnas[nest].antena<<4) & 1) && x >=1 && y < 98)
				pheromoneDir += fields[nest][x-1][y+2];
			if(((dnas[nest].antena<<5) & 1) && x >=1 && y >=3)
				pheromoneEsq += fields[nest][x-1][y-3];
			if(((dnas[nest].antena<<5) & 1) && x >=1 && y <97)
				pheromoneDir += fields[nest][x-1][y+3];
			if(((dnas[nest].antena<<6) & 1) && x >=2 && y >=1)
				pheromoneEsq += fields[nest][x-2][y-1];
			if(((dnas[nest].antena<<6) & 1) && x >=2 && y < 99)
				pheromoneDir += fields[nest][x-2][y+1];
			if(((dnas[nest].antena<<7) & 1) && x >=2 && y >=2)
				pheromoneEsq += fields[nest][x-2][y-2];
			if(((dnas[nest].antena<<7) & 1) && x >=2 && y <98)
				pheromoneDir += fields[nest][x-2][y+2];

			if(x > 0){
				if(pheromoneEsq > pheromoneDir)
					directions[nest][ant] = 0;
				else
					directions[nest][ant] = 2;
				nests[nest][ant][0] = x - 1;
			}else{
				directions[nest][ant] = 1;
			}
		break;
	}
}

void *evaluate(void *arg){
	int fitness = 0;
	int id = ((int*)arg)[0];
	char not_reach = 1;
	for(int i = 0; i< 50; i++){
		directions[id][i] = rand() % 4;
		nests[id][i][0] = start[0];
		nests[id][i][1] = start[1];
	}
	for(int i =0; i < 100; i++)
		for(int j= 0; j < 100; j++)
			fields[id][i][j] = 0;
	while(not_reach){
		for(int i=0; i< 50; i++){
			walk(id,i);
			if(nests[id][i][0] == end[0] && nests[id][i][1] == end[1])
				not_reach = 0;
		}
		fitness++;
		for(int i = 0; i < 100; i++)
			for(int j = 0; j < 100; j++)
				fields[id][i][j] = (fields[id][i][j] - 1 ) % 255;
	}
	points[id][generation % 5] = fitness;
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
				if(((dnas[best].antena >> j) & 1) != ((dnas[i].antena >> j) & 1)){
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
	int args[12] ={0,1,2,3,4,5,6,7,8,9,10,11};

  printf("Commands:\ng-Activate/Deactivate Interface\np-Pause/Unpause\nq-Save And Exit\n");
	srand(time(NULL));
	pthread_create(&ui,NULL,UI,argv);

  initPop();
	printOutput(file);
	while(TRUE){
    for(int i = 0; i < 12; i++)
      pthread_create(&evaluation[i],NULL,evaluate,&args[i]);
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

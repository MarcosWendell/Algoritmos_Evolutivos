#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<pthread.h>
#include<errno.h>
#include<unistd.h>
#include<limits.h>
#include"ui.c"

#define MAX_WITHOUT_CHANGE 5
#define MAX_TO_INVERT 20
#define MUTATION 0.1
#define VELOCITY 10000

struct dna{
	unsigned char antena;
	unsigned char pheromone;
	unsigned char max;
	unsigned char ignoreChance;
	unsigned char turnChance;
};

#define TRUE 1

struct dna dnas[12];
int generationsWithoutChange = 0, invert = 0;
float mutation = MUTATION;
char canReproduct[12];

struct dna theBestDna;
int theBest = INT_MAX;

void initPop(int id){
		dnas[id].antena = rand() % 256;
		dnas[id].pheromone = rand() % 100;
		dnas[id].max = rand() % 255;
		dnas[id].ignoreChance = rand() % 100;
		dnas[id].turnChance = rand() % 100;
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
				if(pheromoneEsq > pheromoneDir && rand() % 100 > dnas[nest].ignoreChance)
						directions[nest][ant] = 1;
				else if(pheromoneEsq < pheromoneDir && rand() % 100 > dnas[nest].ignoreChance)
						directions[nest][ant] = 3;
				nests[nest][ant][1] = y + 1;
			}else{
				if(rand() % 100 > dnas[nest].turnChance){
						if(rand() % 2)
							directions[nest][ant] = 1;
						else
							directions[nest][ant] = 3;
				}else if(rand() % 100 > dnas[nest].ignoreChance){
					directions[nest][ant] = rand() % 4;
				}else
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

				directions[nest][ant] = 2;
			if(x < 99){
				if(pheromoneEsq > pheromoneDir && rand() % 100 > dnas[nest].ignoreChance)
					directions[nest][ant] = 2;
				else if(pheromoneEsq < pheromoneDir && rand() % 100 > dnas[nest].ignoreChance)
					directions[nest][ant] = 0;
				nests[nest][ant][0] = x + 1;
			}else{
				if(rand() % 100 > dnas[nest].turnChance){
					if(rand() % 2)
						directions[nest][ant] = 2;
					else
						directions[nest][ant] = 0;
				}else if(rand() % 100 > dnas[nest].ignoreChance){
					directions[nest][ant] = rand() % 4;
				}else
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
				if(pheromoneEsq > pheromoneDir && rand() % 100 > dnas[nest].ignoreChance)
					directions[nest][ant] = 3;
				else if(pheromoneEsq < pheromoneDir && rand() % 100 > dnas[nest].ignoreChance)
					directions[nest][ant] = 1;
				nests[nest][ant][1] = y - 1;
			}else{
				if(rand() % 100 > dnas[nest].turnChance){
					if(rand() % 2)
						directions[nest][ant] = 3;
					else
						directions[nest][ant] = 1;
				}else if(rand() % 100 > dnas[nest].ignoreChance){
					directions[nest][ant] = rand() % 4;
				}else
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
				if(pheromoneEsq > pheromoneDir && rand() % 100 > dnas[nest].ignoreChance)
					directions[nest][ant] = 0;
				else if(pheromoneEsq < pheromoneDir && rand() % 100 > dnas[nest].ignoreChance)
					directions[nest][ant] = 2;
				nests[nest][ant][0] = x - 1;
			}else{
				if(rand() % 100 > dnas[nest].turnChance){
					if(rand() % 2)
						directions[nest][ant] = 0;
					else
						directions[nest][ant] = 2;
				}else if(rand() % 100 > dnas[nest].ignoreChance){
					directions[nest][ant] = rand() % 4;
				}else
					directions[nest][ant] = 1;
			}
		break;
	}
}

void *evaluate(void *arg){
	int fitness = 0;
	int id = ((int*)arg)[0];
	pthread_mutex_lock(&terminatedAcess);
	terminated[id] = 0;
	pthread_mutex_unlock(&terminatedAcess);
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
		if(tf)
			usleep(VELOCITY);
		fitness++;
		for(int i = 0; i < 100; i++)
			for(int j = 0; j < 100; j++){
				unsigned char aux = fields[id][i][j] - 1;
				fields[id][i][j] = aux % 255;
			}
		if(fitness == 1000)
			break;
	}
	points[id][generation % MEMORY] = fitness;
	pthread_mutex_lock(&terminatedAcess);
	terminated[id] = 1;
	pthread_mutex_unlock(&terminatedAcess);
	pthread_exit(0);
}

void selection(){
	int bestFit = 0;
	for(int i = 0; i < MEMORY; i++)
		bestFit += points[best][i];
	for(int i = 0; i < 12; i++){
		if(i != best){
			int fit = 0;
			for(int j = 0; j < MEMORY; j++)
				fit += points[i][j];
			if(fit < bestFit){
				best = i;
				bestFit = fit;
				generationsWithoutChange = 0;
			}
			canReproduct[i] = 1;
			if(fit == MEMORY*1000){
				initPop(i);
				canReproduct[i] = 0;
			}
			if(points[i][generation % MEMORY] < theBest){
				theBest = points[i][generation % MEMORY];
				theBestDna = dnas[i];
			}
		}
	}
}

void reproduct(){
	for(int i = 0; i < 12; i++){
		if(i != best && canReproduct[i] && points[best][generation % MEMORY] > points[i][generation % MEMORY]){
			dnas[i].pheromone += dnas[best].pheromone;
			dnas[i].pheromone /=2;
			dnas[i].max += dnas[best].max;
			dnas[i].max /=2;
			dnas[i].ignoreChance += dnas[best].ignoreChance;
			dnas[i].ignoreChance /=2;
			dnas[i].turnChance += dnas[best].turnChance;
			dnas[i].turnChance /=2;
			for(int j = 0; j < 8; j++){
				if(((dnas[best].antena >> j) & 1) != ((dnas[i].antena >> j) & 1)){
					dnas[i].antena ^= (rand() % 2)<<j;
				}
			}
			int aux = rand() % 12;
			if(aux == 8){
				dnas[i].pheromone += (-(rand() % 2))*dnas[i].pheromone*mutation;
			}else if(aux == 9){
				dnas[i].max += (-(rand() % 2))*dnas[i].max*mutation;
			}else if(aux == 10){
				dnas[i].ignoreChance += (-(rand() % 2))*dnas[i].ignoreChance*mutation;
			}else if(aux == 11){
				dnas[i].turnChance += (-(rand() % 2))*dnas[i].turnChance*mutation;
			}else{
				dnas[i].antena ^= 1<<aux;
			}
		}
	}
}

void printOutput(FILE * file){
	fprintf(file,"Generation: %d\tMutation: %lf\nBest: %d\n",generation,mutation,best+1);
	printf("Generation: %d\tMutation: %lf\tGenerations: %d\nBest: %d\n",generation,mutation,generationsWithoutChange,best+1);
	for(int i = 0; i< 12; i++){
		fprintf(file,"id: %d\tpheromone: %d\t max: %d\tantena: %d\tignore: %d\tturn: %d\tfitness: %d\n",i+1,dnas[i].pheromone,dnas[i].max,dnas[i].antena,dnas[i].ignoreChance,dnas[i].turnChance,points[i][generation % MEMORY]);
		printf("id: %d\tpheromone: %d\t max: %d\tantena: %d\tignore: %d\tturn: %d\tfitness: %d\n",i+1,dnas[i].pheromone,dnas[i].max,dnas[i].antena,dnas[i].ignoreChance,dnas[i].turnChance,points[i][generation % MEMORY]);
	}
}

int main(int argc, char *argv[]){
	counter = argc;
  FILE *file = fopen("output.txt","w");
	pthread_t ui;
	pthread_mutex_init(&paused,NULL);
	pthread_mutex_init(&save_exit,NULL);
	pthread_mutex_init(&terminatedAcess,NULL);
	int args[12] ={0,1,2,3,4,5,6,7,8,9,10,11};

  printf("Commands:\ng-Activate/Deactivate Interface\np-Pause/Unpause\nq-Save And Exit\n");
	srand(time(NULL));

	pthread_create(&ui,NULL,UI,argv);
	if(argc == 1){
  	for(int i = 0; i < 12; i++)
			initPop(i);
	}else{
		FILE *input = fopen(argv[1],"r");
		struct dna aux;
		fscanf(input,"%hhu\n%hhu\n%hhu\n%hhu\n%hhu",&aux.pheromone,&aux.max,&aux.antena,&aux.ignoreChance,&aux.turnChance);
		for(int i = 0; i < 12; i++)
			dnas[i] = aux;
		fclose(input);
	}
	while(generation < 1000){
    for(int i = 0; i < 12; i++)
      pthread_create(&evaluation[i],NULL,evaluate,&args[i]);
    for(int i = 0; i < 12; i++)
      pthread_join(evaluation[i],NULL);
		selection();
    printOutput(file);
    reproduct();
		pthread_mutex_lock(&paused);
		pthread_mutex_unlock(&paused);
		int e = pthread_mutex_trylock(&save_exit);
		if(e == EBUSY){
			break;
		}
		pthread_mutex_unlock(&save_exit);
		generationsWithoutChange++;
		if(generationsWithoutChange % MAX_WITHOUT_CHANGE == 0 && (mutation == 0.01f || mutation == 0.25f)){
			invert = !invert;
			mutation = MUTATION;
		}else if(generationsWithoutChange % MAX_WITHOUT_CHANGE == 0){
			if(invert && mutation < 1){
				mutation += 0.05;
			}else if(mutation > 0.05f){
				mutation -= 0.05;
			}else{
				mutation = 0.01;
			}
		}
    generation++;
	}

	fprintf(file,"\nThe Best:\tpheromone: %d\t max: %d\tantena: %d\tignore: %d\tturn: %d\tfitness: %d\n",theBestDna.pheromone,theBestDna.max,theBestDna.antena,theBestDna.ignoreChance,theBestDna.turnChance,theBest);
	printf("\nThe Best:\tpheromone: %d\t max: %d\tantena: %d\tignore: %d\tturn: %d\tfitness: %d\n",theBestDna.pheromone,theBestDna.max,theBestDna.antena,theBestDna.ignoreChance,theBestDna.turnChance,theBest);
  fclose(file);

	return 0;
}

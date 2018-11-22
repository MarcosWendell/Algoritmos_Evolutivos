#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<pthread.h>
#include<errno.h>
#include<unistd.h>
#include<limits.h>
#include"ui.c"

#define MAX_WITHOUT_CHANGE 5
#define MAX_TO_LOCAL 20
#define MUTATION 0.25
#define VELOCITY 10000
#define RESTART_MUTATION 0.6

struct dna{
	unsigned char antena1;
	unsigned char antena2;
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
char evolveDna[5] = {1,1,1,1,1};

struct dna theBestDna;
int theBest = INT_MAX;

void initPop(int id){
	if(evolveDna[0]){
		dnas[id].antena1 = rand() % 256;
		dnas[id].antena2 = rand() % 256;
	}
	if(evolveDna[1])
		dnas[id].pheromone = rand() % 101;
	if(evolveDna[2])
		dnas[id].max = rand() % 255;
	if(evolveDna[3])
		dnas[id].ignoreChance = rand() % 101;
	if(evolveDna[4])
		dnas[id].turnChance = rand() % 101;
}

void walk(int nest, int ant){
	int pheromoneEsq = 0, pheromoneDir = 0,x =nests[nest][ant][0] ,y =nests[nest][ant][1];
	int aux = fields[nest][x][y];
	aux += dnas[nest].pheromone;
	if(aux > dnas[nest].max)
		aux = dnas[nest].max;
	fields[nest][x][y] = aux;
	if(rand() % 101 > dnas[nest].ignoreChance){
		directions[nest][ant] = rand() % 4;
	}else{
		switch(directions[nest][ant]){
			case 0:
				if((dnas[nest].antena1 & 1) && x >= 1)
					pheromoneEsq += fields[nest][x-1][y];
				if((dnas[nest].antena2 & 1) && x < 99)
					pheromoneDir += fields[nest][x+1][y];
				if(((dnas[nest].antena1<<1) & 1) && x >= 2)
					pheromoneEsq += fields[nest][x-2][y];
				if(((dnas[nest].antena2<<1) & 1) && x < 98)
					pheromoneDir += fields[nest][x+2][y];
				if(((dnas[nest].antena1<<2) & 1) && x >= 3)
					pheromoneEsq += fields[nest][x-3][y];
				if(((dnas[nest].antena2<<2) & 1) && x < 97)
					pheromoneDir += fields[nest][x+3][y];
				if(((dnas[nest].antena1<<3) & 1) && x >= 1 && y < 99)
					pheromoneEsq += fields[nest][x-1][y+1];
				if(((dnas[nest].antena2<<3) & 1) && x < 99 && y < 99)
					pheromoneDir += fields[nest][x+1][y+1];
				if(((dnas[nest].antena1<<4) & 1) && x >= 2 && y < 99)
					pheromoneEsq += fields[nest][x-2][y+1];
				if(((dnas[nest].antena2<<4) & 1) && x < 98 && y < 99)
					pheromoneDir += fields[nest][x+2][y+1];
				if(((dnas[nest].antena1<<5) & 1) && x >= 3 && y < 99)
					pheromoneEsq += fields[nest][x-3][y+1];
				if(((dnas[nest].antena2<<5) & 1) && x < 97 && y < 99)
					pheromoneDir += fields[nest][x+3][y+1];
				if(((dnas[nest].antena1<<6) & 1) && x >= 1 && y < 98)
					pheromoneEsq += fields[nest][x-1][y+2];
				if(((dnas[nest].antena2<<6) & 1) && x < 99 && y < 98)
					pheromoneDir += fields[nest][x+1][y+2];
				if(((dnas[nest].antena1<<7) & 1) && x >= 2 && y < 98)
					pheromoneEsq += fields[nest][x-2][y+2];
				if(((dnas[nest].antena2<<7) & 1) && x < 98 && y < 98)
					pheromoneDir += fields[nest][x+2][y+2];

				if(y < 99){
					if(pheromoneEsq > pheromoneDir && rand() % 101 > dnas[nest].ignoreChance)
							directions[nest][ant] = 1;
					else if(pheromoneEsq < pheromoneDir && rand() % 101 > dnas[nest].ignoreChance)
							directions[nest][ant] = 3;
					nests[nest][ant][1] = y + 1;
				}else{
					if(rand() % 101 > dnas[nest].turnChance){
							if(rand() % 2)
								directions[nest][ant] = 1;
							else
								directions[nest][ant] = 3;
					}else if(rand() % 101 > dnas[nest].ignoreChance){
						directions[nest][ant] = rand() % 4;
					}else
						directions[nest][ant] = 2;
				}
			break;
			case 1:
				if((dnas[nest].antena1 & 1) && y < 99)
					pheromoneEsq += fields[nest][x][y+1];
				if((dnas[nest].antena2 & 1) && y >=1)
					pheromoneDir += fields[nest][x][y-1];
				if(((dnas[nest].antena1<<1) & 1) && y < 98)
					pheromoneEsq += fields[nest][x][y+2];
				if(((dnas[nest].antena2<<1) & 1) && y >= 2)
					pheromoneDir += fields[nest][x][y-2];
				if(((dnas[nest].antena1<<2) & 1) && y < 97)
					pheromoneEsq += fields[nest][x][y+3];
				if(((dnas[nest].antena2<<2) & 1) && y>=3)
					pheromoneDir += fields[nest][x][y-3];
				if(((dnas[nest].antena1<<3) & 1) && x <99 && y <99)
					pheromoneEsq += fields[nest][x+1][y+1];
				if(((dnas[nest].antena2<<3) & 1) && x < 99 && y >=1)
					pheromoneDir += fields[nest][x+1][y-1];
				if(((dnas[nest].antena1<<4) & 1) && x <99 && y < 98)
					pheromoneEsq += fields[nest][x+1][y+2];
				if(((dnas[nest].antena2<<4) & 1) && x < 99 && y >=2)
					pheromoneDir += fields[nest][x+1][y-2];
				if(((dnas[nest].antena1<<5) & 1) && x <99 && y < 97)
					pheromoneEsq += fields[nest][x+1][y+3];
				if(((dnas[nest].antena2<<5) & 1) && x < 99 && y >=3)
					pheromoneDir += fields[nest][x+1][y-3];
				if(((dnas[nest].antena1<<6) & 1) && x <98 && y < 99)
					pheromoneEsq += fields[nest][x+2][y+1];
				if(((dnas[nest].antena2<<6) & 1) && x < 98 && y >=1)
					pheromoneDir += fields[nest][x+2][y-1];
				if(((dnas[nest].antena1<<7) & 1) && x <98 && y < 98)
					pheromoneEsq += fields[nest][x+2][y+2];
				if(((dnas[nest].antena2<<7) & 1) && x < 98 && y >=2)
					pheromoneDir += fields[nest][x+2][y-2];

				if(x < 99){
					if(pheromoneEsq > pheromoneDir && rand() % 101 > dnas[nest].ignoreChance)
						directions[nest][ant] = 2;
					else if(pheromoneEsq < pheromoneDir && rand() % 101 > dnas[nest].ignoreChance)
						directions[nest][ant] = 0;
					nests[nest][ant][0] = x + 1;
				}else{
					if(rand() % 101 > dnas[nest].turnChance){
						if(rand() % 2)
							directions[nest][ant] = 2;
						else
							directions[nest][ant] = 0;
					}else if(rand() % 101 > dnas[nest].ignoreChance){
						directions[nest][ant] = rand() % 4;
					}else
						directions[nest][ant] = 3;
				}
			break;
			case 2:
				if((dnas[nest].antena1 & 1) && x < 99)
					pheromoneEsq += fields[nest][x+1][y];
				if((dnas[nest].antena2 & 1) && x >=1)
					pheromoneDir += fields[nest][x-1][y];
				if(((dnas[nest].antena1<<1) & 1) && x < 98)
					pheromoneEsq += fields[nest][x+2][y];
				if(((dnas[nest].antena2<<1) & 1) && x >=2)
					pheromoneDir += fields[nest][x-2][y];
				if(((dnas[nest].antena1<<2) & 1) && x <97)
					pheromoneEsq += fields[nest][x+3][y];
				if(((dnas[nest].antena2<<2) & 1) && x >=3)
					pheromoneDir += fields[nest][x-3][y];
				if(((dnas[nest].antena1<<3) & 1) && x <99 && y >=1)
					pheromoneEsq += fields[nest][x+1][y-1];
				if(((dnas[nest].antena2<<3) & 1) && x >=1 && y >=1)
					pheromoneDir += fields[nest][x-1][y-1];
				if(((dnas[nest].antena1<<4) & 1) && x <98 && y>=1)
					pheromoneEsq += fields[nest][x+2][y-1];
				if(((dnas[nest].antena2<<4) & 1) && x >=2 && y >=1)
					pheromoneDir += fields[nest][x-2][y-1];
				if(((dnas[nest].antena1<<5) & 1) && x <97 && y >=1)
					pheromoneEsq += fields[nest][x+3][y-1];
				if(((dnas[nest].antena2<<5) & 1) && x >=3 && y >=1)
					pheromoneDir += fields[nest][x-3][y-1];
				if(((dnas[nest].antena1<<6) & 1) && x <99 && y >=2)
					pheromoneEsq += fields[nest][x+1][y-2];
				if(((dnas[nest].antena2<<6) & 1) && x >=1 && y >=2)
					pheromoneDir += fields[nest][x-1][y-2];
				if(((dnas[nest].antena1<<7) & 1) && x <98 && y >=2)
					pheromoneEsq += fields[nest][x+2][y-2];
				if(((dnas[nest].antena2<<7) & 1) && x >=2 && y >=2)
					pheromoneDir += fields[nest][x-2][y-2];

				if(y > 0){
					if(pheromoneEsq > pheromoneDir && rand() % 101 > dnas[nest].ignoreChance)
						directions[nest][ant] = 3;
					else if(pheromoneEsq < pheromoneDir && rand() % 101 > dnas[nest].ignoreChance)
						directions[nest][ant] = 1;
					nests[nest][ant][1] = y - 1;
				}else{
					if(rand() % 101 > dnas[nest].turnChance){
						if(rand() % 2)
							directions[nest][ant] = 3;
						else
							directions[nest][ant] = 1;
					}else if(rand() % 101 > dnas[nest].ignoreChance){
						directions[nest][ant] = rand() % 4;
					}else
						directions[nest][ant] = 0;
				}
			break;
			case 3:
				if((dnas[nest].antena1 & 1) && y >=1)
					pheromoneEsq += fields[nest][x][y-1];
				if((dnas[nest].antena2 & 1) && y <99)
					pheromoneDir += fields[nest][x][y+1];
				if(((dnas[nest].antena1<<1) & 1) && y >=2)
					pheromoneEsq += fields[nest][x][y-2];
				if(((dnas[nest].antena2<<1) & 1) && y <98)
					pheromoneDir += fields[nest][x][y+2];
				if(((dnas[nest].antena1<<2) & 1) && y >=3)
					pheromoneEsq += fields[nest][x][y-3];
				if(((dnas[nest].antena2<<2) & 1) && y<97)
					pheromoneDir += fields[nest][x][y+3];
				if(((dnas[nest].antena1<<3) & 1) && x >=1 && y >=1)
					pheromoneEsq += fields[nest][x-1][y-1];
				if(((dnas[nest].antena2<<3) & 1) && x >=1 && y <99)
					pheromoneDir += fields[nest][x-1][y+1];
				if(((dnas[nest].antena1<<4) & 1) && x >=1 && y >=2)
					pheromoneEsq += fields[nest][x-1][y-2];
				if(((dnas[nest].antena2<<4) & 1) && x >=1 && y < 98)
					pheromoneDir += fields[nest][x-1][y+2];
				if(((dnas[nest].antena1<<5) & 1) && x >=1 && y >=3)
					pheromoneEsq += fields[nest][x-1][y-3];
				if(((dnas[nest].antena2<<5) & 1) && x >=1 && y <97)
					pheromoneDir += fields[nest][x-1][y+3];
				if(((dnas[nest].antena1<<6) & 1) && x >=2 && y >=1)
					pheromoneEsq += fields[nest][x-2][y-1];
				if(((dnas[nest].antena2<<6) & 1) && x >=2 && y < 99)
					pheromoneDir += fields[nest][x-2][y+1];
				if(((dnas[nest].antena1<<7) & 1) && x >=2 && y >=2)
					pheromoneEsq += fields[nest][x-2][y-2];
				if(((dnas[nest].antena2<<7) & 1) && x >=2 && y <98)
					pheromoneDir += fields[nest][x-2][y+2];

				if(x > 0){
					if(pheromoneEsq > pheromoneDir && rand() % 101 > dnas[nest].ignoreChance)
						directions[nest][ant] = 0;
					else if(pheromoneEsq < pheromoneDir && rand() % 101 > dnas[nest].ignoreChance)
						directions[nest][ant] = 2;
					nests[nest][ant][0] = x - 1;
				}else{
					if(rand() % 101 > dnas[nest].turnChance){
						if(rand() % 2)
							directions[nest][ant] = 0;
						else
							directions[nest][ant] = 2;
					}else
						directions[nest][ant] = 1;
				}
			break;
		}
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
				mutation = MUTATION;
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
		int aux;
		int ok = 0;
		while(!ok){
			aux = rand() % 20;
			if((aux == 19 && evolveDna[1]) || (aux == 18 && evolveDna[2]) || (aux == 17 && evolveDna[3]) || (aux == 16 && evolveDna[4]) || (aux < 16 && evolveDna[0]))
				ok = 1;
		}
		if(i != best && canReproduct[i] && points[best][(generation-1) % MEMORY] >= points[i][(generation-1) % MEMORY]){
			if(evolveDna[1]){
				dnas[i].pheromone += dnas[best].pheromone;
				dnas[i].pheromone /=2;
				if(aux == 19){
					dnas[i].pheromone += (-(rand() % 2))*dnas[i].pheromone*mutation;
					dnas[i].pheromone %= 101;
				}
			}
			if(evolveDna[2]){
				dnas[i].max += dnas[best].max;
				dnas[i].max /=2;
				if(aux == 18){
					dnas[i].max += (-(rand() % 2))*dnas[i].max*mutation;
					dnas[i].max %= 255;
				}
			}
			if(evolveDna[3]){
				dnas[i].ignoreChance += dnas[best].ignoreChance;
				dnas[i].ignoreChance /=2;
				if(aux == 17){
					dnas[i].ignoreChance += (-(rand() % 2))*dnas[i].ignoreChance*mutation;
					dnas[i].ignoreChance %= 101;
				}
			}
			if(evolveDna[4]){
				dnas[i].turnChance += dnas[best].turnChance;
				dnas[i].turnChance /=2;
				if(aux == 16){
					dnas[i].turnChance += (-(rand() % 2))*dnas[i].turnChance*mutation;
					dnas[i].turnChance %= 101;
				}
			}
			if(evolveDna[0]){
				for(int j = 0; j < 8; j++){
					if(((dnas[best].antena1 >> j) & 1) != ((dnas[i].antena1 >> j) & 1)){
						dnas[i].antena1 ^= (rand() % 2)<<j;
					}
					if(((dnas[best].antena2 >> j) & 1) != ((dnas[i].antena2 >> j) & 1)){
						dnas[i].antena2 ^= (rand() % 2)<<j;
					}
				}
				if(aux < 8){
					dnas[i].antena1 ^= 1<<aux;
				}else if(aux < 16){
					dnas[i].antena2 ^= 1<<(aux % 8);
				}
			}
		}
	}
}

void printOutput(FILE * file){
	fprintf(file,"Generation: %d\tMutation: %lf\nBest: %d\n",generation,mutation,best+1);
	printf("Generation: %d\tMutation: %lf\tGenerations: %d\nBest: %d\n",generation,mutation,generationsWithoutChange,best+1);
	for(int i = 0; i< 12; i++){
		fprintf(file,"id: %d\tpheromone: %d\t max: %d\tantena1: %d\tantena2: %d\tignore: %d\tturn: %d\tfitness: %d\n",i+1,dnas[i].pheromone,dnas[i].max,dnas[i].antena1,dnas[i].antena2,dnas[i].ignoreChance,dnas[i].turnChance,points[i][generation % MEMORY]);
		printf("id: %d\tpheromone: %d\t max: %d\tantena1: %d\tantena2: %d\tignore: %d\tturn: %d\tfitness: %d\n",i+1,dnas[i].pheromone,dnas[i].max,dnas[i].antena1,dnas[i].antena2,dnas[i].ignoreChance,dnas[i].turnChance,points[i][generation % MEMORY]);
	}
}

int main(int argc, char *argv[]){
	counter = argc;
  FILE *file = fopen("output.txt","w");
	pthread_t ui;
	pthread_mutex_init(&paused,NULL);
	pthread_mutex_init(&save_exit,NULL);
	pthread_mutex_init(&terminatedAcess,NULL);
	int initializePop = 1;
	int args[12] ={0,1,2,3,4,5,6,7,8,9,10,11};

	if(argc > 1){
		int i = 1;
		struct dna aux;
		while(i < argc){
			char *string = argv[i++];
			if(strcmp(string,"-i") == 0){
				if(i == argc){
					i = -1;
					break;
				}
				FILE *input = fopen(argv[i++],"r");
				fscanf(input,"%hhu\n%hhu\n%hhu\n%hhu\n%hhu\n%hhu",&aux.pheromone,&aux.max,&aux.antena1,&aux.antena2,&aux.ignoreChance,&aux.turnChance);
				fclose(input);
				initializePop = 0;
			}else if(strcmp(string,"-ph") == 0){
				if(i == argc){
					i = -1;
					break;
				}
				aux.pheromone = atoi(argv[i++]) % 101;
				evolveDna[1] = 0;
			}else if(strcmp(string,"-mx") == 0){
				if(i == argc){
					i = -1;
					break;
				}
				aux.max = atoi(argv[i++]) % 255;
				evolveDna[2] = 0;
			}else if(strcmp(string,"-ig") == 0){
				if(i == argc){
					i = -1;
					break;
				}
				aux.ignoreChance = atoi(argv[i++]) % 101;
				evolveDna[3] = 0;
			}else if(strcmp(string,"-tn") == 0){
				if(i == argc){
					i = -1;
					break;
				}
				aux.turnChance = atoi(argv[i++]) % 101;
				evolveDna[4] = 0;
			}else if(strcmp(string,"-at") == 0){
				if(i == argc){
					i = -1;
					break;
				}
				aux.antena1 = atoi(argv[i++]) % 256;
				if(i == argc){
					i = -1;
					break;
				}
				aux.antena2 = atoi(argv[i++]) % 256;
				evolveDna[0] = 0;
			}else{
				i = -1;
				break;
			}
		}
		if(i == -1){
			printf("Usage: ./prog [-i inputFile][-ph pheromone][-mx maximumPheromone][-ig ignoreChance][-tn turnChance][-at antena1 antena2]\n");
			printf("The order of the flags matter.\n");
			exit(0);
		}
		for(int j = 0; j < 12; j++)
			dnas[j] = aux;
		mutation = RESTART_MUTATION;
	}
	printf("%d\n",evolveDna[1]);
	if(!evolveDna[0] && !evolveDna[1] && !evolveDna[2] && !evolveDna[3] && !evolveDna[4]){
		printf("Static Population\n");
		exit(0);
	}
	printf("Commands:\ng-Activate/Deactivate Interface\np-Pause/Unpause\nq-Save And Exit\n");
	srand(time(NULL));
	if(initializePop)
		for(int i = 0; i < 12; i++)
			initPop(i);

	pthread_create(&ui,NULL,UI,argv);
	while(generationsWithoutChange != MAX_TO_LOCAL){
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
		if(generationsWithoutChange % MAX_WITHOUT_CHANGE == 0 && mutation != 0.01f){
			if(mutation > 0.05f){
				mutation -= 0.05;
			}else{
				mutation = 0.01;
			}
		}
    generation++;
	}

	fprintf(file,"\nThe Best:\tpheromone: %d\t max: %d\tantena1: %d\tantena2: %d\tignore: %d\tturn: %d\tfitness: %d\n",theBestDna.pheromone,theBestDna.max,theBestDna.antena1,theBestDna.antena2,theBestDna.ignoreChance,theBestDna.turnChance,theBest);
	printf("\nThe Best:\tpheromone: %d\t max: %d\tantena1: %d\tantena2: %d\tignore: %d\tturn: %d\tfitness: %d\n",theBestDna.pheromone,theBestDna.max,theBestDna.antena1,theBestDna.antena2,theBestDna.ignoreChance,theBestDna.turnChance,theBest);
  fclose(file);

	return 0;
}

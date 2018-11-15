#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<pthread.h>
#include<errno.h>
#include"ui.c"

#define TRUE 1

int dna[12];
pthread_t evaluation[12];

void initPop(){

}

void *evaluate(void *arg){

}

void selection(){

}

void reproduct(){

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
    fprintf(file,"%d\n",generation);
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

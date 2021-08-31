#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <windows.h>

#define N 5
#define THINKING 0
#define HUNGRY 1
#define EATING 2
#define LEFT (i+N-1) % N
#define RIGHT (i + 1) % N

pthread_mutex_t mutex;
pthread_mutex_t s[N];
pthread_cond_t condition_var[N];
int state[N];
int fil[N] = {0, 1, 2, 3, 4};

void take_forks(int i);
void put_forks(int i);
void test(int i);
void* philosopher(void *num);

int main()
{
	int i;
	pthread_t thread_id[N];	
	pthread_mutex_init(&mutex, NULL);

	for (i = 0; i < N; i++){
        pthread_mutex_init(&s[i], NULL);
    	pthread_cond_init(&condition_var[i], NULL);
	}
			
	for (i = 0; i < N; i++) {
		pthread_create(&thread_id[i], NULL, philosopher, &fil[i]);
		state[i] = THINKING;
		printf("O filosofo %d esta pensando\n", i + 1);
	}

	for (i = 0; i < N; i++)
		pthread_join(thread_id[i], NULL);
	
	return 0;
}

// Função para pegar os garfos
void take_forks(int i)
{
	pthread_mutex_lock(&mutex); //Down
	state[i] = HUNGRY;
	printf("O filosofo %d esta com fome.\n", i + 1);

	test(i);
	while(state[i] != EATING)
   {
		pthread_cond_wait(&condition_var[i], &mutex);
	}
	pthread_mutex_unlock(&mutex); //Up
	Sleep(1);
}

// Função para soltar os garfos
void put_forks(int i)
{
	pthread_mutex_lock(&mutex); //Down
	state[i] = THINKING;
	printf("O filosofo %d soltou os garfos e esta pensando\n", i+1);

	test(LEFT);
	test(RIGHT);
	pthread_mutex_unlock(&mutex); //Up
}

void test(int i)
{
	if (state[i] == HUNGRY && state[LEFT]!=EATING && state[RIGHT]!=EATING) {
		state[i] = EATING;
		pthread_cond_signal(&condition_var[i]);
		Sleep(2);
		printf("O filosofo %d pegou os garfos e esta comendo\n", i + 1);

		pthread_mutex_unlock(&s[i]); //Up
		
		int comendo=0;
		int indices[2];
		int i;
		for (i=0; i<N; i++){
			if(state[i]==2){
				indices[comendo] = i;
				comendo++;
			} 
		}
		if (comendo>1) printf("OS FILOSOFOS %d e %d ESTAO COMENDO\n", indices[0]+1, indices[1]+1);
	}
}

void* philosopher(void *num)
{
	while (1) {
		int* i = num;
		Sleep(1);
		take_forks(*i);
		Sleep(0); //eat();
		put_forks(*i);
	}
}
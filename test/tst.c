#include <stdio.h>

#define SIZE 10 

void publish(int *i, int *j, int *agent);
void runthrough(int init, int end);

int main(int argc, char *argv[]) {
	int i=0, j=0, process=3;

	//printf("\nINI -> i:: %d, j:: %d\n",i,j);
	//for(int agent=0; agent<process; agent++) {
//		publish(&i, &j, &agent);
//	}
//	printf("\nEND -> i:: %d, j:: %d\n",i,j);

	int init = 0;
	for(int i = 0; i < 32; i ++) {
		int end = (i+1) * 500 / 32;
		runthrough(init, end);
		init = end;
	}

}

void runthrough(int init, int end) {
	for(; init < end; init++) {
		printf("runnin: %d\n", init);
	}
}

void publish(int *i, int *j, int *agent) {
	printf("i: %d, j: %d - Agent: %d\n",*i,*j,*agent);
	*i= *i +1;
	*j= *j + 1;
}

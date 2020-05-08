#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
// gcc -Wall -O2 no-sse.c -o no-sse -DN=1000 -DR=1000

void get_walltime(double *wct) {
	struct timeval tp;
	gettimeofday(&tp,NULL);
	*wct = (double)(tp.tv_sec+tp.tv_usec/1000000.0);
}	




int main() {
	float **array1, **array2;
	int r, c, i;
	double tstart, tend;

 	float k[] = {0.5, 0.5, 0.5, 0.5,5.0, 0.5, 0.5, 0.5, 0.5};
 	array1 = (float **)malloc(N*sizeof(float *));
 	if (array1==NULL) exit(1);
 	array2 = (float **)malloc(N*sizeof(float *));
 	if (array2==NULL) {free(array1); exit(1); };
 	
 	
 	for(r=0;r<N;r++){
 		array1[r] = (float *)malloc(R*sizeof(float));
 		if(array1 == NULL){
 			for(i = 0; i < r; i++){
 				free(array1[i]);
 			}
 			free (array1);
 			exit(1);
 		}
 		
 	}
 	
 	for(r=0;r<N;r++){
 		array2[r] = (float *)malloc(R*sizeof(float));
 		if(array2 == NULL){
 			for(i = 0; i < r; i++){
 				free(array2[i]);
 			}
 			free (array2);
 			exit(1);
 		}
 	}	
 	for (r=0;r<N;r++) {
 		for (c=0;c<R;c++){
 			array1[r][c] = rand()%255;
 			array2[r][c] = r*c;
 		}			
 	}
 	get_walltime(&tstart);
 	
 	for (r=1;r<N-1;r++) {
 		for (c=1;c<R-1;c++){
 		array2[r][c]= array1[r-1][c-1]*k[0] + array1[c-1][r]*k[2] + array1[r][c-1]*k[3]
 		+ array1[r][c]*k[4] + array1[r][c +1]*k[5] + array1[r+1][c-1]*k[6] +
 		array1[r+1][c]*k[7] + array1[r+1][c+1]*k[8];
 		}
 	}
 	
 	get_walltime(&tend);
 	
 	printf("secs = %f/n", (tend-tstart)*1000000);
 	for(i = 0; i < N; i++){
 		free(array1[i]);
 		free(array2[i]);
 	}
 	free (array1);
 	free (array2);
 	return 0;
 	
 		
 
 


}



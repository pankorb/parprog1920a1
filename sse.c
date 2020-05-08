#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/time.h>
#include <emmintrin.h>


// compile with: gcc -Wall -O2 sse.c -o sse -DN=1000 -DR=1000


void get_walltime(double *wct) {
  struct timeval tp;
  gettimeofday(&tp,NULL);
  *wct = (double)(tp.tv_sec+tp.tv_usec/1000000.0);
}


inline __m128 sse_dot4(__m128 v0, __m128 v1)
{
    v0 = _mm_mul_ps(v0, v1);

    v1 = _mm_shuffle_ps(v0, v0, _MM_SHUFFLE(2, 3, 0, 1));
    v0 = _mm_add_ps(v0, v1);
    v1 = _mm_shuffle_ps(v0, v0, _MM_SHUFFLE(0, 1, 2, 3));
    v0 = _mm_add_ps(v0, v1);

    return v0;
}




int main() {
  float **array1, **array2, *ki, *ka, *prod;
  int i,j,c;
  double tstart,tend;


  float k[] = {0.5, 0.5, 0.5, 0.5, 5.0, 0.5, 0.5, 0.5, 0.5};
  
  array1 = (float **)malloc(N*sizeof(float *));
  if (array1==NULL) exit(1);
  array2 = (float **)malloc(N*sizeof(float *));
  if (array2==NULL) exit(1);

  for(i=0;i<N;i++){
    array1[i] = (float *)malloc(R*sizeof(float));
    array2[i] = (float *)malloc(R*sizeof(float));
    if (array1==NULL) exit(1); 
  }


  __m128 *vfk, *vfa, *vfp, *t;
  i = posix_memalign((void **)&ki,16,8*sizeof(float));
  if (i!=0) exit(1);
  i = posix_memalign((void **)&ka,16,8*sizeof(float));
  if (i!=0) exit(1);
  i = posix_memalign((void **)&prod,16,8*sizeof(float));
  if (i!=0) exit(1);
  
  for(i=0;i<8;i++)
    ki[i] = k[i];


  
 
  for (i=0;i<N;i++) {
    for (j=0;j<R;j++){
      array1[i][j] = rand()%255;
      array2[i][j] = i*j;
    }
  }
  
 
  get_walltime(&tstart);
  

  
  for (i=1;i<N-4;i++) {
    for (j=1;j<R-4;j++) {
      vfa = (__m128 *)ka;
      vfk = (__m128 *)ki;
      vfp = (__m128 *)prod;

      *vfa = _mm_load_ps1(&(array1[i-1][j-1])); 
      t = vfa;
      t++;
      *t = _mm_load_ps1(&(array1[i][j-1]));
     

      for (c=0;c<8;c+=4) {
        *vfp = sse_dot4(*vfk, *vfa); 
        vfk++; vfa++; vfp++;
      }

      *vfp = _mm_add_ps(*vfp,*vfp);
      *vfp = _mm_shuffle_ps(*vfp,*vfp,_MM_SHUFFLE(3,1,0,1));
      *vfp = _mm_add_ps(*vfp,*vfp);
      *vfp = _mm_shuffle_ps(*vfp,*vfp,_MM_SHUFFLE(3,1,0,1));
     
      array2[i][j] = prod[0] + array1[i+1][j+1]*k[8];
    }
  }
 
 
  get_walltime(&tend);
  

  printf("secs = %f\n", (tend-tstart)*1e6); 
  
  
  for(i = 0; i < N; i++){
 		free(array1[i]);
 		free(array2[i]);
 	}
 	free (array1);
 	free (array2);
 	free(ki);
 	free(ka);
 	free(prod);
  
  return 0;
}

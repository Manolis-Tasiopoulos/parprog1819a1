#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pmmintrin.h>

void get_walltime(double *wct);

int main(int argc, char* argv[])
{

	float *array_A, *array_B, *array_C, *array_final_sum;
	float *pa, *pb, *pc, *pfs;
	__m128 *va, *vb, *sum, tmp;		
	
	float time, mflops;
	double ts = 0, te = 0;	
	
		
  if(posix_memalign((void **) &array_A, 16, N*N*sizeof(float))!= 0)
	{
		exit(1);
	}
		
	if(posix_memalign((void **) &array_B, 16, N*N*sizeof(float)) != 0)
	{
		free(array_A);
		exit(1);
	}
		
	if(posix_memalign((void **) &array_C, 16, N*N*sizeof(float)) != 0)
	{
		free(array_A);
		free(array_B);
		exit(1);
	}	
	
	if(posix_memalign((void **) &array_final_sum, 16, 4*sizeof(float)) != 0)
	{
		free(array_A);
		free(array_B);
		exit(1);
	}
	
	pa = array_A;
  pb = array_B;
  pc = array_C;	
	
	for(int i = 0; i < N*N; i++)
	{
		*pa = 2.0;
		*pb = 3.0;
		*pc = 0.0;		
		
		pa++; 
		pb++; 
		pc++;
	}
	
	pfs = array_final_sum;
	
	for(int i = 0; i < 4; i++)
	{
		*pfs = 0.0;
		pfs++;
	}

	pa = array_A;
  pb = array_B;
  pc = array_C;	
	pfs = array_final_sum;	
	
	sum = (__m128 *) array_final_sum;		
  
	
  
	get_walltime(&ts);		

	for(int k = 0; k < N; k++) 
	{
		pb = array_B;
		vb = (__m128 *) pb;
		
		for(int j = 0; j < N; j++)
		{
			pa = array_A + k*N;
			va = (__m128 *) pa;
			
			*sum = _mm_set_ps(0.0, 0.0, 0.0, 0.0);
			
			for(int i = 0; i < N; i += 4)
			{
				*sum = _mm_add_ps(*sum, _mm_mul_ps(*va, *vb));
				va++;
				vb++;
			}
			
			tmp = _mm_hadd_ps(*sum, *sum);
			*sum = _mm_hadd_ps(tmp, tmp);
			
			*pc = *pfs;			
			pc++;
		}
	}
	
	get_walltime(&te);
  
	
  
	time = te - ts;
	mflops = (N*N*N) / (time * 1e6);
	
	printf("\ntime taken to run O(n^3): %f\n", time);
	printf("mflops: %f\n", mflops);
	
	check_val = 2.0 * 3.0 * N;
	pc = array_C;
	
	printf("\nChecking Array_C values...\n");
	
	for(int i = 0; i < N*N; i++) 
	{
		if(*pc != 2.0 * 3.0 * N)
		{
			printf("! Wrong value on Array[%d] = %f", i ,*pc);
			exit(1);
		}
		pc++;
	}
	
	printf("Array_C values are correct !!!\n");
	
	free(array_A);
	free(array_B);
	free(array_C); 
	
	return 0;
}


void get_walltime(double *wct) 
{
	struct timeval tp;
	gettimeofday(&tp, NULL);
	*wct = (double)(tp.tv_sec + tp.tv_usec / 1000000.0);
}

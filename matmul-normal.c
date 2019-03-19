#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

void get_walltime(double *wct);

int main(int argc, char* argv[])
{

	float *array_A, *array_B, *array_C;
	float *pa, *pb, *pc;
	
	float time, mflops;
	double ts, te;
	
	float check_val;
	
   	
	array_A = (float*)malloc(N*N*sizeof(float));
	if(array_A == NULL)
	{
		exit(1);
	}
	
	array_B = (float*)malloc(N*N*sizeof(float));
	if(array_B == NULL)
	{
		free(array_A);
		exit(1);
	}
	
	array_C = (float*)malloc(N*N*sizeof(float));
	if(array_C == NULL)
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

	pa = array_A;
        pb = array_B;
        pc = array_C;

	get_walltime(&ts);		
	
	for(int k = 0; k < N; k++) 
	{
		pb = array_B;
		for(int j = 0; j < N; j++)
		{
			pa = array_A + k*N;
			for(int i = 0; i < N; i++)
			{
				*pc += (*pa) * (*pb);
				pa++; 
				pb++;
			}
			pc++;
		}
	}
	
	get_walltime(&te);
	
	time = te - ts;
	mflops = (N*N*N) / (time * 1e6);
	
	printf("\ntime taken to run O(n^3): %f\n", time);
	printf("mflops: %f\n", mflops);
	
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

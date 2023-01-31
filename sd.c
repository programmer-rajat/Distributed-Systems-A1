#include <stdio.h>
#include <math.h>
#include <stdlib.h>

typedef struct complex{
	double real;
	double imag;
} complex;

double calculateMod(complex Point){
	double r=Point.real;
	double i=Point.imag;
	double mod=sqrt((r*r)+(i*i));
	return mod;
}

int checkMandelbrotSet(int K,complex Point){
	complex Z;
	Z.real=0;
	Z.imag=0;
	int count=0;
	
	
	while(calculateMod(Z)<=2 && count < K)
	{
		double rZ=Z.real;
		double iZ=Z.imag;
		double rC=Point.real;
		double iC=Point.imag;
		double rtemp=rZ*rZ - iZ*iZ + rC;
		double itemp=2*rZ*iZ + iC;
		Z.real=rtemp;
		Z.imag=itemp;
		count++;
	}
	if(count==K) return 1;
	else return 0;
	
}
int main(){

	int N,M,K;
	scanf("%d",&N);
	scanf("%d",&M);
	scanf("%d",&K);

	
	
		for (int i = 0; i < M; i++) 
    {
        for (int j = 0; j < N; j++) 
        {
            double x = -1.5 + (double)j * (2.5 / (double)(N-1));
            double y = 1.0 - (double)i * (2.0 / (double)(M-1));
            complex c;
            c.real = x;
            c.imag = y;
            printf("%d ", checkMandelbrotSet(K,c));
        }
        printf("\n");
    }		
				
				
		
		


		complex testp;
		testp.real=-0.000000;
		testp.imag=1.000000;
		if(checkMandelbrotSet(K,testp)==1){
					printf("1 ");
				}
				else
					printf("0 ");
					printf("\n");
		
		
	return 0;
	
}

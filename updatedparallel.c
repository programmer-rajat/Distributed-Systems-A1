#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <complex.h>
#include <mpi.h>

double calculateMod(double complex Point){
	double r=creal(Point);
	double i=cimag(Point);
	double mod=sqrt((r*r)+(i*i));
	return mod;
}

int checkMandelbrotSet(int K,double complex Point){
	double complex Z=CMPLX(0,0);
	
	int count=0;
	
	
	while(calculateMod(Z)<=2 && count < K)
	{
		double rZ=creal(Z);
		double iZ=cimag(Z);
		double rC=creal(Point);
		double iC=cimag(Point);
		double rtemp=rZ*rZ - iZ*iZ + rC;
		double itemp=2*rZ*iZ + iC;
		Z=CMPLX(rtemp,itemp);
		//Z.imag=itemp;
		count++;
	}
	if(count==K) return 1;
	else return 0;
	
}


int main(int argc,char* argv[]){
    MPI_Init(&argc, &argv);
    int N,M,K;
    double complex *inputarr;
    int finalsize;
    int my_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    int num_process;
    MPI_Comm_size(MPI_COMM_WORLD, &num_process);

    if(my_rank==0){
        
	    scanf("%d",&N);
	    scanf("%d",&M);
	    scanf("%d",&K);
        int prod=N*M;
        int rem=prod%num_process;
        if(rem==0)
            finalsize=N*M;
        else{
            int dumm_num=num_process-rem;
            finalsize=N*M+dumm_num;
            //inputarr=(double complex*)malloc()
        }
        inputarr=(double complex*)malloc(finalsize*sizeof(double complex));
        int count=0;
        for(int i=0;i<M;i++){
                for (int j = 0; j < N; j++) {
                double x = -1.5 + (double)j * (2.5 / (double)(N-1));
                double y = 1.0 - (double)i * (2.0 / (double)(M-1));
                double complex c=CMPLX(x,y);
                inputarr[count++]=c;

            }
        }
        
        if(rem!=0){
        int dumm_num=num_process-rem;
        for(int t=0;t<dumm_num;t++){
            double complex c=CMPLX(-1.6,1.1);
            inputarr[count++]=c;
        }
        }


    }
    MPI_Bcast(&K, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&M, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&finalsize, 1, MPI_INT, 0, MPI_COMM_WORLD);


    //int prod=N*M;
    //int rem=prod%num_process;
    int num_to_send=finalsize/num_process;
    //if(rem==0)
      //  num_to_send=(finalsize)/num_process;
    //else{
        //int dumm_num=num_process-rem;
      //  num_to_send=

    //}

    /*if(my_rank==0)
    {

        inputarr=(double complex*)malloc(N*M*sizeof(double complex));
        int count=0;
        for(int i=0;i<M;i++){
                for (int j = 0; j < N; j++) {
                double x = -1.5 + (double)j * (2.5 / (double)(N-1));
                double y = 1.0 - (double)i * (2.0 / (double)(M-1));
                double complex c=CMPLX(x,y);
                inputarr[count++]=c;

            }
        }
        
    }*/
    
    /*
    MPI_Bcast(&K, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&M, 1, MPI_INT, 0, MPI_COMM_WORLD);*/
    
    double complex *inputsubarr=(double complex*)malloc(num_to_send*sizeof(double complex));

    MPI_Scatter(inputarr,num_to_send,MPI_C_DOUBLE_COMPLEX, inputsubarr,num_to_send , MPI_C_DOUBLE_COMPLEX, 0, MPI_COMM_WORLD);
    int *outputsubarr=(int*)malloc(num_to_send * sizeof(int));;

    for(int i=0;i<num_to_send;i++){
        outputsubarr[i]=checkMandelbrotSet(K,inputsubarr[i]);
    }

    int *outputarr=(int*)malloc(finalsize * sizeof(int));
    MPI_Gather(outputsubarr, num_to_send, MPI_INT, outputarr, num_to_send, MPI_INT,0, MPI_COMM_WORLD);

    int my_ranke;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_ranke);

    if(my_ranke==0){
        int cnt=0;
        for(int p=0;p<M*N;p++){
                if(cnt==N){
                    printf("\n");
                    cnt=0;
                }
                printf("%d ",outputarr[p]);
                cnt++;
            }
        
    }





MPI_Finalize();
 
return 0;
}
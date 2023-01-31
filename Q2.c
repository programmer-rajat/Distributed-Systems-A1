#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>


typedef struct pair{
	int key;
	int freq;
} pair;

void merge(pair input[],int start,int end){

    pair* output=(pair*)malloc((end-start+1)*sizeof(pair));
    int mid=(start+end)/2;
    int i=start;
    int j=mid+1;
    int k=0;
    while(i<=mid && j<=end){
        if(input[i].key<input[j].key){
            output[k++]=input[i++];
        }
        else
            output[k++]=input[j++];
    }
    
    while(i<=mid)
        output[k++]=input[i++];
    
    while(j<=end)
        output[k++]=input[j++];
     
     int q=0;
     for(int i=start;i<start+(end-start+1);i++){
        input[i]=output[q++];
    
     } 

}

void mergeSort(pair input[],int start,int end){
    if(start>=end)
        return;

    int mid=(start+end)/2;
    mergeSort(input,start,mid);
    mergeSort(input,mid+1,end);
    merge(input,start,end);
}




int main(int argc,char* argv[]){

    MPI_Init(&argc, &argv);

    int N;
    scanf("%d",&N);
    pair* keywords=(pair*)malloc(N*sizeof(pair));
    for(int i=0;i<N;i++){
        int u,v;
        scanf("%d",&u);
        scanf("%d",&v);
        pair ele;
        ele.key=u;
        ele.freq=v;
        keywords[i]=ele;

    }

    printf("\nBefore sorting\n");

    for(int i=0;i<N;i++){
        pair ele=keywords[i];
        printf("%d ",ele.key);
        printf("%d ",ele.freq);
        printf("\n");
    }


    mergeSort(keywords,0,N-1);
    printf("\nAfter sorting\n");


     for(int i=0;i<N;i++){
        pair ele=keywords[i];
        printf("%d ",ele.key);
        printf("%d ",ele.freq);
        printf("\n");
    }




    MPI_Finalize();
    return EXIT_SUCCESS;

}
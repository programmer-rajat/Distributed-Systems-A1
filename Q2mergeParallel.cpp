#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <bits/stdc++.h>

using namespace std;

/*
typedef struct pairq{
	int key;
	int freq;
} pairq;
*/

class pairq{
    public:
	    int key;
	    int freq;
};



void merge(pairq input[],int start,int end){

    pairq* output=(pairq*)malloc((end-start+1)*sizeof(pairq));
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

void mergeSort(pairq input[],int start,int end){
    if(start>=end)
        return;

    int mid=(start+end)/2;
    mergeSort(input,start,mid);
    mergeSort(input,mid+1,end);
    merge(input,start,end);
}


void merge2Arrays(pairq* receivedarr,pairq* &myarr,int size1,int size2){
        // for(int w=0;w<size1;w++){
        //     cout<<receivedarr[w].key<<" "<<receivedarr[w].freq<<endl;
        // }


        vector<pairq> output;
        int p1=0;
        int p2=0;

        while(p1<size1 && p2<size2){
            if(myarr[p2].key<receivedarr[p1].key){
                output.push_back(myarr[p2]);
                p2++;
            }

            else{
                output.push_back(receivedarr[p1]);
                p1++;
            }
        }

        while(p1<size1){
            output.push_back(receivedarr[p1]);
            p1++;
        }

        while(p2<size2){
            output.push_back(myarr[p2]);
            p2++;
        }

        // for(int k=0;k<output.size();k++){
        //     cout<<output[k].key<<" "<<output[k].freq<<endl;
        // }
        myarr=new pairq[size2+size1];
        for(int k=0;k<output.size();k++){
            myarr[k]=output[k];
        }
}
/*
class forPQ{
    public:
        pairq element;
        int arrindex;
        int index;

    
}; 

struct compareKeys{
bool operator()(const forPQ& p1, const forPQ& p2)
{
 
    pairq p1_ele=p1.element;
    pairq p2_ele=p2.element;
    return p1_ele.key > p2_ele.key;
}
};


vector<pairq> mergeKArrays(vector<vector<pairq>> arr){
    vector<pairq> output;
    priority_queue<forPQ,vector<forPQ>,compareKeys> pq;
    for(int i=0;i<arr.size();i++){
        forPQ fpq{arr[i][0],i,0};
        pq.push(fpq);
        
    }

    while (!pq.empty()) {
        forPQ curr = pq.top();
        pq.pop();
 
        // i ==> Array Number
        // j ==> Index in the array number
        int i = curr.arrindex;
        int j = curr.index;
 
        output.push_back(curr.element);
 
        // The next element belongs to same array as
        // current.
        if (j + 1 < arr[i].size()){
            forPQ fpq{arr[i][j+1],i,j+1};
            pq.push(fpq);
            //pq.push({ arr[i][j + 1], { i, j + 1 } });
        }
    }
 
    return output;
}

*/

int main(int argc,char* argv[]){

    MPI_Init(&argc, &argv);

    int my_rank;
    int num_proc;

    int N;
    int finalsize;
    pairq* keywords;
    int* freqarray;
    int* keyarray;

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_proc);

    if(my_rank==0){

    
    scanf("%d",&N);
    int rem=N%num_proc;
        if(rem==0)
            finalsize=N;
        else{
            int dumm_num=num_proc-rem;
            finalsize=N+dumm_num;
            //inputarr=(double complex*)malloc()
        }
    keywords=(pairq*)malloc(finalsize*sizeof(pairq));
    freqarray=new int[finalsize];
    keyarray=new int[finalsize];
    for(int i=0;i<N;i++){
        int u,v;
        scanf("%d",&u);
        scanf("%d",&v);
        pairq ele;
        ele.key=u;
        keyarray[i]=u;
        ele.freq=v;
        freqarray[i]=v;
        keywords[i]=ele;

    }

    for(int i=N;i<finalsize;i++){
        pairq ele;
        ele.key=INT_MAX;
        keyarray[i]=INT_MAX;
        ele.freq=0;
        freqarray[i]=0;
        keywords[i]=ele;
    }
    
    printf("\nBefore sorting\n");
    /*
    for(int i=0;i<N;i++){
        pairq ele=keywords[i];
        printf("%d ",ele.key);
        printf("%d ",ele.freq);
        printf("\n");
    }

    */

   for(int i=0;i<finalsize;i++){
        pairq ele=keywords[i];

        printf("%d ",ele.key);
        printf("%d ",ele.freq);
        printf("\n");
/*
        printf("%d ",keyarray[i]);
        printf("%d ",freqarray[i]);
        printf("\n");

        */
    }

    

    }

    MPI_Bcast(&finalsize, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int num_to_send=finalsize/num_proc;
    

    //pairq *inputsubarr=(pairq*)malloc(num_to_send*sizeof(pairq));
    int* keysubarr=new int[num_to_send];
    int* freqsubarr=new int[num_to_send];
    
   MPI_Scatter(keyarray,num_to_send,MPI_INT, keysubarr,num_to_send , MPI_INT, 0, MPI_COMM_WORLD);
   MPI_Scatter(freqarray,num_to_send,MPI_INT, freqsubarr,num_to_send , MPI_INT, 0, MPI_COMM_WORLD);
   
   pairq* mysubarray;
   mysubarray=new pairq[num_to_send];
   //vector<pairq> mysubarray(num_to_send);
   for(int i=0;i<num_to_send;i++){
    pairq item;
    item.key=keysubarr[i];
    item.freq=freqsubarr[i];
    mysubarray[i]=item;
   }

    cout<<endl;
    cout<<"Rank =="<<my_rank<<"++"<<endl;
    for(int i=0;i<num_to_send;i++){
        cout<<mysubarray[i].key<<" "<<mysubarray[i].freq<<endl;
    }
    
    mergeSort(mysubarray,0,num_to_send-1);

    cout<<"Rank =="<<my_rank<<"++"<<endl;
    for(int i=0;i<num_to_send;i++){
        cout<<mysubarray[i].key<<" "<<mysubarray[i].freq<<endl;
    }

    

    int* outputkeysubarr=new int[num_to_send];
    int* outputfreqsubarr=new int[num_to_send];
    for(int i=0;i<num_to_send;i++){
        outputkeysubarr[i]=mysubarray[i].key;
        outputfreqsubarr[i]=mysubarray[i].freq;
    }
    
    int my_ranke;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_ranke);
    if(my_ranke!=0){

    MPI_Send(outputkeysubarr, num_to_send, MPI_INT, 0, 0, MPI_COMM_WORLD);
    MPI_Send(outputfreqsubarr, num_to_send, MPI_INT, 0, 1, MPI_COMM_WORLD);

    }

    if(my_ranke==0){

        vector<pairq>finalsorted;
        for(int i=1;i<num_proc;i++){
            int* prockeyarr=new int[num_to_send];
            int* procfreqarr=new int[num_to_send];
            MPI_Recv(prockeyarr, num_to_send, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(procfreqarr, num_to_send, MPI_INT, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            pairq* recvprocarr=new pairq[num_to_send];
            // cout<<"Process se"<<i<<endl;
            for(int j=0;j<num_to_send;j++){
            pairq item;
            item.key=prockeyarr[j];
            //cout<<prockeyarr[j]<<" "<<procfreqarr[j]<<endl;
            item.freq=procfreqarr[j];
            recvprocarr[j]=item;
            }
            
            merge2Arrays(recvprocarr,mysubarray,num_to_send,num_to_send*i);
            /*
            for(int t=0;t<N;t++){
        pairq ele=mysubarray[t];
        printf("%d ",ele.key);
        printf("%d ",ele.freq);
        printf("\n");

        }
        */
        }
        printf("\nAfter sorting\n");


        for(int i=0;i<N;i++){
        pairq ele=mysubarray[i];
        printf("%d ",ele.key);
        printf("%d ",ele.freq);
        printf("\n");

        
    }




    }

    //Breakpoint


    
/*
    int* outputkeysubarr=new int[num_to_send];
    int* outputfreqsubarr=new int[num_to_send];
    for(int i=0;i<num_to_send;i++){
        outputkeysubarr[i]=mysubarray[i].key;
        outputfreqsubarr[i]=mysubarray[i].freq;
    }
    int *outputkeyarr=(int*)malloc(finalsize * sizeof(int));
    int *outputfreqarr=(int*)malloc(finalsize * sizeof(int));
    MPI_Gather(outputkeysubarr, num_to_send, MPI_INT, outputkeyarr, num_to_send, MPI_INT,0, MPI_COMM_WORLD);
    MPI_Gather(outputfreqsubarr, num_to_send, MPI_INT, outputfreqarr, num_to_send, MPI_INT,0, MPI_COMM_WORLD);

    int my_ranke;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_ranke);
    vector<pairq> finalsorted;
    
    
    if(my_ranke==0){
    //finalsorted=new pairq[finalsize];
    vector<vector<pairq>> outputarr(num_to_send);
    int index=0;
    for(int i=0;i<finalsize;i++){
        
            pairq element;
            element.key=outputkeyarr[i];
            element.freq=outputfreqarr[i];
            outputarr[index].push_back(element);

            if(i!=0 && i%num_to_send==0)
                index++;
        
        
    }

    finalsorted=mergeKArrays(outputarr);

    printf("\nAfter sorting\n");

    for(int i=0;i<N;i++){
        pairq ele=finalsorted[i];
        printf("%d ",ele.key);
        printf("%d ",ele.freq);
        printf("\n");
    }




    




    }
*/
    /*
    printf("\nAfter sorting\n");


     for(int i=0;i<N;i++){
        pairq ele=keywords[i];
        printf("%d ",ele.key);
        printf("%d ",ele.freq);
        printf("\n");
    }

    */




    MPI_Finalize();
    return EXIT_SUCCESS;

}

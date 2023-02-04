#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <bits/stdc++.h>
#include <vector>



using namespace std;

vector<vector<int>> costmat;
vector<vector<int>> rootmat;
//vector<int> parents;



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


void fillParents(int l,int r,int parent,vector<int> &parents,vector<vector<int>> finalrootmat){
    if(l>r)
        return;

    // if(l==r){
    //     parents[l]=parent;
    //     return;
    // }

    parents[finalrootmat[l-1][r]]=parent;


    //parents[finalrootmat[l][r]]=parent;
    //cout<<parent<<endl;
    //cout<<parents[rootmat[l][r]]<<endl;
    fillParents(l,finalrootmat[l-1][r]-1,finalrootmat[l-1][r],parents,finalrootmat);
    fillParents(finalrootmat[l-1][r]+1,r,finalrootmat[l-1][r],parents,finalrootmat);

}

int main(int argc,char* argv[]){

    MPI_Init(&argc, &argv);

    int my_rank;
    int num_proc;

    int N;
    //int *costmat;
    
    int finalsize;
    pairq* keywords;
    int* freqarray;
    int* keyarray;

    int *sortedkey;
    int *sortedfreq;



     

    

    
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
    
  //  printf("\nBefore sorting\n");
   

//    for(int i=0;i<finalsize;i++){
//         pairq ele=keywords[i];

//         // printf("%d ",ele.key);
//         // printf("%d ",ele.freq);
//         // printf("\n");

//     }

    

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

    //cout<<endl;
  //  cout<<"Rank =="<<my_rank<<"++"<<endl;
    // for(int i=0;i<num_to_send;i++){
    //     cout<<mysubarray[i].key<<" "<<mysubarray[i].freq<<endl;
    // }
    
    mergeSort(mysubarray,0,num_to_send-1);

   // cout<<"Rank =="<<my_rank<<"++"<<endl;
    // for(int i=0;i<num_to_send;i++){
    //     cout<<mysubarray[i].key<<" "<<mysubarray[i].freq<<endl;
    // }

    

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
    //sortedkey=new int[N];
      //  sortedfreq=new int[N];
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
         
        }
        //printf("\nAfter sorting\n");

        sortedkey=new int[N];
        sortedfreq=new int[N];
        for(int i=0;i<N;i++){
        pairq ele=mysubarray[i];
        
        sortedkey[i]=ele.key;
        sortedfreq[i]=ele.freq;
        // printf("%d ",ele.key);
        // printf("%d ",ele.freq);
        // printf("\n");

        
    }

    for(int r=1;r<num_proc;r++){
     MPI_Send(sortedkey,N, MPI_INT, r, 2, MPI_COMM_WORLD);
     MPI_Send(sortedfreq,N, MPI_INT, r, 3, MPI_COMM_WORLD);
    }
    
    // for(int l=0;l<=N;l++){
    //     int row=l;
    //     int col=l;
    //     int width=N+1;
    //     costmat[row*width+col]=sortedfreq[l-1];

    // }
    
    // //breakpoint
    // for(int l=1;l<N;l++){
    //     int row=l;
    //     int col=l+1;
    //     int width=N+1;
    //     costmat[row*width+col]=sortedfreq[l-1]+sortedfreq[l]+min(sortedfreq[l-1],sortedfreq[l]);


    // }




    }

    //MPI_Bcast(costmat,300000, MPI_INT, 0, MPI_COMM_WORLD);









    



    


     if(my_ranke!=0){
       sortedkey=new int[N];
       sortedfreq=new int[N];
       MPI_Recv(sortedkey, N, MPI_INT, 0, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
       MPI_Recv(sortedfreq, N, MPI_INT, 0, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    //    cout<<"After everything"<<endl;
    //     for(int i=0;i<N;i++){
    //          cout<<sortedkey[i]<<" "<<sortedfreq[i]<<endl;
    //      }
    //     cout<<endl;
    //     for(int l=1;l<=N;l++){
    //     int row=l;
    //     int col=l;
    //     int width=N+1;
    //     cout<<costmat[row*width+col]<<" ";

    // }

    // cout<<endl;
    
    // //breakpoint
    // for(int l=1;l<N;l++){
    //     int row=l;
    //     int col=l+1;
    //     int width=N+1;
    //     cout<<costmat[row*width+col]<<" ";//=sortedfreq[l-1]+sortedfreq[l]+min(sortedfreq[l-1],sortedfreq[l]);


    // }
    // cout<<endl;
     }

    costmat.resize(N+1,vector<int>(N+1,0));
    rootmat.resize(N+1,vector<int>(N+1,0));
    //parents.resize(N+1,0);

    for(int u=0;u<N;u++){
        costmat[u][u+1]=sortedfreq[u];
        rootmat[u][u+1]=sortedkey[u];
    }
    

    for(int i=1;i<N;i++){
        int num_cells_per_process=ceil((double)(N-i)/num_proc);
        int cnt=0;
        int *costs=new int[num_cells_per_process];
        int *local_roots=new int[num_cells_per_process];


        for(int j=my_ranke*num_cells_per_process;j<(my_ranke+1)*num_cells_per_process;j++){
            if(j+i+1>N){
                break;
            }
            else{
                int sum_freq=0;
                int to_add=INT_MAX;
                for(int k=j;k<(j+i+1);k++){
                    sum_freq+=sortedfreq[k];
                    int temp_cost=costmat[j][k]+costmat[k+1][j+i+1];
                    if(to_add<=temp_cost){

                    }

                    else{
                        to_add=temp_cost;
                        local_roots[cnt]=sortedkey[k];
                    }
                    //to_add=min(to_add,costmat[j][k]+costmat[k+1][j+i+1]);
                }

                costs[cnt]=sum_freq+to_add;
                cnt++;

            }
        }

    int complete_diag[N-i];
    int complete_root[N-i];

    int iter1=0;
    int iter2=0;

    if(my_ranke==0){
        for(int q=0;q<cnt;q++){
            complete_diag[iter1++]=costs[q];
            complete_root[iter2++]=local_roots[q];
        }

        

        

        for(int r=1;r<num_proc;r++){
            int cnt;
            MPI_Recv(&cnt, 1, MPI_INT, r, 11, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            int temp_diag[cnt];
            MPI_Recv(temp_diag, cnt, MPI_INT, r, 12, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            for(int w=0;w<cnt;w++)
                complete_diag[iter1++]=temp_diag[w];

            MPI_Recv(temp_diag, cnt, MPI_INT, r, 14, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            for(int w=0;w<cnt;w++)
                complete_root[iter2++]=temp_diag[w];

            
            


        
        
        }

        for(int p=1;p<num_proc;p++){
            MPI_Send(complete_diag,N-i, MPI_INT,p, 13, MPI_COMM_WORLD);
            MPI_Send(complete_root,N-i, MPI_INT,p, 15, MPI_COMM_WORLD);

        }


    }

    else{
        MPI_Send(&cnt,1, MPI_INT,0, 11, MPI_COMM_WORLD);
        MPI_Send(costs,cnt, MPI_INT, 0, 12, MPI_COMM_WORLD);
        MPI_Send(local_roots,cnt, MPI_INT, 0, 14, MPI_COMM_WORLD);
        MPI_Recv(complete_diag,N-i, MPI_INT, 0, 13, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(complete_root,N-i, MPI_INT, 0, 15, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    }

    for(int h=0;h<N-i;h++){
        costmat[h][h+i+1]=complete_diag[h];
        rootmat[h][h+i+1]=complete_root[h];
    }

    }


    

    if(my_ranke==0){
        // cout<<endl;
        // cout<<"Cost matrix"<<endl;
        // for(int y=0;y<=N;y++){
        //     for(int z=0;z<=N;z++){
        //         cout<<costmat[y][z]<<" ";
                
        //     }
            
            
        //     cout<<endl;
        // }
        // cout<<endl;
        // cout<<"Root matrix"<<endl;
        // for(int y=0;y<=N;y++){
        //     for(int z=0;z<=N;z++){
        //         cout<<rootmat[y][z]<<" ";
                
        //     }
        //     cout<<endl;
        // }

        // vector<vector<int>> finalrootmat(N,vector<int>(N,0));
        // for(int y=0;y<N;y++){
        //     for(int z=1;z<=N;z++){
        //         finalrootmat[y][z-1]=rootmat[y][z];
                
        //     }
        // }

        // for(int y=0;y<N;y++){
        //     for(int z=0;z<N;z++){
        //         cout<<finalrootmat[y][z]<<" ";
                
        //     }
        //     cout<<endl;
        // }
            cout<<costmat[0][N]<<endl;
         vector<int> parents(N+1,0);
        // //cout<<"B1"<<endl;
         fillParents(1,N,-1,parents,rootmat);
        // // cout<<"B2"<<endl;
          for(int i=1;i<=N;i++){
            if(parents[i]==-1)
                cout<<0<<" ";
            else
                cout<<parents[i]<<" ";
          }


           
    }




    MPI_Finalize();
    return EXIT_SUCCESS;

}
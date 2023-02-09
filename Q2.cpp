#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <bits/stdc++.h>


//MPI_Recv((void*)recieved_vec.data(), recv_size*sizeof(Points), MPI_BYTE, rank+1,2,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
using namespace std;

unordered_map<int,unordered_map<int,vector<int>>> map2;

class Pym{
    public:
        int PID;
        int x;
        int y;
        char dir;

    Pym(){

    }

    Pym(int ID,int i,int j,char d){
        this->PID=ID;
        this->x=i;
        this->y=j;
        this->dir=d;

    }
    
    void Move(int N,int M){
        map2[this->x][this->y].erase(remove(map2[this->x][this->y].begin(),map2[this->x][this->y].end(),this->PID),map2[this->x][this->y].end());
        
        if(this->dir=='U'){
            if(this->y==M-1){
                this->y=M-2;
                this->dir='D';
            }

            else{
                this->y=(this->y)+1;
            }

        }

        else if(this->dir=='D'){
            if(this->y==0){
                this->y=1;
                this->dir='U';
            }
            
            else{

                this->y=(this->y)-1;
            }

        }

        else if(this->dir=='L'){
            if(this->x==0){
                this->x=1;
                this->dir='R';

            }

            else{
                this->x=(this->x)-1;
            }
        }

        else if(this->dir=='R'){
            if(this->x==N-1){
                this->x=N-2;
                this->dir='L';

            }

            else{
                this->x=(this->x)+1;
            }
        }
        map2[this->x][this->y].push_back(this->PID);
       // map1[this->PID]=this;
    }



};

unordered_map<int,Pym> map1;
void detectCollision(){
    for(auto &iter1:map2){
        for(auto &iter2:iter1.second){
            if((iter2.second.size())==2){
                int id1=iter2.second[0];
                int id2=iter2.second[1];
                char dir1=map1[id1].dir;
                char dir2=map1[id2].dir;
                if(dir1=='U' && dir2=='D'){
                    map1[id1].dir='R';
                    map1[id2].dir='L';
                }

                else if(dir1=='D' && dir2=='U'){
                    map1[id1].dir='L';
                    map1[id2].dir='R';
                }

                else if(dir1=='L' && dir2=='R'){
                    map1[id1].dir='U';
                    map1[id2].dir='D';
                }

                else if(dir1=='R' && dir2=='L'){
                    map1[id1].dir='D';
                    map1[id2].dir='U';
                }

            }
        }
    }

}


int boundarycrossed(int rank,int N,int M,int num_process,int per_process,int x,int y){
    int ans; //ans =0 no cross, ans=1 niche aa rha, ans=2 upar jaa rha
    //No of rows more than no of cols, division across y axis

    if(M>N){
        if(rank==0){
                int bs=0;
                int be=per_process;
                if(y>be){
                    ans=2;
                }
                else{
                    ans=0;
                }
        }

        else if(rank==num_process-1){
            int bs=rank*per_process;
            int be=bs+M%num_process;
            if(y<bs){
                ans=1;
            }
            else
                ans=0;
        }

        else{
            int bs=rank*per_process;
            int be=(rank+1)*per_process - 1;

            if(y<bs){
                ans=1;
            }
            else if(y>be){
                ans=2;
            }
            else
                ans=0;
        }
        
    }

    else{
        if(rank==0){
                int bs=0;
                int be=per_process;
                if(x>be){
                    ans=2;
                }
                else{
                    ans=0;
                }
        }

        else if(rank==num_process-1){
            int bs=rank*per_process;
            int be=bs+N%num_process;
            if(x<bs){
                ans=1;
            }
            else
                ans=0;
        }

        else{
            int bs=rank*per_process;
            int be=(rank+1)*per_process - 1;

            if(x<bs){
                ans=1;
            }
            else if(x>be){
                ans=2;
            }
            else
                ans=0;
        }

        
    }

    return ans;
}

int main(int argc,char* argv[]){
    MPI_Init(&argc, &argv);
    int N,M,K,T;
    int my_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    int num_process;
    MPI_Comm_size(MPI_COMM_WORLD, &num_process);
    
    int per_process;



    if(my_rank==0){
        cin>>N;
        cin>>M;
        cin>>K;
        cin>>T;
        //vector<int> particleIds;
        
        if(M>N){
            per_process = M/num_process;

        }
        else{
            per_process = N/num_process;
        }
        //cout<<"reached here";
        //breakpoint
        vector<Pym> scatterobj;
        vector<vector<Pym>> globalvec(num_process);
        for(int i=0;i<K;i++){
            int x,y;
            char c;
            cin>>x>>y>>c;
            //cout<<"reached here";
            //Pym pymobj(i,x,y,c);
            //map1[i]=pymobj;
            Pym pymobj(i,x,y,c);
            if(M>=N){
                
                globalvec[y/per_process].push_back(pymobj);
            }

            else{
                globalvec[x/per_process].push_back(pymobj);
            }

            

           // cout<<map1[i].PID<<" "<<map1[i].x<<" "<<map1[i].y<<" "<<map1[i].dir;
            //cout<<endl;
            //map2[x][y].push_back(i);

        }
        //cout<<"reached here";
        
            for(int i=0;i<globalvec.size();i++){
                vector<Pym> procvec=globalvec[i];
                for(int j=0;j<procvec.size();j++){
                    scatterobj.push_back(procvec[j]);
                    
                }
                
            }

            // for(int i=0;i<scatterobj.size();i++)
            //     cout<<scatterobj[i].PID<<" "<<scatterobj[i].x<<" "<<scatterobj[i].y<<" "<<scatterobj[i].dir<<endl;
            
            //int scatterobj_size=scatterobj.size();
            int *counts=new int[num_process];
            int index=0;
            //cout<<globalvec.size();
            for(int i=0;i<globalvec.size();i++){
                counts[index++]=globalvec[i].size() * sizeof(Pym);
                
            }

            index=0;
            int *displacements=new int[num_process];
            for(int i=0;i<num_process;i++){
                if(i==0)
                    displacements[i]=0;
                else{
                    displacements[i]=counts[i-1]+displacements[i-1];
                }
            }
            
            for(int p=1;p<num_process;p++){
                 int cnt=counts[p];

                 MPI_Send(&cnt, 1, MPI_INT, p, p, MPI_COMM_WORLD);
            }
            //breakpoint
            //cout<<"reached here";
            
            vector<Pym> mypoints(counts[0]/sizeof(Pym));
            MPI_Scatterv((void *)scatterobj.data(), counts, displacements, MPI_BYTE, (void *)mypoints.data(), counts[my_rank], MPI_BYTE, 0, MPI_COMM_WORLD);
          //  cout<<"Rank = "<<my_rank<<endl;
            //cout<<"Before move"<<endl;
             for(int i=0;i<mypoints.size();i++){
            // cout<<mypoints[i].PID<<" "<<mypoints[i].x<<" "<<mypoints[i].y<<" "<<mypoints[i].dir<<endl;
            Pym ymobj(mypoints[i].PID,mypoints[i].x,mypoints[i].y,mypoints[i].dir);
            map1[mypoints[i].PID]=ymobj;
            map2[mypoints[i].x][mypoints[i].y].push_back(mypoints[i].PID);



            

     }

    }

    

    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&M, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&K, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&T, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&per_process, 1, MPI_INT, 0, MPI_COMM_WORLD);



    if(my_rank!=0){
        int cnt;
        MPI_Recv(&cnt, 1, MPI_INT, 0, my_rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        vector<Pym> mypoints(cnt/sizeof(Pym));
        MPI_Scatterv(NULL, NULL, NULL, MPI_BYTE, (void *)mypoints.data(), cnt, MPI_BYTE, 0, MPI_COMM_WORLD);
        // cout<<"+++++++++++++"<<endl;
        // cout<<my_rank<<endl;
        // cout<<cnt<<endl;
        // cout<<"++++++++++"<<endl;
        //if(my_rank==1)
        
        //cout<<"Rank = "<<my_rank<<endl;
       // cout<<"Before move"<<endl;
        for(int i=0;i<mypoints.size();i++){
            //cout<<mypoints[i].PID<<" "<<mypoints[i].x<<" "<<mypoints[i].y<<" "<<mypoints[i].dir<<endl;
            Pym pymobj(mypoints[i].PID,mypoints[i].x,mypoints[i].y,mypoints[i].dir);
            map1[mypoints[i].PID]=pymobj;
            map2[mypoints[i].x][mypoints[i].y].push_back(mypoints[i].PID);
        }


    }

//array scattered till here
//fill map1
//fill map2
    for(int t=0;t<T;t++){
        detectCollision();

        

        for(auto particles:map1){
            int id=particles.first;
            Pym part=particles.second;
            part.Move(N,M);
            map1[id]=part;


        }
        
        //cout<<"Rank = "<<my_rank<<endl;
        //cout<<"After move"<<endl;
        
        //cout<<map1.size()<<endl;
        // for(auto particles:map1){
        //     int id=particles.first;
        //    // cout<<id<<endl;
        //     Pym part=particles.second;
        //     cout<<part.PID<<" "<<part.x<<" "<<part.y<<" "<<part.dir<<endl;



        // }
        // sleep(1);
        vector<int> to_delete;
        vector<Pym> forward_send_buff;
        vector<Pym> backward_send_buff;
        for(auto particles:map1){
            int xcor=particles.second.x;
            int ycor=particles.second.y;
            if(boundarycrossed(my_rank,N,M,num_process,per_process,xcor,ycor)==1){
                to_delete.push_back(particles.first);
                backward_send_buff.push_back(particles.second);

                //send_buff1 and send_buff2 for mid processes since upar niche dono ko bhejna
            }

            else if(boundarycrossed(my_rank,N,M,num_process,per_process,xcor,ycor)==2){
                to_delete.push_back(particles.first);
                forward_send_buff.push_back(particles.second);
            }
            else{

            }

        }
        // if(my_rank==1){
        //     cout<<"Forward buffer"<<endl;
        //     for(int k=0;k<forward_send_buff.size();k++)
        //         cout<<"--->"<<forward_send_buff[k].PID<<" "<<forward_send_buff[k].x<<" "<<forward_send_buff[k].y<<" "<<forward_send_buff[k].dir<<endl;
        
        //     cout<<"Backward buffer"<<endl;
        // for(int k=0;k<backward_send_buff.size();k++)
        //         cout<<"<---"<<backward_send_buff[k].PID<<" "<<backward_send_buff[k].x<<" "<<backward_send_buff[k].y<<" "<<backward_send_buff[k].dir<<endl;
        
        // }
        vector<Pym> receivedbuff;
        if(my_rank==0){
            int cnt=forward_send_buff.size();
            //send cnt
            MPI_Send(&cnt, 1, MPI_INT, my_rank+1, 21, MPI_COMM_WORLD);
            vector<Pym> sendarr;
            for(int i=0;i<forward_send_buff.size();i++){
                sendarr.push_back(forward_send_buff[i]);
            }
            //send array

            MPI_Send((void *)sendarr.data(), cnt*sizeof(Pym), MPI_BYTE, my_rank+1, 22, MPI_COMM_WORLD);
            
            //MPI_Send(my_rank+1);

            //recv cnt
            int recv_cnt;
            MPI_Recv(&recv_cnt, 1, MPI_INT, my_rank+1, 21, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            //recv array
            vector<Pym> recv_arr(recv_cnt);
            MPI_Recv((void *)recv_arr.data(),recv_cnt*sizeof(Pym), MPI_BYTE, my_rank+1, 22, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            
            //MPI_Recv(my_rank+1);
            for(int i=0;i<recv_cnt;i++){
                receivedbuff.push_back(recv_arr[i]);
            }
            
        }

        else if(my_rank==num_process-1){
            int recv_cnt;
            MPI_Recv(&recv_cnt, 1, MPI_INT, my_rank-1, 21, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            //int recv_arr[recv_cnt];
            vector<Pym> recv_arr(recv_cnt);
            MPI_Recv((void *)recv_arr.data(),recv_cnt*sizeof(Pym), MPI_BYTE, my_rank-1, 22, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            //MPI_Recv(my_rank-1);

            int cnt=backward_send_buff.size();
            //send cnt
            MPI_Send(&cnt, 1, MPI_INT, my_rank-1, 21, MPI_COMM_WORLD);
            vector<Pym> sendarr;
            //int* sendarr=new int[cnt];
            for(int i=0;i<backward_send_buff.size();i++){
                sendarr.push_back(backward_send_buff[i]);
            }
            MPI_Send((void *)sendarr.data(), cnt*sizeof(Pym), MPI_BYTE, my_rank-1, 22, MPI_COMM_WORLD);
            //MPI_Send(my_rank+1);

            for(int i=0;i<recv_cnt;i++){
                receivedbuff.push_back(recv_arr[i]);
            }
        }

        else{
            //MPI_Recv(my_rank-1);
            int recv1_cnt;
            MPI_Recv(&recv1_cnt, 1, MPI_INT, my_rank-1, 21, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            vector<Pym> recv1_arr(recv1_cnt);
            MPI_Recv((void*)recv1_arr.data(),recv1_cnt*sizeof(Pym), MPI_BYTE, my_rank-1, 22, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            
            //MPI_Send(my_rank+1);
            //use send_buff2
            int cnt2=forward_send_buff.size();
            MPI_Send(&cnt2, 1, MPI_INT, my_rank+1, 21, MPI_COMM_WORLD);
            vector<Pym> sendarr2;
            for(int i=0;i<forward_send_buff.size();i++){
                sendarr2.push_back(forward_send_buff[i]);
            }

            MPI_Send((void*)sendarr2.data(), cnt2*sizeof(Pym), MPI_BYTE, my_rank+1, 22, MPI_COMM_WORLD);



            //MPI_Recv(my_rank+1);
            int recv2_cnt;
            MPI_Recv(&recv2_cnt, 1, MPI_INT, my_rank+1, 21, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            vector<Pym> recv2_arr(recv2_cnt);
            MPI_Recv((void*)recv2_arr.data(),recv2_cnt*sizeof(Pym), MPI_BYTE, my_rank+1, 22, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            //MPI_Send(my_rank-1);
            //use send_buff1

            int cnt1=backward_send_buff.size();
            MPI_Send(&cnt1, 1, MPI_INT, my_rank-1, 21, MPI_COMM_WORLD);
            vector<Pym> sendarr1;
            for(int i=0;i<backward_send_buff.size();i++){
                sendarr1.push_back(backward_send_buff[i]);
            }

            MPI_Send((void*)sendarr1.data(), cnt1*sizeof(Pym), MPI_BYTE, my_rank-1, 22, MPI_COMM_WORLD);

            for(int i=0;i<recv1_cnt;i++){
                receivedbuff.push_back(recv1_arr[i]);
            }

            for(int i=0;recv2_cnt;i++){
                receivedbuff.push_back(recv2_arr[i]);
            }
        }

        //delete to_delete ids from map1,map2
        for(int i=0;i<to_delete.size();i++){
            Pym obj=map1[i];
            map1.erase(to_delete[i]);
            map2[obj.x][obj.y].erase(remove(map2[obj.x][obj.y].begin(),map2[obj.x][obj.y].end(),obj.PID),map2[obj.x][obj.y].end());



            
        }

        

        

        //insert received values into map1,map2
        for(int i=0;i<receivedbuff.size();i++){
            Pym pymobj=receivedbuff[i];
            int id=pymobj.PID;
            map1[id]=pymobj;
            map2[pymobj.x][pymobj.y].push_back(pymobj.PID);




        }
        // for(auto i:map1){
        //     cout<<i.second.PID<<" "<<i.second.x<<" "<<i.second.y<<" "<<i.second.dir<<endl;
        // }
        // cout<<endl;

    }
   

    MPI_Finalize();
    return EXIT_SUCCESS;
 
    //return 0;
}

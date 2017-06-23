#include<msgpack.hpp>
#include<msgpack/fbuffer.hpp>
#include<unistd.h>
#include<stdlib.h>
#include<vector>
#include<iostream>
#include<fstream>
#include<string.h>
#include<utility>
using namespace std;

vector <int>gameMap;

int rowNum,colNum;
int mapSize;

int cycleFlag;
int tempMaxPath;


int currentNodeType;

int FindCycle(int position,int t){

    /*
    if(!visitMap[position]&& gameMap[position]==currentNodeType){
        cycleFlag=1;
        return 0;
    }
    */
    //visitMap[position]=0;
    if(cycleFlag) return 0;
    gameMap[position]=currentNodeType*(-1);
    int a[4]={0,0,0,0};
    //left 3
    if(position%colNum!=0 && t!=1){
        if(gameMap[position-1]==currentNodeType)
            a[0]=FindCycle(position-1,3);
        else if(gameMap[position-1]==currentNodeType*(-1)){
            cycleFlag=1;
            return 0;
        }


    }
    //up 4
    if(position-colNum>=0 && t!=2){
        if(gameMap[position-colNum]==currentNodeType )
            a[1]=FindCycle(position-colNum,4);
        else if(gameMap[position-colNum]==currentNodeType*(-1)){
            cycleFlag=1;
            return 0;
        }

    }

    //down 2
    if(position+colNum<mapSize  && t!=4){
        if(gameMap[position+colNum]==currentNodeType )
            a[2]=FindCycle(position+colNum,2);
        else if(gameMap[position+colNum]==currentNodeType*(-1)){
            cycleFlag=1;
            return 0;
        }

    }

    //right 1
    if(position%colNum!=colNum-1 && t!=3){
        if(gameMap[position+1]==currentNodeType)
            a[3]=FindCycle(position+1,1);
        else if(gameMap[position+1]==currentNodeType*(-1)){
            cycleFlag=1;
            return 0;
        }

    }

    for(int i=3;i>0;i--){
        if(a[i]>a[i-1])
            swap(a[i],a[i-1]);
    }

    if(!a[0]) return 1;
    else{
        for(int i=3;i>1;i--){
            if(a[i]>a[i-1])
                swap(a[i],a[i-1]);
            }
        if(a[0]+a[1]>tempMaxPath) tempMaxPath=a[0]+a[1];
        return a[0]+1;
    }



}
string itoa(int a)
{

    string ss="";   //create empty string
    if(a==0){
        ss="0";
        return ss;
    }
    while(a)
    {
        int x=a%10;
        a/=10;
        char i='0';
        i=i+x;
        ss=i+ss;      //append new character at the front of the string!
    }
    return ss;
}


int main()
{
	fstream fin,fout;
	fin.open("input.txt",ios::in|ios::binary);
	fout.open("output.txt",ios::out);
	msgpack::object_handle oh;

    int bufsize;

	fin.seekg(0, ios::end);
    bufsize = fin.tellg();
    fin.seekg(0);
    char *buf = NULL;

    buf = new char[bufsize];

    fin.read(buf, bufsize);

    size_t off=0;
    int task=0;


    oh = msgpack::unpack(buf,bufsize,off);
    oh.get().convert(task);

    msgpack::sbuffer sbuf;
    while(task--){
        vector <string>ans;



        oh = msgpack::unpack(buf,bufsize,off);
        oh.get().convert(rowNum);

        oh = msgpack::unpack(buf,bufsize,off);
        oh.get().convert(colNum);
        mapSize=rowNum*colNum;

        int nodeNum;
        oh = msgpack::unpack(buf,bufsize,off);
        oh.get().convert(nodeNum);


        oh = msgpack::unpack(buf,bufsize,off);
        oh.get().convert(gameMap);

        vector<int>maxPath(nodeNum,0);


        ////////map
        for(int j=0;j<mapSize;j++){


            if(gameMap[j]<0 || maxPath[gameMap[j]-1]==-1) continue;
            currentNodeType=gameMap[j];
            tempMaxPath=0;
            cycleFlag=0;
            FindCycle(j,0);
            if(cycleFlag){
                maxPath[currentNodeType-1]=-1;
            }
            else{
                maxPath[currentNodeType-1]=max(maxPath[currentNodeType-1],tempMaxPath);
            }
        }

        for(int j=0;j<nodeNum;j++){
            if(maxPath[j]==-1){
                //cout<<"cycle"<<endl;
                ans.push_back("cycle");
            }
            else{
                //cout<<maxPath[j]<<endl;
                ans.push_back(itoa(maxPath[j]));
            }
        }



        msgpack::pack(sbuf,ans);
        ans.clear();
        gameMap.clear();
        maxPath.clear();

    }

        fout.write(sbuf.data(),sbuf.size());
        fout.close();
        delete []buf;
        return 0;
}

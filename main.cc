#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <queue>
#include <sstream>
#include <bits/stdc++.h>
using namespace std;

struct process
{
  char pname[50];
  int arrivaltime;
  int servicetime;
  int finishtime;
  int turnaroundtime;
  float normturn;
  int complete;		//SPN-HHRN
  int remainingtime;	//RR- SRT -FD-1 -FD-2^i
  int index;		
  int runningtime;	//RR-FD-1 -FD-2^i
  int priority;		//FD-1 -FD-2^i
  int pariority;	//Aging
};
//compare function for priority queue (SRT)
struct compare{
	bool operator()(process const& p1,process const& p2){
	
	if(p1.remainingtime==p2.remainingtime)
	return  p1.arrivaltime > p2.arrivaltime;
	else
	return p1.remainingtime>p2.remainingtime;
	} 

};
//compare function for priority queue (FD-1 /FD-2i)
struct fdcompare{
	bool operator()(process const& p1,process const& p2){
	
	if(p1.priority==p2.priority)
	return  p1.arrivaltime > p2.arrivaltime;
	else
	return p1.priority>p2.priority;
	} 

};
//compare function for priority queue (Aging)
struct compare2{                                     // Used in aging algorithim
	bool operator()(process const& p1,process const& p2){   
	
	if(p1.pariority==p2.pariority) {       //if both pariorities are equal then return process which arrived first
      if(p1.runningtime==90){
		return p1.arrivaltime>p2.runningtime+1;}
		if(p2.runningtime==90){
		return p2.arrivaltime<p1.runningtime+1;}
	return  p1.runningtime > p2.runningtime;}
	else
	return p1.pariority<p2.pariority;      //else return the highest pariority
	} 

};
queue<struct process> temp1;
queue<struct process> temp2;
//function to print number of duration given when type input is trace
void noduration(int x,int y){
	int i,j;
	for(i=0;i<=y;){
   	if(i==y){
   		cout<<"0 "<<endl; i++;
   	}else{
	cout<<"0 1 2 3 4 5 6 7 8 9 ";
	i=i+10;}   	
   	}
   	cout <<"------------------------------------------------"<<endl;
}
//function to print output when type input is stats
void outputb(queue<struct process> temp2,int x,float totalturnaround,float totalnormturn){
	cout<<"Process    ";
	while(!temp2.empty()){
   		printf("|%3s  ",temp2.front().pname);
   		temp1.push(temp2.front());
   		temp2.pop();
   	}
   	cout<<"|"<<endl;
   	cout<<"Arrival    ";
   	while(!temp1.empty()){
   		printf("|%3d  ",temp1.front().arrivaltime);
   		temp2.push(temp1.front());
   		temp1.pop();
   	}
   	cout<<"|"<<endl;
   	cout<<"Service    ";
   	while(!temp2.empty()){
   		printf("|%3d  ",temp2.front().servicetime);
   		temp1.push(temp2.front());
   		temp2.pop();
   	}
   	cout<<"| Mean|"<<endl;
   	cout<<"Finish     ";
   	while(!temp1.empty()){
   		printf("|%3d  ",temp1.front().finishtime);
   		temp2.push(temp1.front());
   		temp1.pop();
   	}
   	cout<<"|-----|"<<endl;
   	cout<<"Turnaround ";
   	while(!temp2.empty()){
   		printf("|%3d  ",temp2.front().turnaroundtime);
   		temp1.push(temp2.front());
   		temp2.pop();
   	}
   	printf("|%5.2f|\n",totalturnaround/float(x));
   	cout<<"NormTurn   ";
   	while(!temp1.empty()){
   		printf("|%5.2f",temp1.front().normturn);
   		temp2.push(temp1.front());
   		temp1.pop();
   	}
   	printf("|%5.2f|\n",totalnormturn/float(x));
   	cout<< endl;
}
int main() {
   string type,algorithm,duration,nop;
   std::getline(cin,type);  		//read firstline ->trace or stats
   std::getline(cin,algorithm);		//read secondline -> no of algorihtm
   int n = algorithm.length();
   char char_array[n + 1];
   char algo[200][200];
   int i=0,j=0,noelement=0;
   char* token = strtok(strcpy(char_array,algorithm.c_str()), ",");
   while (token != NULL) {
    	strcpy(algo[i],token);
        noelement++;
        i++;
        token = strtok(NULL, ",");
   }
    std::getline(cin,duration);		//read thirdline ->duration
    std::getline(cin,nop);		//read fourthline ->number of process
    queue<struct process> qq;
    string temp;
    for (int i=0; i<stoi(nop);i++){	//loop for every process to read its input
      std::getline(cin,temp);		
      int n = temp.length();
      char char_array[n + 1];
      char* token = strtok(strcpy(char_array,temp.c_str()), ",");
      j=0;
      process tempp;
      tempp.index=i;
      while (token != NULL) {
        if(j==0){
           strcpy(tempp.pname,token);
           j++;
        }else if(j==1){
           tempp.arrivaltime=stoi(token);
           j++;
        }
        else{
          tempp.servicetime=stoi(token);
          tempp.remainingtime=stoi(token);
          tempp.pariority=stoi(token);
        }
        token = strtok(NULL, ",");
      }
      qq.push(tempp);
    }
    int m=0,q;
    char res[stoi(nop)][stoi(duration)];	//array to save result
  for(m=0;m<noelement;m++) { //loop according to number of algorithm input
    for(i=0;i<stoi(nop);i++){ //initalize array with spaces
    	for(j=0;j<stoi(duration);j++)
    		res[i][j]=' ';
    }
    int time=0,p=0;
    float totalturnaround=0,totalnormturn=0;
    queue<struct process> temp1;
    queue<struct process> temp2;
    queue<struct process> done;
    queue<struct process> temp4;
    while(!qq.empty()){			// copy of original queue input to another queue.
    		temp1.push(qq.front()); 
    		temp2.push(qq.front());
    		done.push(qq.front());
    		qq.pop();
    }
    while(!temp2.empty()){	//return original queue as it is 
    	temp4.push(temp2.front());
    	qq.push(temp2.front());  
    	temp2.pop();
    }
    if(algo[m][0]=='1'){  // FCFS algorithm
    	//loop on each interval of time till end of duration
   	while(time<stoi(duration) && p<stoi(nop)){
   		//if next process is arrived then it will exexute 
   		if(temp1.front().arrivaltime<=time){
   				//print '.' at time when process was in ready queue
   			 	for(i=temp1.front().arrivaltime;i<time;i++){
   			 		res[p][i]='.';
   			 	}
				//calculate finishtime of process
   			 	temp1.front().finishtime=time+temp1.front().servicetime;
   			 	//print '*' at time of execution 
   			 	for(i=time;i<temp1.front().finishtime;i++){
   			 		res[p][i]='*';
   			 	}
   			 	//assign time with finishtime of executed process
   			        time=temp1.front().finishtime; 
   			        //calculate turnaround,normturn
   			        temp1.front().turnaroundtime=temp1.front().finishtime-temp1.front().arrivaltime;
   	        		temp1.front().normturn=float(temp1.front().turnaroundtime)/float(temp1.front().servicetime);
   				totalturnaround=totalturnaround+temp1.front().turnaroundtime;
   				totalnormturn=totalnormturn+temp1.front().normturn;
   				temp2.push(temp1.front());
   			        temp1.pop();
   			        p++;
   		}else{
   			time++;
   		}
   	}
   	//if type trace output will be grantchart represent time of ready and execution
   	if(type=="trace"){
   	cout <<"FCFS  ";
   	noduration(stoi(nop),stoi(duration));
   	 for(i=0;i<stoi(nop);i++){
   	 	std::cout << temp2.front().pname <<"     ";
   		temp2.pop();
    		for(j=0;j<stoi(duration);j++)
    			printf("|%c",res[i][j]);
    		cout<<"| "<<endl;
    	}
   	cout<<"------------------------------------------------"<<endl;
   	cout<<endl;
   	}
   	 if (type == "stats"){
   	cout <<"FCFS"<<endl;
   	outputb(temp2,stoi(nop),totalturnaround,totalnormturn);
   	}}
   	//-----------------------------------------------------------------
   	else if (algo[m][0]=='2'){ //RoundRobin Algorihtm
   	q=algo[m][2]-48; //get value of q that read as input
   	queue<struct process> temp3;
   	int x=stoi(nop),current,ready;
   	for (time=0;time<stoi(duration);time++){
   		//loop for all process in queue if any one of them is arrived push in temp2 (ready queue)
   		for(p=0;p<x;p++){
   			process c=temp1.front();
   			temp1.pop();
   			if (c.arrivaltime<=time ){
   				temp2.push(c);	
   				x=x-1;	
   			}
   			else{
   			temp1.push(c);
   			}
   		}
   		//return process that exexute to queue after check if any process arrived
		while(!temp3.empty()){
			temp2.push(temp3.front());
			temp3.pop();
		}
		//take first process waiting in the queue and start exexution
   		process p = temp2.front();
   		current=p.index;
   		temp2.pop();
   		//print '.' to all process waiting in queue
   		while(!temp2.empty()){
   			temp3.push(temp2.front());
   			temp2.pop();
   		}
   		while(!temp3.empty()){
   			ready=temp3.front().index;
   			res[ready][time]='.';
   			temp2.push(temp3.front());
   			temp3.pop();
   		}
   		//remaining time = service time ->decrement by 1 as it execute by 1
   		//running time initialize to zero at first and at each execute increment by1 to compare with quantum
   		res[current][time]='*';
   		p.remainingtime=p.remainingtime-1;
   		p.runningtime=p.runningtime+1;
   		//if remainingtime equal to zero then process finish execution 
   		//and calculate finishtime,turnaround,normturn 
   		if (p.remainingtime==0){
   			while(!done.empty()){
   			if(p.index==done.front().index){
   			done.front().finishtime=time+1;
   			done.front().turnaroundtime=done.front().finishtime-done.front().arrivaltime;
   	        	done.front().normturn=float(done.front().turnaroundtime)/float(done.front().servicetime);
   	        	totalturnaround=totalturnaround+done.front().turnaroundtime;
   			totalnormturn=totalnormturn+done.front().normturn;
   			}	
   			temp3.push(done.front());
   			done.pop();
 			}
 			while(!temp3.empty()){
   			done.push(temp3.front());
   			temp3.pop();}
   		}//process continue exexution
   		else{ //if runningtime equal quantum then it will enter ready queue
   			if(p.runningtime==q){
   				p.runningtime=0;
   				temp3.push(p);
   			}
   	  //same process need to continue exexution in the next interval of time ->put at the front of ready queue
   			else{
   				while(!temp2.empty()){
   				temp3.push(temp2.front());
   				temp2.pop();
   				}
   				temp2.push(p);
   				while(!temp3.empty()){
   				temp2.push(temp3.front());
   				temp3.pop();
   				}
   			}
   		}
   	}
   	if (type=="trace"){
    	cout <<"RR-"<<q<< "  ";
   	noduration(stoi(nop),stoi(duration));
        for(i=0;i<stoi(nop);i++){
   	 	std::cout << done.front().pname <<"     ";
   		done.pop();
    		for(j=0;j<stoi(duration);j++)
    			printf("|%c",res[i][j]);
    		cout<<"| "<<endl;
    	}
   	cout<<"------------------------------------------------"<<endl;
   	cout<<endl;
   	}if(type =="stats"){
   	cout <<"RR-"<<q<<endl;
   	outputb(done,stoi(nop),totalturnaround,totalnormturn);
   	}
   	//----------------------------------------------------------------------------
   	}
   	else if(algo[m][0] == '3'){ //SPN algorithm
     int time=0;int loc;
     while(time<stoi(duration)){
      int min=1000;
      //loop for all process to get process with min service time that will start execute
      for(int i=0;i<stoi(nop);i++){
         if((temp1.front().arrivaltime <= time) && (temp1.front().servicetime<min) && (temp1.front().complete != 1)){
            min=temp1.front().servicetime;
            loc=i;
         }
            temp2.push(temp1.front()); 
            temp1.pop();
         }
          for(int j=0;j<stoi(nop);j++) {
          //loop for all process and compare with index that will need to execute
            if(j==loc){
            //print '.' at time when process was in ready queue
              for(i=temp2.front().arrivaltime;i<time;i++){
   			res[j][i]='.';
   		}
   		//calculate finishtime
   		temp2.front().finishtime=time+temp2.front().servicetime;
   		//print '*' at time of execution
   		for(i=time;i<temp2.front().finishtime;i++){
   		 res[j][i]='*';
   		}//assign complete to this process ->next loop donot calculate its ratio
              	temp2.front().complete=1;
              	//assign time to finish time of the process that executed
              	time=temp2.front().finishtime;
              	//calculate turnaroundtime and normturn
              	temp2.front().turnaroundtime=temp2.front().finishtime-temp2.front().arrivaltime;
   	        temp2.front().normturn=float(temp2.front().turnaroundtime)/float(temp2.front().servicetime);
   		totalturnaround=totalturnaround+temp2.front().turnaroundtime;
   		totalnormturn=totalnormturn+temp2.front().normturn;
            }
            temp1.push(temp2.front());
            temp2.pop();
          }
        }
        if (type=="trace"){
        cout <<"SPN   ";
    	noduration(stoi(nop),stoi(duration));
        for(i=0;i<stoi(nop);i++){
   	 	std::cout << temp1.front().pname <<"     ";
   		temp1.pop();
    		for(j=0;j<stoi(duration);j++)
    			printf("|%c",res[i][j]);
    		cout<<"| "<<endl;
    	}
   	cout<<"------------------------------------------------"<<endl;
   	cout<<endl;
   	}
   	if(type =="stats"){
   	cout <<"SPN"<<endl;
   	outputb(temp1,stoi(nop),totalturnaround,totalnormturn);
   	}}
   	//----------------------------------------------------
   	else if(algo[m][0]== '4'){ //SRT algorithm
   	priority_queue<struct process,vector <process>,compare> temp3;
   	int x=stoi(nop),current,ready;
   	for (time=0;time<stoi(duration);time++){
   	//loop for all process in queue if any one of them is arrived push in temp3 (ready queue)
   		for(p=0;p<x;p++){
   			process c=temp1.front();
   			temp1.pop();
   			if (c.arrivaltime<=time ){
   				temp3.push(c);	
   				x=x-1;	
   			}
   			else{
   			temp1.push(c);
   			}
   		}
   		//take first process waiting in the queue and start exexution
   		process p=temp3.top();
   		current=p.index;
   		temp3.pop();
   		//remaining time = service time ->decrement by 1 as it execute by 1
   		p.remainingtime=p.remainingtime-1;
   		//print '.' to all process waiting in queue
   		while(!temp3.empty()){
   			temp2.push(temp3.top());
   			temp3.pop();
   		}
   		while(!temp2.empty()){
   			ready=temp2.front().index;
   			res[ready][time]='.';
   			temp3.push(temp2.front());
   			temp2.pop();
   		}
   		//if remainingtime equal to zero then process finish execution 
   		//and calculate finishtime,turnaround,normturn 
   		if (p.remainingtime==0){
   			while(!done.empty()){
   			if(p.index==done.front().index){
   			done.front().finishtime=time+1;
   			done.front().turnaroundtime=done.front().finishtime-done.front().arrivaltime;
   	        	done.front().normturn=float(done.front().turnaroundtime)/float(done.front().servicetime);
   	        	totalturnaround=totalturnaround+done.front().turnaroundtime;
   			totalnormturn=totalnormturn+done.front().normturn;
   			}	
   			temp2.push(done.front());
   			done.pop();
 			}
 			while(!temp2.empty()){
   			done.push(temp2.front());
   			temp2.pop();}
   		
   		}else{
   			temp3.push(p);
   		}
   		res[current][time]='*';	
   	}
   	 if (type=="trace"){
        cout <<"SRT   ";
    	noduration(stoi(nop),stoi(duration));
        for(i=0;i<stoi(nop);i++){
   	 	std::cout << done.front().pname <<"     ";
   		done.pop();
    		for(j=0;j<stoi(duration);j++)
    			printf("|%c",res[i][j]);
    		cout<<"| "<<endl;
    	}
   	cout<<"------------------------------------------------"<<endl;
   	cout<<endl;
   	}
   	if(type=="stats"){
   	cout <<"SRT"<<endl;
   	outputb(done,stoi(nop),totalturnaround,totalnormturn);
   	}
   	//----------------------------------------------------------------------
	}else if(algo[m][0]== '5'){ //HRRN Algorithm
   	for(time=0;time<stoi(duration);){
   		int loc; 
   		float ratio;
   		float highratio=-999;
   		//calculate ratio for every process and get high ratio that will start execute
   		for(p=0;p<stoi(nop);p++){
   			if(temp1.front().arrivaltime<=time && temp1.front().complete!=1){
   			ratio=float(temp1.front().servicetime+(time-temp1.front().arrivaltime))/float(temp1.front().servicetime);	
   		if(highratio<ratio){
   			highratio=ratio;
   			loc=p; //get index of process that will start execute 
   		}}
   		temp2.push(temp1.front());
   		temp1.pop();
   		}
   		for(p=0;p<stoi(nop);p++){
   			//loop for all process and compare with index that will need to execute
   			if(p==loc){
   				//print '.' at time when process was in ready queue
   				for(j=temp2.front().arrivaltime;j<time;j++){
   			 		res[p][j]='.';
   			 	}
   			 	//calculate finishtime
   			 	temp2.front().finishtime=time+temp2.front().servicetime;
   			 	//print '*' at time of execution 
   			 	for(j=time;j<temp2.front().finishtime;j++){
   			 		res[p][j]='*';
   			 	}//assign time to finish time of the process that executed
   			 	time=temp2.front().finishtime;
   			 	//assign complete to this process ->next loop donot calculate its ratio
   			 	temp2.front().complete=1;
   			 	//calculate turnaroundtime and normturn
   			 	temp2.front().turnaroundtime=temp2.front().finishtime-temp2.front().arrivaltime;
   	        		temp2.front().normturn=float(temp2.front().turnaroundtime)/float(temp2.front().servicetime);
   				totalturnaround=totalturnaround+temp2.front().turnaroundtime;
   				totalnormturn=totalnormturn+temp2.front().normturn;
   			}
   			temp1.push(temp2.front());
   			temp2.pop();	
   		}	
   	}
   	if(type == "trace"){
   	cout <<"HRRN  ";
	noduration(stoi(nop),stoi(duration));
   	for(i=0;i<stoi(nop);i++){
   	 	std::cout << temp1.front().pname <<"     ";
   		temp1.pop();
    		for(j=0;j<stoi(duration);j++)
    			printf("|%c",res[i][j]);
    		cout<<"| "<<endl;
    	}
   	cout<<"------------------------------------------------"<<endl;
   	cout<<endl;
   	}
   	if(type == "stats"){
   	cout <<"HRRN"<<endl;
   	outputb(temp1,stoi(nop),totalturnaround,totalnormturn);
   	}}
   	//-----------------------------------------------------------
   	else if(algo[m][0]=='6'){	//Feedback q=1 algorithm
   	priority_queue<struct process,vector <process>,fdcompare> temp3;
   	int n,x=stoi(nop),current,ready;
   	for (time=0;time<stoi(duration);time++){
   	//loop for all process in queue if any one of them is arrived push in temp2 (ready queue)
   		for(p=0;p<x;p++){
   			process c=temp1.front();
   			temp1.pop();
   			if (c.arrivaltime<=time ){
   				c.priority=0;
   				temp3.push(c);
   				x=x-1;	
   			}
   			else{
   			temp1.push(c);
   			}
   		}
   		//if there is one process in ready queue then it must remain on processor with same priority	
   		if(temp3.empty()){
   			temp2.front().priority=temp2.front().priority-1;
   			temp3.push(temp2.front());
   			temp2.pop();
   		}
   		//return process that exexute to queue after check if any process arrived
   		while(!temp2.empty()){
   		temp3.push(temp2.front());
   		temp2.pop();
   		}
   		//take first process waiting in the queue and start exexution
   		process p=temp3.top();
   		current=p.index;
   		temp3.pop();
   		//remaining time = service time ->decrement by 1 as it execute by 1
   		res[current][time]='*';
   		p.remainingtime=p.remainingtime-1;
   		//print '.' to all process waiting in queue
   		while(!temp3.empty()){
   			temp2.push(temp3.top());
   			temp3.pop();
   		}
   		while(!temp2.empty()){
   			ready=temp2.front().index;
   			res[ready][time]='.';
   			temp3.push(temp2.front());
   			temp2.pop();
   		}
   		//if remainingtime equal to zero then process finish execution 
   		//and calculate finishtime,turnaround,normturn 
   		if (p.remainingtime==0){
   			while(!done.empty()){
   			if(p.index==done.front().index){
   			done.front().finishtime=time+1;
   			done.front().turnaroundtime=done.front().finishtime-done.front().arrivaltime;
   	        	done.front().normturn=float(done.front().turnaroundtime)/float(done.front().servicetime);
   	        	totalturnaround=totalturnaround+done.front().turnaroundtime;
   			totalnormturn=totalnormturn+done.front().normturn;
   			}	
   			temp2.push(done.front());
   			done.pop();
 			}
 			while(!temp2.empty()){
   			done.push(temp2.front());
   			temp2.pop();}
   		}else{
   			p.priority=p.priority+1;
   			temp2.push(p);	
   		}	
   	}
   	if(type=="trace"){
   	cout <<"FB-1  ";
	noduration(stoi(nop),stoi(duration));
   	 for(i=0;i<stoi(nop);i++){
   	 	std::cout << done.front().pname <<"     ";
   		done.pop();
    		for(j=0;j<stoi(duration);j++)
    			printf("|%c",res[i][j]);
    		cout<<"| "<<endl;
    	}
   	cout<<"------------------------------------------------"<<endl;
   	cout<<endl;
   	}
   	if(type == "stats"){
   	cout <<"FB-1"<<endl;
   	outputb(done,stoi(nop),totalturnaround,totalnormturn);
   	}}
   	//--------------------------------------------------------------------------------------
   	else if(algo[m][0]=='7'){	//Feedback q=2^i algorithm
   	priority_queue<struct process,vector <process>,fdcompare> temp3;
   	int n;
   	int l=0;
   	int x=stoi(nop),current,ready;
   	process p;
   	for (time=0;time<stoi(duration);time++){
   		//loop for all process in queue if any one of them is arrived push in temp2 (ready queue)
   		for(i=0;i<x;i++){
   			process c=temp1.front();
   			temp1.pop();
   			if (c.arrivaltime<=time ){
   				c.priority=0;
   				temp3.push(c);
   				x=x-1;	
   			}
   			else{
   			temp1.push(c);
   			}
   		}
   		//if there is one process in ready queue then it must remain on processor with same priority
   		if(temp3.empty() && !temp2.empty()){
   			temp2.front().priority=temp2.front().priority-1;
   		}
   		if(l==0){ //if quantum of process finish then take process with high priority and start execution
   		while(!temp2.empty()){
   		temp3.push(temp2.front());
   		temp2.pop();
   		}
  		p=temp3.top();
   		temp3.pop();
   		}//if quantum of process doesnot finish then take same process and continue execution
   		else{
   		p=temp2.front();
   		temp2.pop();
   		}
   		current=p.index;
   		//remaining time = service time ->decrement by 1 as it execute by 1
   		//running time initialize to zero at first and at each execute increment by1 to compare with quantum
   		res[current][time]='*';
   		p.remainingtime=p.remainingtime-1;
   		p.runningtime=p.runningtime+1;
   		//print '.' to all process waiting in queue
   		while(!temp3.empty()){
   			temp2.push(temp3.top());
   			temp3.pop();
   		}
   		while(!temp2.empty()){
   			ready=temp2.front().index;
   			res[ready][time]='.';
   			temp3.push(temp2.front());
   			temp2.pop();
   		}
   		//if remainingtime equal to zero then process finish execution 
   		//and calculate finishtime,turnaround,normturn 
   		if (p.remainingtime==0){
   			l=0;
   			while(!done.empty()){
   			if(p.index==done.front().index){
   			done.front().finishtime=time+1;
   			done.front().turnaroundtime=done.front().finishtime-done.front().arrivaltime;
   	        	done.front().normturn=float(done.front().turnaroundtime)/float(done.front().servicetime);
   	        	totalturnaround=totalturnaround+done.front().turnaroundtime;
   			totalnormturn=totalnormturn+done.front().normturn;
   			}	
   			temp2.push(done.front());
   			done.pop();
 			}
 			while(!temp2.empty()){
   			done.push(temp2.front());
   			temp2.pop();}
   		}else{//calculate q=2^(priority of process excuted)
   			q=int(pow(2*1.0,p.priority*1.0));
   			//if runningtime equal q then return runningtime to zero and increment priority level 
   			if(p.runningtime==q){
   			p.runningtime=0;
   			p.priority=p.priority+1; 
   			l=0;			
   			}
   			else l++; // continue execution
   			temp2.push(p);	
   		}	
   	}
   	if(type=="trace"){
   	cout <<"FB-2i ";
	noduration(stoi(nop),stoi(duration));
   	 for(i=0;i<stoi(nop);i++){
   	 	std::cout << done.front().pname <<"     ";
   		done.pop();
    		for(j=0;j<stoi(duration);j++)
    			printf("|%c",res[i][j]);
    		cout<<"| "<<endl;
    	}
   	cout<<"------------------------------------------------"<<endl;
   	cout<<endl;
   	}
   	if(type == "stats"){
   	cout <<"FB-2i"<<endl;
   	outputb(done,stoi(nop),totalturnaround,totalnormturn);
   	}}
   	//------------------------------------------------------------------------
   	else if(algo[m][0]== '8'){   // Aging algorithim
   	q=algo[m][2]-48;                          // Converting the quantum from ASCI to decimel
	queue<struct process> currentprocess;    // Queue that stores the current running process
    priority_queue<struct process,vector <process>,compare2> ready;   //Pariority queue to pick highest pariority
	process d;int dot;                     // Variables used in handling waiting of processes
	process x;process c;
	int time=0,current;     	
	while(time<stoi(duration) ){       //looping on the whole duration
	  if(!currentprocess.empty()){
	   currentprocess.front().pariority=currentprocess.front().servicetime;
	  }
	  if(temp1.front().arrivaltime<=time && !temp1.empty()){//if the process arrival time equals the time instant I'm in
		temp1.front().runningtime=90;                 
		ready.push(temp1.front());                 //then process is pushed in ready queueu
		temp1.pop();
	  }
	  while(!ready.empty()){            // 2 while loops to increament pariorities in ready queue by 1
	     x=ready.top();               
	    x.pariority=x.pariority+1; //increamenting pariority by 1 and pushing it in temp2
            temp2.push(x);		   //(temp2 only used as a temprory place to store process)
	    ready.pop();
	 }
	 while(!temp2.empty()){              // temp2 will be empty again
    	  ready.push(temp2.front());     // Repushing them again in ready queue after increamenting pariority
    	  temp2.pop();
   	 }
	if(!currentprocess.empty()){
	  ready.push(currentprocess.front());
	  //pushing current process in ready queue to chose max pariority according to compare2 function
	  currentprocess.pop();
	}
	// pushing the highest pariority in current process to be running for time quantum 'q'
	currentprocess.push(ready.top());       
	ready.pop();
	c=currentprocess.front();     //to print '*' for running process
	current=c.index;
	currentprocess.front().runningtime=time;
    for(i=time;i<time+q;i++){
		if(i<stoi(duration)){
		   res[current][i]='*'; 
	}}
	time=time+q;         //assigning time to time + quantum done
   	}
        while(!temp4.empty()){       //for handling waiting for process
		d=temp4.front();
	        dot=d.index;
		temp4.pop();
		for(int k=d.arrivaltime;k<stoi(duration);k++){
		  if(res[dot][k]!= '*'){
		  res[dot][k]='.';
		 }
		}
	}
	if (type=="trace"){
              cout <<"Aging ";
    	      noduration(stoi(nop),stoi(duration));
              for(i=0;i<stoi(nop);i++){
   	      std::cout <<done.front().pname<<"     ";
   	      done.pop();
    	      for(j=0;j<stoi(duration);j++)
    		printf("|%c",res[i][j]);
    	  	cout<<"| "<<endl;
    	     }
             cout<<"------------------------------------------------"<<endl;
             cout<<endl;
	}
	}	
  }return 0;
}
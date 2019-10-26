#include<stdio.h>
using namespace std;

struct process {
	int pid,at,st,p;
}; 

process proc[50];

bool comp(process a,process b) {
	if(a.at == b.at)
		return a.p < b.p;

	return a.at < b.at;	
}

void fcfs(int n,process proc[]) {
	int service_time[n],tat[n],wt[n],total_wt = 0,total_tat = 0;
	float avg_wt,avg_tat;

	service_time[0]=0;
	wt[0] = 0;
	for(int i=1;i<n;i++) {
		service_time[i] = service_time[i-1] + proc[i-1].st;
		wt[i] = service_time[i] - proc[i].at;
		if(wt[i]<0)
			wt[i]=0; 
	}
	for(int i=0;i<n;i++) {
		tat[i] = proc[i].st + wt[i];
	}
	cout<<"PID "<<"Wt "<<"Tat "<<endl;
	for(int i=0;i<n;i++)
		cout<<proc[i].pid<<" "<<wt[i]<<" "<<tat[i]<<endl;	

}


int main() {
	int n;
	cin>>n;
	for(int i=0;i<n;i++)
		cin>>proc[i].pid>>proc[i].at>>proc[i].st>>proc[i].p;

	sort(proc,proc+n,comp);
	fcfs(n,proc);
	return 0;
}
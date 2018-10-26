#include<stdio.h>
#include<stdlib.h>
struct Node{
	int data;
	struct Node *next;
};

void append(struct Node** head_ref, int new_data)
{
    struct Node* new_node = (struct Node*) malloc(sizeof(struct Node));
    struct Node *last = *head_ref;
    new_node->data  = new_data;
    new_node->next = NULL;
 
    if (*head_ref == NULL)
    {
       *head_ref = new_node;
       return;
    }  
    while (last->next != NULL)
        last = last->next;
    last->next = new_node;
    return;    
}

void printList(struct Node *node)
{
  while (node != NULL)
  {
     printf("%d", node->data);
     node = node->next;
  }
}

int main(){
	int N;
	printf("Enter the number of couples");
	scanf("%d",&N);
	int manPref[N][N];
	int womenPref[N][N];
	int rankByWomen[N][N];
	int next[N];
	int womStatus[N];
	struct Node *freeMan = (struct Node*)malloc(sizeof(struct Node));
	printf("Enter then men's preference list");
	for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            scanf("%d",&manPref[i][j]);
            manPref[i][j]--;
        }
        next[i]=0;
        append(&freeMan,i);
	}
    freeMan = freeMan->next;
	printf("Enter then women's preference list");
	for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            scanf("%d",&womenPref[i][j]);
            womenPref[i][j]--;
            rankByWomen[i][womenPref[i][j]]=j;
        }
        womStatus[i] =-1;
	}
	int s[N][2];
	int number=0;
	while(freeMan!=NULL && next[freeMan->data]!=N){
        if(womStatus[manPref[freeMan->data][next[freeMan->data]]]==-1){
            s[number][0] = freeMan->data;
            s[number][1] = manPref[freeMan->data][next[freeMan->data]];
            womStatus[manPref[freeMan->data][next[freeMan->data]]] = freeMan->data;
            freeMan=freeMan->next;
            number++;
        }
        else{
            if(rankByWomen[next[freeMan->data]][freeMan->data]<womStatus[manPref[freeMan->data][next[freeMan->data]]]){
                s[number][0] =freeMan->data;
                s[number][1] = manPref[freeMan->data][next[freeMan->data]];
                womStatus[manPref[freeMan->data][next[freeMan->data]]]=freeMan->data;
                freeMan=freeMan->next;
            }
            else{
             next[freeMan->data]++;
            }
        }
	}
    if(number == 0){
        printf("Not possible\n");
    }
	for(int i=0;i<number;i++){
        printf("%d marries %d\n",s[i][0], s[i][1]);
	}
}

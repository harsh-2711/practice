#include<bits/stdc++.h>
using namespace std;

int main(void) {
	string s;
	cout<<"Enter string to be coded: ";
	cin>>s;
	
	int A[26] = {0};
	
	for(unsigned int i = 0; i < s.length(); i++)
	{
		for(int j = 97; j < 123; j++) {
			if(s[i] == j) {
				A[j-97]++;
				break;
			}
		}
	}
	
	vector<pair<int,int>> vt;
	
	for(int i = 0; i < 26; i++) {
		if(A[i] != 0)
			vt.push_back(make_pair(A[i],i+97));
	}
	
	sort(vt.begin(),vt.end());
	
	char tree[vt.size()*vt.size()] = {0};
		
	int node_location = 0;
	
	for(unsigned int i = 0; i < vt.size(); i+=2) {
		tree[2*node_location] = vt[vt.size()-i-1].second;
		tree[2*node_location + 1] = vt[vt.size()-i-2].second;
		node_location++;
	}
	
	vector<vector<string>> huffmanCodes(100);
	
	huffmanCodes[0].push_back("0");
	huffmanCodes[1].push_back("1");
	
	node_location = 0;
	
	for(unsigned int i = 2; i < vt.size()*vt.size(); i+=2) {
		huffmanCodes[i] = huffmanCodes[node_location];
		huffmanCodes[i].push_back("0");
		huffmanCodes[i] = huffmanCodes[node_location];
		huffmanCodes[i].push_back("1");
		node_location++;
	} 
	
	cout<<"\n";
	for(unsigned int i = 0; i < vt.size(); i++) {
	    if(tree[i] != 0) {
	        cout<<tree[i]<<" - ";
	        for(unsigned int j = 0; j < huffmanCodes[i].size(); j++) {
	            cout<<huffmanCodes[i][j];
	        }
	        cout<<"\n";
	    }
	}
	
}

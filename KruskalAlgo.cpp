#include<bits/stdc++.h>
#include<cstdio>
using namespace std;

class KruskalMST {
    int nodes;
    int edges;
    vector<int> from;
    vector<int> to;
    vector<int> weight;
    vector<pair<int,int>> sorted;
    int start, stop;
    
    int count = 0;
    
    public:
    
    vector<int> parents;
    vector<pair<int,int>> bandwidths;
    
    KruskalMST(int g_nodes, int g_edges, vector<int> g_from, vector<int> g_to, vector<int> g_weight, vector<pair<int,int>> vt, int starting, int stopping) {
        nodes = g_nodes;
        edges = g_edges;
        from = g_from;
        to = g_to;
        weight = g_weight;
        sorted = vt;
        start = starting;
        stop = stopping;
        
        for(int i = 0; i < nodes; i++) {
            parents.push_back(-1);
        }
    }
    
    int find(int node) {
        if(parents[node] == -1) {
            return node;
        }
        return find(parents[node]);
    }
    
    void Union(int node1, int node2) {
        int parent1 = find(node1);
        int parent2 = find(node2);
        
        if(parent1 == -1)
            parents[parent1] = parent2;
        else
            parents[parent2] = parent1;
    }
    
    void formula() {
        for(int i = 0; i < edges; i++) {
            int index = sorted[edges-i-1].second;
            
            int parent1 = find(from[index]);
            int parent2 = find(to[index]);
            
            if(parent1 != parent2) {
                count++;
                bandwidths.push_back(make_pair(from[index],weight[i]));
                Union(parent1, parent2);
            }
            
            if(count == nodes-1)
                break;
        }
        
        sort(bandwidths.begin(), bandwidths.end());
        vector<int> total;
        for(int i = start-1; i < stop-1; i++) {
            total.push_back(bandwidths[i].second);
        }
        sort(total.begin(), total.end());
        cout<<"\nShortest Bandwidth along the path with most weight: "<<total[0]<<endl;
        
    }
};

int main(void) {
    int noOfStations;
    cout<<"\nEnter number of stations: ";
    cin>>noOfStations;
    int noOfCable;
    cout<<"\nEnter number of cables: ";
    cin>>noOfCable;
    
    vector<int> g_from(noOfCable);
    vector<int> g_to(noOfCable);
    vector<int> g_weight(noOfCable);
    vector<pair<int,int>> vt;
    
    for(int i = 0; i < noOfCable; i++) {
        cout<<"\nEnter starting station, stoping station, and bandwidth between them: ";
        cin>>g_from[i]>>g_to[i]>>g_weight[i];
        vt.push_back(make_pair(g_weight[i], i));
    }
    
    sort(vt.begin(), vt.end());
    
    int start, stop;
    cout<<"\nEnter starting station: ";
    cin>>start;
    cout<<"\nEnter stoping station: ";
    cin>>stop;
    
    KruskalMST kmst = KruskalMST(noOfStations, noOfCable, g_from, g_to, g_weight, vt, start, stop);
    kmst.formula();
}

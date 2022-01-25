// c++ STL containers
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <set>
#include <algorithm>
#include <cstdlib>
#include <iterator>
#include <cassert>
// LEMON
#include "lemon/smart_graph.h"
#include "lemon/dijkstra.h"


struct Arc{
    std::string sourceID;
    std::string targetID; 
    int cost;  
}; 

int main(int argc, char *argv[]){
    unsigned int count = 0; 
    std::ifstream f;   
    std::string start_node, end_node; 
    std::vector<std::string> source_nodes; 
    std::vector<std::string> dst_nodes; 
    std::vector<int> weight; 
    std::string line; 
    std::set<std::string> unique_set;
    std::vector<std::string> unique_nodes; 
    std::string s1, s2; 
    int w; 
    

    if (argc == 4){ // file 
        start_node = argv[2];
        end_node = argv[3];
        f.open(argv[1]); 
        for(int i = 0; f >> s1 >> s2 >> w; ++i){
            source_nodes.push_back(s1);
            dst_nodes.push_back(s2); 
            weight.push_back(w); 
            unique_set.insert(source_nodes.at(i));
            unique_set.insert(source_nodes.at(i)); 
            count++; 
        }
    }
    else{ // no file 
        start_node = argv[1];
        end_node = argv[2]; 

        for(int i = 0; std::cin>> s1 >> s2>> w; ++i){
            source_nodes.push_back(s1);
            dst_nodes.push_back(s2);
            weight.push_back(w); 
            unique_set.insert(source_nodes.at(i)); 
            unique_set.insert(dst_nodes.at(i));
            count++; 
        }
    }

    unique_nodes.resize(unique_set.size()); 
    std::copy(unique_set.begin(), unique_set.end(), unique_nodes.begin()); 
    if(start_node == end_node){
        std::cout << "0: " << start_node; 
        return 0; 
    }

    std::map<std::string, int> nodes; 
        for (unsigned int i = 0; i < unique_nodes.size(); i++){
            nodes.insert(std::make_pair(unique_nodes[i], i)); 
        }

    std::vector<Arc> Arcs; 
        for (unsigned int i = 0; i < count; i++){
            Arc temp = Arc{source_nodes[i], dst_nodes[i], weight[i]}; 
            Arcs.push_back(temp); 
        }
    

    using SptSolver = lemon::Dijkstra <lemon::SmartDigraph, lemon::SmartDigraph::ArcMap<double>>; 
    lemon::SmartDigraph g; 
    lemon::SmartDigraph::ArcMap<double> costMap(g); 
    lemon::SmartDigraph::NodeMap<std::string> nodeMap(g);

    std::map<std::string, int> lemon_graph_node_id; 

    
//populate graph
//nodes first
lemon::SmartDigraph::Node currentNode;
for (auto nodesIter = nodes.begin(); nodesIter != nodes.end(); ++nodesIter)
{
    std::string key = nodesIter->first; // double check std 
    currentNode = g.addNode();
    nodeMap[currentNode] = key;
    lemon_graph_node_id[key] = g.id(currentNode);
}

//then the arcs with the costs through the cost map
lemon::SmartDigraph::Arc currentArc;
for (auto arcsIter = Arcs.begin(); arcsIter != Arcs.end(); ++arcsIter)
{
    int sourceIndex = nodes.at(arcsIter->sourceID);
    int targetIndex = nodes.at(arcsIter->targetID);
 
    lemon::SmartDigraph::Node sourceNode = g.nodeFromId(sourceIndex);
    lemon::SmartDigraph::Node targetNode = g.nodeFromId(targetIndex);
 
    currentArc = g.addArc(sourceNode, targetNode);
    costMap[currentArc] = arcsIter->cost;
}

lemon::SmartDigraph::Node start = g.nodeFromId(lemon_graph_node_id.at(start_node)); 
lemon::SmartDigraph::Node end = g.nodeFromId(lemon_graph_node_id.at(end_node)); 

SptSolver spt(g, costMap);

if(spt.run(start, end)){
    std::vector<lemon::SmartDigraph::Node> path; 
    for(lemon::SmartDigraph::Node v = g.nodeFromId(lemon_graph_node_id.at(end_node)); 
            v != g.nodeFromId(lemon_graph_node_id.at(start_node)); 
            v = spt.predNode(v))
        {
            if (v != lemon::INVALID && spt.reached(v)){
                path.push_back(v); 
            }

        }
        path.push_back(g.nodeFromId(lemon_graph_node_id.at(start_node))); 

        unsigned int cost = 0; 
        cost = spt.dist(g.nodeFromId(lemon_graph_node_id.at(end_node))); 
        std::cout << cost << ": "; 
        for(auto p = path.rbegin(); p != path.rend(); ++p){
            std::cout << nodeMap[*p] << " "; 
        }
}
else{
    std::cout<<"No path exists from " << start_node <<" to " << end_node << std::endl; 
}


return 0; 

}
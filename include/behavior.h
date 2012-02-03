/**
 *  
 * 
 */
 
#ifndef __BEHAVIOR_H
#define __BEHAVIOR_H

#include<vector>
#include<string>
#include<map>

#include<utility>

#include<boost/graph/graph_traits.hpp>
#include<boost/graph/adjacency_list.hpp>
 
#include<step.h>
#include<trace.h>
 
using namespace boost; 
 
typedef adjacency_list<vecS, vecS, directedS> BehaviorGraph;

typedef graph_traits<BehaviorGraph>::vertex_descriptor BGVertex; 

typedef vector<Step*> BGProperties;

class Behavior {
	
	private:
		// graph to store vertecies and edges of steps
		BehaviorGraph bg_;
		// vector to store information about steps associated with
		// each vertex of grath
		BGProperties props_;
		
	public:
		Behavior();
		~Behavior();
		
		// add a new vertex and associate the given state with it
		BGVertex add_step(Step* step);
		// obtaine step associated with given vertex
		Step* get_step(BGVertex vertex); 
		
		void add_edge(BGVertex v, BGVertex u);
		void remove_edge(BGVertex v, BGVertex u);
		
};
 
 
 #endif // __BEHAVIOR_H

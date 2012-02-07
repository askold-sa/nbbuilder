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
#include<vis.h>
 
using namespace boost; 
 
typedef adjacency_list<vecS, vecS, directedS> BehaviorGraph;

typedef graph_traits<BehaviorGraph>::vertex_descriptor BGVertex; 
typedef graph_traits<BehaviorGraph>::vertex_iterator BGVertexIter;

typedef vector<Step*> BGProperties;

class Behavior {
	
	private:
		// graph to store vertecies and edges of steps
		BehaviorGraph bg_;
		// vector to store information about steps associated with
		// each vertex of grath
		BGProperties props_;
		// root of behavior tree
		BGVertex root_;
		
	public:
		Behavior();
		~Behavior();
		
		// add a new vertex and associate the given state with it
		BGVertex add_step(Step* step);
		// obtaine step associated with given vertex
		Step* get_step(BGVertex vertex); 
		
		void add_edge(BGVertex v, BGVertex u);
		void remove_edge(BGVertex v, BGVertex u);
		
		void set_root(BGVertex);
		BGVertex get_root();
		
		// add to behavior graph a path of steps from trace
		// between two iterators
		// and connect this path to given pair of vertices
		void add_path(Trace::const_iterator,
			Trace::const_iterator, BGVertex, BGVertex);
		// add path for each trace in trace set and
		// place these paths between given vertices
		void add_traces(const TraceSet&,BGVertex, BGVertex);
		
		void debugPrint();
		
		// produce graph layout in dot format (graphvis)
		string produce_dot();
		// save graph layout to file
		void save_dot(const string filename, const string &dot);
};
 
 
 #endif // __BEHAVIOR_H

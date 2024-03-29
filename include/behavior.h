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
 
typedef adjacency_list<listS, listS, directedS> BehaviorGraph;

typedef graph_traits<BehaviorGraph>::vertex_descriptor BGVertex; 
typedef graph_traits<BehaviorGraph>::vertex_iterator BGVertexIter;
typedef graph_traits<BehaviorGraph>::edge_descriptor BGEdge;
typedef graph_traits<BehaviorGraph>::out_edge_iterator BGOutEdgeIt;

typedef struct BGProperty 
{
	BGProperty(Step* sp, int ind) : step(sp),index(ind) {}
	Step* step;
	int index;
} BGProperty;
typedef map<BGVertex,BGProperty> BGProperties;

class Behavior {
	
	private:
		// graph to store vertecies and edges of steps
		BehaviorGraph bg_;
		// vector to store information about steps associated with
		// each vertex of grath
		BGProperties props_;
		// root of behavior tree
		BGVertex root_;
		// index for new vertex
		int cur_index_;
		
	public:
		Behavior();
		~Behavior();
		
		// add a new vertex and associate the given state with it
		BGVertex add_step(Step* step);
		// obtaine step associated with given vertex
		Step* get_step(BGVertex vertex) const; 
		//
		void remove_step(BGVertex vertex);
		// 
		int get_index(BGVertex vertex) const;
		
		void add_edge(BGVertex v, BGVertex u);
		void remove_edge(BGVertex v, BGVertex u);
		
		pair<BGOutEdgeIt,BGOutEdgeIt> get_out_edges(const BGVertex v) 
			const;
		BGVertex get_target(const BGEdge e) const;
		BGVertex get_source(const BGEdge e) const;
		
		void set_root(BGVertex);
		BGVertex get_root() const;
		
		// add to behavior graph a path of steps from trace
		// between two iterators
		// and connect this path to given pair of vertices
		void add_path(Trace::const_iterator,
			Trace::const_iterator, BGVertex, BGVertex);
		// add path for each trace in trace set and
		// place these paths between given vertices
		void add_traces(const TraceSet&,BGVertex, BGVertex);
		
		// produce graph layout in dot format (graphvis)
		string produce_dot() const;
		// save graph layout to file
		void save_dot(const string filename, const string &dot) const;
};
 
 
 #endif // __BEHAVIOR_H

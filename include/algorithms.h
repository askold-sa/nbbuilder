#ifndef __ALGORITHMS_H
#define __ALGORITHMS_H

#include<step.h>
#include<trace.h>
#include<behavior.h>
#include<miscfuncs.h>

map<string, BGVertex> create_layer(Behavior&, vector<trcit_pair_t>&);

void shift_iterators(vector<trcit_pair_t>&);

void link_layers(Behavior&, const TraceSet&,
	const map<string, BGVertex>&, const map<string, BGVertex>&);

/**
 * Create "full" behavior graph i.e. graph with some  
 * layers of "label vertex" and paths between pairs 
 * of neighbor label vertices
 * But in this graph could exist paths which violate label orders  
 */
Behavior make_full_bh(const TraceSet&);

/**
 * Take "full" behavior graph, produce new behavior graph
 * containing only such paths what have correct label orders
 */
Behavior make_full_with_clo (const Behavior&, const vector<LOrder>&);

#endif // __ALGORITHMS_H

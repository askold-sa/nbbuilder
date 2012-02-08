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

Behavior make_full_bh(const TraceSet&);

#endif // __ALGORITHMS_H

#ifndef __MISCFUNCS_H
#define __MISCFUNCS_H

#include<step.h>
#include<trace.h>
#include<behavior.h>

LOrder lorder(const Trace& trace);

TraceSet subt(const TraceSet& trace_set, string l1, string l2);

Trace::const_iterator get_next_label( Trace::const_iterator,
	Trace::const_iterator);

typedef pair<Trace::const_iterator,Trace::const_iterator> 
		trcit_pair_t;
		
map<string, BGVertex> create_layer(Behavior&, vector<trcit_pair_t>&);


void debugPrint(const Trace& trace);
void debugPrint(const TraceSet& trace_set);

#endif // __MISCFUNCS_H

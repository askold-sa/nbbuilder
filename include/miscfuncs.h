#ifndef __MISCFUNCS_H
#define __MISCFUNCS_H

#include<step.h>
#include<trace.h>
#include<behavior.h>

LOrder lorder(const Trace& trace);

bool lorder_is_equal(const LOrder&, const LOrder&);

vector<LOrder> lorder_unique(const TraceSet& traces);

bool lorder_in_set(const LOrder& lo, const vector<LOrder>& loset);

TraceSet subt(const TraceSet& trace_set, string l1, string l2);

Trace::const_iterator get_next_label( Trace::const_iterator,
	Trace::const_iterator);

typedef pair<Trace::const_iterator,Trace::const_iterator> 
		trcit_pair_t;

void debugPrint(const Trace& trace);
void debugPrint(const TraceSet& trace_set);

#endif // __MISCFUNCS_H

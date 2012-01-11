#ifndef __MISCFUNCS_H
#define __MISCFUNCS_H

#include<step.h>
#include<trace.h>

LOrder lorder(const Trace& trace);

TraceSet subt(const TraceSet& trace_set, string l1, string l2);

void debugPrint(const Trace& trace);

void debugPrint(const TraceSet& trace_set);

void debugPrint(const LOrder& lorder);

#endif // __MISCFUNCS_H

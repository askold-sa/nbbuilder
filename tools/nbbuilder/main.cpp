#include<iostream>

#include<step.h>
#include<trace.h>
#include<behavior.h>
#include<miscfuncs.h>

using namespace std;

#define MAX_TRACE_LEN	16
#define TRACE_Q			3

int main ( int argc, char **argv ) {

	vector<Step*> gs_steps;
	TraceSet traces;

	const char* names[TRACE_Q][MAX_TRACE_LEN] = {
		{"label:l0;","name:a;","name:b;",
		"label:l1;","name:c;","name:d;",
		"label:l2;","name:e;","name:f;",
		"label:l1;","name:g;","label:lfin;"},
		{"label:l1;","name:k;","name:l;",
		"label:l2;","name:p;","name:q;",
		"label:l1;","name:m;","name:n;",
		"label:l2;","name:x;",
		"label:l2;","label:l1;"},
		{"label:l2;","name:y;","label:l1;","name:z;"}
	};
    
    for (int i=0;i<TRACE_Q;i++) {
		Trace trace;
		for (int j=0;j<MAX_TRACE_LEN;j++)
			if (names[i][j]!=0) {
				Step *step = new StepImpl(string(names[i][j]));
				gs_steps.push_back(step);
				
				trace.push_back(step);
			}
		if (trace.size()!=0)
			traces.push_back(trace);
	}
	
	cout<<"Original traces:\n";
	debugPrint(traces);
	cout<<"LOrders:\n";
	for (TraceSet::const_iterator it=traces.begin();
		it!=traces.end();it++) 
	{
		debugPrint(lorder(*it));
		cout<<"\n";
	}
	
	string l1 = "l2", l2 = "l1";
	cout<<"Subtraces for labels: "<<l1<<" and "<<l2<<":\n";
	TraceSet subtraces = subt(traces,l1,l2);
	debugPrint(subtraces);	

	// create Behavior graph for original traces
	Behavior BHorig;
	BGVertex v,u;
	
	v = BHorig.add_step(NULL);
	BHorig.set_root(v);
	
	for (TraceSet::const_iterator it=traces.begin();
		it!=traces.end();it++) 
	{	
		v = BHorig.get_root();
		
		for (Trace::const_iterator itt=it->begin();
			itt!=it->end();itt++) 
		{	
			u = BHorig.add_step(*itt);
			BHorig.add_edge(v,u);
			v = u;
		}
	}
	
	BHorig.save_dot("orig.dot",BHorig.produce_dot());

	// create Behavior graph for trace extension
	Behavior BHext;
	BGVertex root,fin;
	
	root = BHext.add_step(NULL);
	BHext.set_root(root);
	for (TraceSet::const_iterator ts_it=traces.begin();
			ts_it!=traces.end();ts_it++) {
		
		fin = BHext.add_step(NULL);
		BHext.add_path(ts_it->begin(),ts_it->end(),root,fin);
	}
	
	BHext.save_dot("ext.dot",BHext.produce_dot());

    return 0;
}

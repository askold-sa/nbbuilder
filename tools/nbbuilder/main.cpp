#include<iostream>

#include<step.h>
#include<trace.h>
#include<behavior.h>
#include<miscfuncs.h>
#include<algorithms.h>

using namespace std;

#define MAX_TRACE_LEN	16
#define TRACE_Q			8

int main ( int argc, char **argv ) {

	vector<Step*> gs_steps;
	TraceSet traces;

	/*const char* names[TRACE_Q][MAX_TRACE_LEN] = {
		{"label:l0;","name:a;","name:b;",
		"label:l1;","name:c;","name:d;",
		"label:l2;","name:e;","name:f;",
		"label:l1;","name:g;","label:lfin;"},
		{"label:l1;","name:k;","name:l;",
		"label:l2;","name:p;","name:q;",
		"label:l1;","name:m;","name:n;",
		"label:l2;","name:x;",
		"label:l2;","label:l1;"},
		{"label:l2;","name:y;","label:l1;","name:z;","label:l3;"}
	};*/
	const char* names[TRACE_Q][MAX_TRACE_LEN] = {
		{"label:l0;","name:a;","label:l1;","name:b;","label:l2;"},
		{"label:l0;","name:a;","label:l1;","name:c;","label:l2;"},
		{"label:l0;","name:d;","label:l1;","name:b;","label:l2;"},
		{"label:l0;","name:d;","label:l1;","name:c;","label:l2;"},
		{"label:l0;","name:a;","label:l1;","name:e;","label:l2;"},
		{"label:l0;","name:a;","label:l3;","name:f;","label:l4;"},
		{"label:l0;","name:a;","label:l1;","name:b;","label:l5;"},
		{"label:l0;","name:a;","label:l1;","name:c;","label:l5;"}
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
	Behavior BHfull = make_full_bh(traces);
	BHfull.save_dot("ext.dot",BHfull.produce_dot());

	// obtain vector of correct label order
	vector<LOrder> lorder_vec = lorder_unique(traces);
	// create Behavior with only paths what have correct lorders
	Behavior BHfull_clo = make_full_with_clo(BHfull,lorder_vec);
	BHfull_clo.save_dot("clo.dot",BHfull_clo.produce_dot());

    return 0;
}

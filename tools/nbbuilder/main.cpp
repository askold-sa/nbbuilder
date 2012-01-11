#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>

#include<iostream>

#include<step.h>
#include<trace.h>
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
    
    /*
    Step *s1 = new StepImpl("dglkhgh name:stepA;   ");
    Step *s2 = new StepImpl("jfGHFGA;label:L1;   add-data: Gosznak;   ");

    cout << "s1 name: " << s1->get_name() << endl;
    cout << "s1 is label: " << s1->is_label() << endl;

    cout << "s2 name: " << s2->get_name() << endl;
    cout << "s2 is label: " << s2->is_label() << endl;
	*/

    return 0;
}

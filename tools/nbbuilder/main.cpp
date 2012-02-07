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
		{"label:l2;","name:y;","label:l1;","name:z;","label:l3"}
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
	fin = BHext.add_step(NULL);
	/*
	for (TraceSet::const_iterator ts_it=traces.begin();
			ts_it!=traces.end();ts_it++) {
		
		fin = BHext.add_step(NULL);
		BHext.add_path(ts_it->begin(),ts_it->end(),root,fin);
	}
	*/
	/*
	vector<Trace::const_iterator> label_it;
	for (TraceSet::const_iterator it=traces.begin();
			it!=traces.end();it++) label_it.push_back(it->begin());
	*/
	//fin = BHext.add_step(NULL);
	//BHext.add_traces(traces,root,fin);
	
	// take pairs of iterators for each trace
	// each pair contain iterator for current position in trace
	// and the trace.end()
	vector< trcit_pair_t > tit_vec;
	// store such the pairs in vector
	for (TraceSet::const_iterator it=
		traces.begin();it!=traces.end();it++)
		tit_vec.push_back(trcit_pair_t(it->begin(),it->end()));
	// create 2 layers of label-vertices: prev and current
	// each layer contain information about label name
	// and associated vertex
	map<string, BGVertex> prev_layer, cur_layer;
	// now create first layer of "label vertices" and connect
	// them to "root" vertex 
	prev_layer = create_layer(BHext,tit_vec);
	for (map<string,BGVertex>::iterator lit = 
		prev_layer.begin();lit!=prev_layer.end();lit++)
		BHext.add_edge(root,lit->second);
	
	int layer = 0;
	// create "full" graph
	do {
		cout<<"layer "<<layer<<" labels: ";
		// 0. shift iterators to next position to deal only with
		// "remaining" traces
		shift_iterators(tit_vec);
		// 1. create the next layer of "label vertices"
		cur_layer = create_layer(BHext,tit_vec);
				
		cout<<endl;
		layer++;
		
		// 2. connect each pair of "label vertices" 
		// in previous and current layers with appropriate
		// paths obtained from subt(traces,label1,label2)
		for (map<string,BGVertex>::iterator lit1 = 
			prev_layer.begin();lit1!=prev_layer.end();lit1++)
		for (map<string,BGVertex>::iterator lit2 = 
			cur_layer.begin();lit2!=cur_layer.end();lit2++)
		{
			TraceSet ts = subt(traces,lit1->first,lit2->first);
			if (!ts.empty())
				BHext.add_traces(ts,lit1->second,lit2->second);
		}
		
		prev_layer = cur_layer;
	}
	while (!tit_vec.empty()); 
	
	
	BHext.save_dot("ext.dot",BHext.produce_dot());

    return 0;
}

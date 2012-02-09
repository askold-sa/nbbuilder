#include<algorithms.h>

using namespace boost;

map<string, BGVertex> create_layer(Behavior& BH, 
	vector<trcit_pair_t>& tit_vec) 
{
	map<string, BGVertex> layer;
	
	for (vector<trcit_pair_t>::iterator it = tit_vec.begin();
		it!=tit_vec.end();it++)
	{
		// get next label (via trace iterator)
		Trace::const_iterator cur = 
			get_next_label(it->first,it->second);
		// set iterator in vector to this label
		it->first = cur;
		
		// check if label was found
		if (cur != it->second) 
		{	
			string label_name = (*cur)->get_name();

			// check if "label vertex" with given name already exists
			// in layer or not, if not - add it to layer
			if (layer.find(label_name) == layer.end())
			{
				BGVertex lv = BH.add_step(*cur);
				layer.insert(pair<string,BGVertex>
					(label_name,lv));
			}
		}
	}
	return layer;
}

void shift_iterators(vector<trcit_pair_t>& vec)
{
	vector<trcit_pair_t> tmp_vec;
	for (vector<trcit_pair_t>::iterator it = 
		vec.begin();it!=vec.end();it++)
		if (it->first != it->second && ++(it->first) != it->second) 
			tmp_vec.push_back(trcit_pair_t(it->first,it->second));
	vec = tmp_vec;
}

void link_layers(Behavior& BH, const TraceSet& traces,
	const map<string, BGVertex>& prev, 
	const map<string, BGVertex>& cur) 
{
	for (map<string,BGVertex>::const_iterator lit1 = 
			prev.begin();lit1!=prev.end();lit1++)
	for (map<string,BGVertex>::const_iterator lit2 = 
			cur.begin();lit2!=cur.end();lit2++)
	{
		TraceSet ts = subt(traces,lit1->first,lit2->first);
		if (!ts.empty())
			BH.add_traces(ts,lit1->second,lit2->second);
	}
}

Behavior make_full_bh(const TraceSet& traces)
{
	Behavior BH;
	
	BGVertex root,fin;
	root = BH.add_step(NULL);
	BH.set_root(root);
	//fin = BH.add_step(NULL);
	
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
	prev_layer = create_layer(BH,tit_vec);
	for (map<string,BGVertex>::iterator lit = 
		prev_layer.begin();lit!=prev_layer.end();lit++)
		BH.add_edge(root,lit->second);

	// create "full" graph
	do {
		// 0. shift iterators to next position to deal only with
		// "remaining" traces
		shift_iterators(tit_vec);
		// 1. create the next layer of "label vertices"
		cur_layer = create_layer(BH,tit_vec);
		
		// 2. connect each pair of "label vertices" 
		// in previous and current layers with appropriate paths
		// Paths are obtained from subt(traces,label1,label2)
		link_layers(BH,traces,prev_layer,cur_layer);
		
		prev_layer = cur_layer;
	}
	while (!tit_vec.empty()); 
	
	return BH;
}

// recursive depth-first handling all paths in Behavior graph
// BHorig - original behavior graph
// cur_v - current vertex in original graph
// lo_vec - vector of lorders
// BHnew - new behavior graph containign only paths with correct
// lorders from lo_vec
// trace - prefix of trace, trace corresponds the path
// from "root" of BHorig to current vertex cur_v, including cur_v
// fin - dummy vertex: paths are insertet between "root" and "fin"
// ALL ACTUAL modification of BHnew is done in non-recursive
// state of function
// The recursive state ONLY do graph traversal and prepare trace
void handle_path(const Behavior& BHorig, const BGVertex& cur_v,
	const vector<LOrder>& lo_vec, 
	Behavior& BHnew, const Trace& trace, BGVertex fin)
{
	// get out_edges of cur_v
	BGOutEdgeIt out_i, out_end;
	BGEdge e;
	tie(out_i,out_end) = BHorig.get_out_edges(cur_v);
	if (out_i!=out_end) // recursively handle all child vertices
		for (;out_i!=out_end;out_i++)
		{
			e = *out_i;
			// get next vertex
			BGVertex v = BHorig.get_target(e);
			// add new Step for given vertex to new trace
			Trace t = trace;
			t.push_back(BHorig.get_step(v));
			// recursive call for this func with new params
			handle_path(BHorig,v,lo_vec,BHnew,t,fin);
		}
	else // if there are no out edges - then no recursion
	{
		// now we have full trace
		// check if its lorder correct 
		LOrder lo = lorder(trace);
		bool need_add = false;
		for (vector<LOrder>::const_iterator lo_it=
			lo_vec.begin();lo_it!=lo_vec.end();lo_it++)
			if (lorder_is_equal(*lo_it,lo)) {
				need_add = true;
				break;
			}
		// and if so, add path for this trace in BHnew
		if (need_add) 
			BHnew.add_path(trace.begin(),trace.end(),
				BHnew.get_root(),fin);
	}
	return ;
}

Behavior make_full_with_clo (const Behavior& BHfull, 
	const vector<LOrder>& lorder_vec)
{
	Behavior BH;
	BGVertex root,fin;
	root = BH.add_step(NULL);
	BH.set_root(root);
	fin = BH.add_step(NULL);
	Trace trace;
	
	handle_path(BHfull,root,lorder_vec,BH,trace,fin);
	
	return BH;
}

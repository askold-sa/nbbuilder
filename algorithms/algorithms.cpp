#include<algorithms.h>

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
	fin = BH.add_step(NULL);
	
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

Behavior make_full_with_clo (const Behavior& BHfull, 
	const vector<LOrder>& lorder_vec)
{
	Behavior BH;
	
	
	return BH;
}

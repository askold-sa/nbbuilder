#include<algorithms.h>

using namespace boost;

/////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////
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

/////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////

// recursive depth-first handling all paths in Behavior graph
// BHorig - original behavior graph
// cur_v - current vertex in original graph
// lo_vec - vector of lorders
// BHnew - new behavior graph containign only paths with correct
// lorders from lo_vec
// trace - prefix of trace, trace corresponds the path
// from "root" of BHorig to current vertex cur_v, including cur_v
// return - true if vertex cur_v was added to BHnew, i.e. 
// iff path has correct lorder
bool handle_path(const Behavior& BHorig, const BGVertex& cur_v,
	const vector<LOrder>& lo_vec, 
	Behavior& BHnew, const Trace& trace, BGVertex& new_v)
{
	//cout<<"prefix trace: ";debugPrint(trace);cout<<"\n";
	// get out_edges of cur_v
	BGOutEdgeIt out_i, out_end;
	BGEdge e;
	tie(out_i,out_end) = BHorig.get_out_edges(cur_v);
	if (out_i==out_end) // if there are no out edges - no recursion 
	{
		//cout<<"no recursion for: ";debugPrint(trace);cout<<"\n";
		// now we have full trace
		// check if its lorder correct 
		// and if so, add vertex for Step corresponding cur_v
		if (lorder_in_set(lorder(trace),lo_vec)) {
			new_v = BHnew.add_step( BHorig.get_step(cur_v) );
			return true;
		} else return false;
			//BHnew.add_path(trace.begin(),trace.end(),
			//	BHnew.get_root(),fin);
	} else // recursively handle all child vertices
	{
		// flag to check if some path was added for any child vertex
		bool was_added = false;
		vector<BGVertex> new_vvec;
		
		for (;out_i!=out_end;out_i++)
		{
			e = *out_i;
			// get next child vertex
			BGVertex child_v = BHorig.get_target(e);
			// add new Step for given vertex to new trace
			Trace t = trace;
			t.push_back(BHorig.get_step(child_v));
			//cout<<"form new trace: ";debugPrint(t);cout<<"\n";
			// recursive call for this func with new params
			// if for given edge child_v vertex new_v was added
			// then store this new_v in vector and set the flag
			BGVertex new_v;
			if (handle_path(BHorig,child_v,lo_vec,BHnew,t,new_v)) {
				was_added = true;
				new_vvec.push_back(new_v);
			}
		}
		// if at least one path was added, then 
		if (was_added) {
			// create vertex for current Step
			BGVertex this_v;
			this_v = BHnew.add_step(BHorig.get_step(cur_v));
			// connect it to all recursively added child vertices
			for (vector<BGVertex>::const_iterator vvec_it = 
				new_vvec.begin();vvec_it!=new_vvec.end();vvec_it++)
				BHnew.add_edge(this_v,*vvec_it);
			// set this vertex as a new_v for our parent
			new_v = this_v;
		}
		return was_added;
	}
}

Behavior make_full_with_clo (const Behavior& BHfull, 
	const vector<LOrder>& lorder_vec)
{
	Behavior BH;
	BGVertex root;
	Trace trace;
	
	if (handle_path(BHfull,BHfull.get_root(),lorder_vec,BH,trace,root))
		BH.set_root(root);
	
	return BH;
}

/////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////
// obtain child vertices for given vertex set
vector<BGVertex> obtain_childs(const Behavior& BH, 
	const vector<BGVertex> vvec)
{
	vector<BGVertex> childs;
	
	BGOutEdgeIt out_i, out_end;
	BGEdge e;
	for (vector<BGVertex>::const_iterator vit =
		vvec.begin();vit!=vvec.end();vit++) {
	
		tie(out_i,out_end) = BH.get_out_edges(*vit);
		for (;out_i!=out_end;out_i++)
		{
			e = *out_i;
			BGVertex child_v = BH.get_target(e);
			childs.push_back(child_v);
		}
	}
	return childs;
}

// store vector of vertices marked with given step
typedef struct StepChildVertices {
	Step* sp;
	vector<BGVertex> vvec;
	StepChildVertices(Step* step) : sp(step) {}
};

// for given vertex, obtain Step and undate StepChildVertices vector
void add2SChVV (vector<StepChildVertices>& storage, 
	const Behavior& BHorig, const BGVertex v)
{
	Step* step = BHorig.get_step(v);
	// check if entry for given Step already exists in vector
	for (vector<StepChildVertices>::iterator it=
		storage.begin();it!=storage.end();it++)
		if (*step == *((*it).sp)) // check if Steps are equal
		{
			// if so, store given vertex (marked with given Step) 
			it->vvec.push_back(v);
			return ;
		}
	// if entry doen't exist, create and add it to storage
	StepChildVertices entry = StepChildVertices(step);
	entry.vvec.push_back(v);
	storage.push_back(entry);
	return ;
}

/*
 * For given cur_v in BHnew create unique childs, add them to BHnew 
 * and connect them all to cur_v
 * Then call this func for each childs vertices
 * Unique childs mean what for given cur_v and any Step exists 
 * only one (or zero) child vertex marked with this Step symbol 
 */
void create_childs_for_min(const Behavior& BHorig, Behavior& BHnew,
	BGVertex cur_v, const vector<BGVertex>& childs)
{
	if (childs.empty()) // no recursion, do nothing
		return ;
	else // main logic, do all work here 
	{
		// iterate all child vertices, obtain set of unique Steps
		// with corresponding vertices
		vector<StepChildVertices> storage;
		for (vector<BGVertex>::const_iterator it=
			childs.begin();it!=childs.end();it++)
			add2SChVV(storage,BHorig,*it);
		// now for each "child Step" create vertex, add it to BHnew
		// and connect it to cur_v 
		for (vector<StepChildVertices>::const_iterator sit=
			storage.begin();sit!=storage.end();sit++)
		{
			BGVertex new_v = BHnew.add_step(sit->sp);
			BHnew.add_edge(cur_v,new_v);
			
			vector<BGVertex> new_childs;
			new_childs = obtain_childs(BHorig,sit->vvec);
			
			// finally, recursion 
			create_childs_for_min(BHorig,BHnew,new_v,new_childs);
		}
		return ;
	}
}

Behavior make_minimized (const Behavior& BHorig)
{
	Behavior BH;
	BGVertex root;
	root = BH.add_step(NULL);
	BH.set_root(root);
	
	vector<BGVertex> parents;
	parents.push_back(BHorig.get_root());
	
	create_childs_for_min(BHorig,BH,root,obtain_childs(BHorig,parents));
	
	return BH;
}

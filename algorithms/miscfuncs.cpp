#include<miscfuncs.h>

LOrder lorder(const Trace& trace)
{
    LOrder lorder_;
    
    for (Trace::const_iterator it=trace.begin();it!=trace.end();it++) {
		if ( (*it)->is_label() )
			lorder_.push_back( (*it) );
	}
    
    return lorder_;
}

TraceSet subt(const TraceSet& trace_set, string l1, string l2)
{
    TraceSet traces_;
    
    // handle each trace in trace set
    TraceSet::const_iterator it_ts;
    for (it_ts=trace_set.begin();it_ts!=trace_set.end();it_ts++) {
		// for each trace find all subtraces between
		// labels with names l1 and l2
		// 1. set to iterators to the begin of trace
		Trace::const_iterator it_l1,it_l2;
		it_l1 = it_l2 = (*it_ts).begin();
		
		while (it_l1!=(*it_ts).end() && it_l2!=(*it_ts).end()) {
			
			if (!((*it_l1)->is_label() &&
				(*it_l1)->get_name() == l1)) 
			{
				// 2. if it_l1 doesn't point to label step
				// with name "l1", take next step of trace
				it_l1++;
			} else 
			{
				// 3. now it_l1 points to label with name "l1"
				// so we make it_l2 to point to the next label
				it_l2 = it_l1+1;
				while(it_l2!=(*it_ts).end() && !(*it_l2)->is_label())
					it_l2++;
				// if it_l2 points to end of trace - the trace is over,
				// no subtrace found
				if (it_l2==(*it_ts).end())
					break;
				// if it_l2 points to label, check if this label name
				// is "l2" and if so, extract and save subtrace
				// between it_l1 and it_l2
				if ((*it_l2)->get_name()==l2) {
					Trace trace_(it_l1+1,it_l2);
					traces_.push_back(trace_);
				}
				// 4. anyway, set the first iterator to the next label
				// and continue
				it_l1 = it_l2;
			}
		}	
	}
    
    return traces_;
}

Trace::const_iterator get_next_label(Trace::const_iterator it, 
	Trace::const_iterator it_e) 
{
	while ( it != it_e && !(*it)->is_label() ) it++;
	return it;
}

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

void debugPrint(const Trace& trace) 
{
	for (Trace::const_iterator it=trace.begin();
		it!=trace.end();it++)
		cout<<(*it)->get_visname()<<" ";
}

void debugPrint(const TraceSet& trace_set) 
{
	cout<<"Trace count: "<<trace_set.size()<<"\n";
	for (TraceSet::const_iterator it=trace_set.begin();
		it!=trace_set.end();it++) 
	{
		debugPrint(*it);
		cout << "\n";
	}
}

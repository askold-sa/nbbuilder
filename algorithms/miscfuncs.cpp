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

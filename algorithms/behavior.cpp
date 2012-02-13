#include<behavior.h>

#include<iostream>
#include<fstream>
#include<sstream>

using namespace boost;
using namespace std;

Behavior::Behavior() {
	cur_index_ = 0;
}

Behavior::~Behavior() {
}

BGVertex Behavior::add_step(Step* step) {
	
	BGVertex vertex;
	vertex = add_vertex( bg_ );
	
	BGProperty p = BGProperty(step,cur_index_++);
	props_.insert(pair<BGVertex,BGProperty> (vertex,p) );
	
	return vertex;
}

Step* Behavior::get_step(BGVertex vertex) const 
{	
	Step *step = NULL;
	BGProperties::const_iterator it = props_.find(vertex);
	if (it!=props_.end())
		step = (it->second).step;
	
	return step;
}

void Behavior::remove_step(BGVertex vertex)
{
	BGProperties::iterator it = props_.find(vertex);		
	
	props_.erase(it);
	clear_vertex(vertex,bg_);
	remove_vertex(vertex,bg_);
}

int Behavior::get_index(BGVertex vertex) const 
{
	int index = -1;
	BGProperties::const_iterator it = props_.find(vertex);
	if (it!=props_.end())
		index = (it->second).index;
	
	return index;
}

void Behavior::add_edge(BGVertex v, BGVertex u) {
	boost::add_edge(v,u,bg_);
}

void Behavior::remove_edge(BGVertex v, BGVertex u) {
	boost::remove_edge(v,u,bg_);
}

pair<BGOutEdgeIt,BGOutEdgeIt> Behavior::get_out_edges(const BGVertex v)
	const
{
	return out_edges(v,bg_);
}

BGVertex Behavior::get_target(const BGEdge e) const
{
	return target(e,bg_);
}

BGVertex Behavior::get_source(const BGEdge e) const
{
	return source(e,bg_);
}

void Behavior::set_root(BGVertex v) {
	root_ = v;
}

BGVertex Behavior::get_root() const {
	return root_;
}

void Behavior::add_path(Trace::const_iterator it1, 
	Trace::const_iterator it2, BGVertex v, BGVertex u) 
{
	BGVertex v1,v2;
	
	v1 = v;
	while (it1 != it2) {
		v2 = add_step(*it1);
		add_edge(v1,v2);
		v1 = v2;
		it1++;
	}
	add_edge(v1,u);
}

void Behavior::add_traces(const TraceSet& ts,BGVertex v, BGVertex u) 
{
	for (TraceSet::const_iterator it=ts.begin();
		it!=ts.end();it++) 
		add_path(it->begin(),it->end(),v,u);
}

string Behavior::produce_dot() const 
{
	// set global graph params here
	stringstream dot;
	dot << "digraph structs {\n  node[fontsize="
		<< VIS_FONTSIZE << "];\n\n";
	
	// add nodes
	//property_map<BehaviorGraph,vertex_index_t>::type 
	//	index = get(vertex_index, bg_);

	std::pair<BGVertexIter,BGVertexIter> vp;
	for (vp = vertices(bg_);vp.first != vp.second; ++vp.first)
	{
		stringstream new_node;
		int index = -1;
		Step* sp = NULL;
		BGProperties::const_iterator it = props_.find(*vp.first);
		if (it!=props_.end()) {
			index = (it->second).index;
			sp = (it->second).step;
		}
		
		if (*vp.first == root_)
			new_node << "  "<< index <<
				"[shape="<<VIS_ROOT_SHAPE <<
				",fontcolor="<<VIS_ROOT_COLOR<<
				",label="""<<VIS_ROOT_MSG<<"""];";
		else 
		{			
			if (!sp) // pointer to NULL
				new_node <<"  "<< index <<
					"[shape="<<VIS_NULL_SHAPE <<
					",fontcolor="<<VIS_NULL_COLOR <<
					",label="""<<VIS_NULL_MSG<<"""];";
			else // set appropriate shape and color, set visname
				new_node<<"  "<< index <<
					"[shape=" <<
					(sp->is_label() ? VIS_LABEL_SHAPE : VIS_STEP_SHAPE)
					<<",fontcolor=" <<
					(sp->is_label() ? VIS_LABEL_COLOR : VIS_STEP_COLOR)
					<<",label="""<<sp->get_visname()<<"""];";	
		}
			
		dot << new_node.str() << "\n";
	}
	dot << "\n";
	// add edges
	graph_traits<BehaviorGraph>::edge_iterator ei,ei_end;
	for (tie(ei,ei_end) = edges(bg_);ei!=ei_end;++ei)
	{
		int index1 = -1, index2 = -1;
		BGProperties::const_iterator it1 = 
			props_.find(source(*ei,bg_));
		if (it1!=props_.end()) index1 = (it1->second).index;
		BGProperties::const_iterator it2 = 
			props_.find(target(*ei,bg_));
		if (it2!=props_.end()) index2 = (it2->second).index;
		
		dot <<"  "<<index1<<"->"<<index2<<";\n";
	}
	
	// end of graph
	dot << "\n}";
	return dot.str();
}

void Behavior::save_dot(const string filename, const string &dot) const 
{
	fstream file;
	
	file.open(filename.c_str(),ios::out);
	
	if (! file.is_open() ) {
		cout<< "ERROR open the file "<<filename
			<<" to save graph layout!!";
		return; 
	} 
	
	file << dot;
	
	file.close();
}

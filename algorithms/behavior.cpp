#include<behavior.h>

#include<iostream>
#include<fstream>
#include<sstream>

using namespace boost;
using namespace std;

Behavior::Behavior() {
}

Behavior::~Behavior() {
}

BGVertex Behavior::add_step(Step* step) {
	
	BGVertex vertex;
	vertex = add_vertex( bg_ );
	
	props_.push_back( step );
	
	return vertex;
}

Step* Behavior::get_step(BGVertex vertex) {
	
	property_map<BehaviorGraph,vertex_index_t>::type
		index = get(vertex_index, bg_);
	
	Step *step = NULL;
	
	try 
	{
		step = props_.at(index[vertex]);
	} catch (out_of_range&)
	{
	}
	
	return step;
}

void Behavior::add_edge(BGVertex v, BGVertex u) {
	boost::add_edge(v,u,bg_);
}

void Behavior::remove_edge(BGVertex v, BGVertex u) {
	boost::remove_edge(v,u,bg_);
}

void Behavior::set_root(BGVertex v) {
	root_ = v;
}

BGVertex Behavior::get_root() {
	return root_;
}


void Behavior::debugPrint() {
	
	property_map<BehaviorGraph,vertex_index_t>::type 
		index = get(vertex_index, bg_);
	
	cout<<" vertices(Behavior)= ";
	std::pair<BGVertexIter,BGVertexIter> vp;
	for (vp = vertices(bg_);vp.first != vp.second; ++vp.first)
		cout<<index[*vp.first]<<"("
			<< (*vp.first==root_ ? "root" : 
				(props_[*vp.first] ? props_[*vp.first]->get_name()
					: "NULL")
				) <<") ";
	cout<<endl;
	cout<<"edges(Behavior)= ";
	graph_traits<BehaviorGraph>::edge_iterator ei,ei_end;
	for (tie(ei,ei_end) = edges(bg_);ei!=ei_end;++ei)
		cout <<"("<<index[source(*ei,bg_)]
			<<","<<index[target(*ei,bg_)]<<") ";
	cout<<endl;
}

string Behavior::produce_dot() {

	// set global graph params here
	stringstream dot;
	dot << "digraph structs {\n  node[fontsize="
		<< VIS_FONTSIZE << "];\n\n";
	
	// add nodes
	property_map<BehaviorGraph,vertex_index_t>::type 
		index = get(vertex_index, bg_);

	std::pair<BGVertexIter,BGVertexIter> vp;
	for (vp = vertices(bg_);vp.first != vp.second; ++vp.first)
	{
		stringstream new_node;
		
		if (*vp.first == root_)
			new_node << "  "<<index[*vp.first] <<
				"[shape="<<VIS_ROOT_SHAPE <<
				",fontcolor="<<VIS_ROOT_COLOR<<
				",label="""<<VIS_ROOT_MSG<<"""];";
		else 
		{
			Step *sp = props_[*vp.first];
			
			if (!sp) // pointer to NULL, no step, error
				new_node <<"  "<<index[*vp.first] <<
					"[shape="<<VIS_ERROR_SHAPE <<
					",fontcolor="<<VIS_ERROR_COLOR <<
					",label="""<<VIS_ERROR_MSG<<"""];";
			else // set appropriate shape and color, set visname
				new_node<<"  "<<index[*vp.first] <<
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
		stringstream new_edge;
		new_edge <<"  "<<index[source(*ei,bg_)]
			<<"->"<<index[target(*ei,bg_)]<<";";
		
		dot << new_edge.str() <<"\n";
	}
	
	// end of graph
	dot << "\n}";
	return dot.str();
}

void Behavior::save_dot(const string filename, const string &dot) {

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

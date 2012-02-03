#include<behavior.h>

using namespace boost;

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

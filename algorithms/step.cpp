#include<step.h>
#include<boost/regex.hpp>

using namespace std;

//////////////////////////////////////////////////////////////////////////////

StepImpl::StepImpl() {
    init();
}

StepImpl::StepImpl(string init_str) {
    init_str_ = init_str;
    init();
}

StepImpl::~StepImpl() {
}

void StepImpl::set_init(string init_str) {
    init_str_ = init_str;
}

void StepImpl::set_name(string name) {
    name_ = name;
}

string StepImpl::get_name() {
    return name_;
}

void StepImpl::label_on() {
    flabel_ = true;
}

void StepImpl::label_off() {
    flabel_ = false;
}

bool StepImpl::is_label() {
    return flabel_;
}

void StepImpl::set_visname(string visname) {
    visname_ = visname;
}

string StepImpl::get_visname() {
    return visname_;
}

void StepImpl::set_add_data(string add_data) {
    add_data_ = add_data;
}

string StepImpl::get_add_data() {
    return add_data_;
}

InitResult StepImpl::init() {
    
    static boost::regex simpleStep_ex ("name:([\\d\\l\\u_]+);[ ]*(add-data:(.*);){0,1}[ ]*");
    static boost::regex labelStep_ex ("label:([\\d\\l\\u_]+);[ ]*(add-data:(.*);){0,1}[ ]*");
    boost::smatch res;

    if (regex_search( init_str_, res, labelStep_ex )) 
    {
        name_ = res[1];
        visname_ = name_;
        flabel_ = true;
        add_data_ = res[3];

        return OK;
    } 
    else if (regex_search( init_str_, res, simpleStep_ex  )) 
    {
        name_ = res[1];
        visname_ = name_;
        flabel_ = false;
        add_data_ = res[3];

        return OK;
    }
        
    return ERROR;
}


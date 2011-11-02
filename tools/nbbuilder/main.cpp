#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>

#include<iostream>
#include<step.h>

using namespace std;

int main ( int argc, char **argv ) {

    Step *s1 = new StepImpl("dglkhgh name:stepA;   ");
    Step *s2 = new StepImpl("jfGHFGA;label:L1;   add-data: Gosznak;   ");

    cout << "s1 name: " << s1->get_name() << endl;
    cout << "s1 is label: " << s1->is_label() << endl;

    cout << "s2 name: " << s2->get_name() << endl;
    cout << "s2 is label: " << s2->is_label() << endl;

    return 0;
}

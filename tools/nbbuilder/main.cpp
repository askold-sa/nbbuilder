#include<iostream>
#include<step.h>

using namespace std;

int main ( int argc, char **argv ) {

    Step *s1 = new StepImpl("name:stepA;   ");
    Step *s2 = new StepImpl("label:L1;   add-data: Gosznak;   ");

    cout << "s1 name: " << s1->get_name() << endl;
    cout << "s1 is label: " << s1->is_label() << endl;
    cout << "s1 add_data: " << s1->get_add_data() << endl;

    cout << "s2 name: " << s2->get_name() << endl;
    cout << "s2 is label: " << s2->is_label() << endl;
    cout << "s2 add_data: " << s2->get_add_data() << endl;


    return 0;
}

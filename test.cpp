#include "json.h"
#include <iostream>

using namespace std;

int main() {
	string input = "[[1,2,3,4,\"ewqe\"],[],[],[]]";

	json* test1 = new json(input);

	cout
		<< test1->convert_vector()->at(0)->convert_vector()->at(0)->get_int() << ' '
		<< test1->convert_vector()->at(0)->convert_vector()->at(1)->get_int() << ' '
		<< test1->convert_vector()->at(0)->convert_vector()->at(2)->get_int() << ' '
		<< test1->convert_vector()->at(0)->convert_vector()->at(3)->get_int() << ' '
		<< test1->convert_vector()->at(0)->convert_vector()->at(4)->get_string() << endl;

	input = "{ \"name\" : \"runoob\" , \"alexa\":10000,\"sites\":{\"site1\":\"www.runoob.com\",\"site2\":\"m.runoob.com\",\"site3\" : \"c.runoob.com\" } }";

	json* test2 = new json(input);
	cout << (*test2->convert_object())["name"]->get_string() << endl;

	cout << test2->serialize() << endl;

	input = "{\"value\":[14,234,53,32,134]}";

	json* test3 = new json(input);
	for (int i = 0; i < 5; ++i) {
		cout << (*(*test3->convert_object())["value"]->convert_vector())[i]->get_int() << ' ';
	}
	cout << endl;

	input = "[true, true, false]";

	json* test4 = new json(input);

	cout << test4->serialize() << endl;

	delete(test1);
	delete(test2);
	delete(test3);
	delete(test4);
}
#include <iostream>
#include "json.h"

#define output

using namespace std;

int main() {
	json* test1 = new json("[[-1 ,true,-3, false,\"er\\\"\\\\\\\"ewqe\"], [{\"id\":123456 , \"name\" : \"Jack\"}, {\"strings\"  :{\"string1\": \"I am 1\", \"string2\" :\"I am 2\", \"string3\" :\"I am 3\"  } } ]]");
	json* test2 = new json("{ \"name\" : null , \"alexa\":10000,\"sites\":{\"site1\":\"www.runoob.com\",\"site2\":\"m.runoob.com\",\"site3\" : \"c.runoob.com\" } }");
	json* test3 = new json("{ \"send\\\"_id\":10001,\"send_name\":\"Pei\\\"nan\"}");
	json* test4 = new json("[null, true, false, null, 3, null, [null, [], null]]");

#ifdef output
	cout
		<< test1->convert_vector()->at(0)->convert_vector()->at(0)->get_int() << ' '
		<< test1->convert_vector()->at(0)->convert_vector()->at(1)->get_bool() << ' '
		<< test1->convert_vector()->at(0)->convert_vector()->at(2)->get_int() << ' '
		<< test1->convert_vector()->at(0)->convert_vector()->at(3)->get_bool() << ' '
		<< test1->convert_vector()->at(0)->convert_vector()->at(4)->get_string() << ' '
		<< test1->convert_vector()->at(1)->convert_vector()->at(0)->convert_object()->at("id")->get_int() << ' '
		<< test1->convert_vector()->at(1)->convert_vector()->at(0)->convert_object()->at("name")->get_string() << ' '
		<< test1->convert_vector()->at(1)->convert_vector()->at(1)->convert_object()->at("strings")->convert_object()->at("string1")->get_string() << ' '
		<< test1->convert_vector()->at(1)->convert_vector()->at(1)->convert_object()->at("strings")->convert_object()->at("string2")->get_string() << ' '
		<< test1->convert_vector()->at(1)->convert_vector()->at(1)->convert_object()->at("strings")->convert_object()->at("string3")->get_string() << endl;

	cout << test1->serialize() << endl;
	cout << test2->serialize() << endl;
	cout << test3->serialize() << endl;
	cout << test4->serialize() << endl;
#endif

	delete(test1);
	delete(test2);
	delete(test3);
	delete(test4);
}
#include <iostream>
#include "json.h"

#define output

using namespace std;

int main() {
#ifndef output
	while (true)
#endif
	{
		json test1 = json::create("[[-1 ,true,-3, false,\"er\\\"\\\\\\\"ewqe\"], [{\"id\":123456 , \"name\" : \"Jack\"}, {\"strings\"  :{\"string1\": \"I am 1\", \"string2\" :\"I am 2\", \"string3\" :\"I am 3\"  } } ]]");
		json test2 = json::create("{ \"name\" : null , \"alexa\":10000,\"sites\":{\"site1\":\"www.runoob.com\",\"site2\":\"m.runoob.com\",\"site3\" : \"c.runoob.com\" } }");
		json test3 = json::create("{ \"send\\\"_id\":10001,\"send_name\":\"Pei\\\"nan\"}");
		json test4 = json::create("[null, true, false, null, 3, null, [null, [], null]]");
		json test5 = json(vector<json>{ {1},{1} });
		json test6 = json(unordered_map<string, json>{ { "str1", {1} }, { "str2", {1} }, { "str3", test5 } });
		vector<json> f{ {1},{1} };
		json test7 = json(f);
		unordered_map<string, json> m{ { "str1", { 1 } }, { "str2", {1} }, { "str3", test7 } };
		json test8 = json(m);
		json test9(test8);
		json test10(move(test7));
		test9 = test7;
		test7 = move(test10);

#ifdef output
		cout
			<< test1[0][0].get_int() << ' '
			<< test1[0][1].get_bool() << ' '
			<< test1[0][2].get_int() << ' '
			<< test1[0][3].get_bool() << ' '
			<< test1[0][4].get_string() << ' '
			<< test1[1][0]["id"].get_int() << ' '
			<< test1[1][0]["name"].get_string() << ' '
			<< test1[1][1]["strings"]["string1"].get_string() << ' '
			<< test1[1][1]["strings"]["string2"].get_string() << ' '
			<< test1[1][1]["strings"]["string3"].get_string() << endl;

		cout << test1.serialize() << endl;
		cout << test2.serialize() << endl;
		cout << test3.serialize() << endl;
		cout << test4.serialize() << endl;
		cout << test5.serialize() << endl;
		cout << test6.serialize() << endl;
		cout << test7.serialize() << endl;
		cout << test8.serialize() << endl;
		cout << test9.serialize() << endl;
		cout << test10.serialize() << endl;
#endif
	}

	return 0;
}

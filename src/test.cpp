#include <iostream>
#include <chrono>
#include "json.h"

#define output

using namespace std;
using namespace peinan;
using namespace chrono;

int main() {
    string correctnessTestSet[21] = {
            " { } ",
            " { \"a\" : \"b\" } ",
            " { \"a\" : \"b\" , \"b\" : \"a\"} ",
            " [ ] ",
            " [ 1 ] ",
            " [ 1 , 2 ] ",
            " \"\" ",
            " \" \" ",
            " \"\\u0026\"",
            " -1 ",
            " 0 ",
            "0",
            " 0.1 ",
            " 0.1e1 ",
            " 0.1E1 ",
            " 0.1e+1 ",
            " 0.1e-1 ",
            " 0.1E+1 ",
            " 0.1E-1 ",
            " 01E+1 ",
            " 01E-1 ",
    };

    string CASE_1 = "{ \"field\" : [[-1 ,true,-3, false,\"er\\\"\\\\\\\"ewqe\"], [{\"id\":123456 , \"name\" : \"Jack\"}, {\"strings\"  :{\"string1\": \"I am 1\", \"string2\" :\"I am 2\", \"string3\" :\"I am 3\"  } } ]]}";
    string CASE_2 = "{ \"name\" : null , \"alexa\":10000,\"sites\":{\"site1\":\"www.runoob.com\",\"site2\":\"m.runoob.com\",\"site3\" : \"c.runoob.com\" } }";
    string CASE_3 = "{ \"send\\\"_id\":10001,\"send_name\":\"Pei\\\"nan\"}";
    string CASE_4 = "{ \"field\" : [null, true, false, null, -3, null, [null, [], null]]}";
    string CASE_5 = "{ \"field\": \"\\/Hello\\u0026\\u0026\\u0026\\u0026\\u0026\\u0026\\u0026\\u0026 \\\"World\\\" T\"}";
    string CASE_6 = "{ \"num\" :[0.0e0,12345,0.003,0.002e-5, -0.03405e0, 3]}";
    string FORMATTED_1 = "{\"field\":[[-1,true,-3,false,\"er\\\"\\\\\\\"ewqe\"],[{\"id\":123456,\"name\":\"Jack\"},{\"strings\":{\"string1\":\"I am 1\",\"string2\":\"I am 2\",\"string3\":\"I am 3\"}}]]}";
    string FORMATTED_2 = "{\"name\":null,\"alexa\":10000,\"sites\":{\"site1\":\"www.runoob.com\",\"site2\":\"m.runoob.com\",\"site3\":\"c.runoob.com\"}}";
    string FORMATTED_3 = "{\"send\\\"_id\":10001,\"send_name\":\"Pei\\\"nan\"}";
    string FORMATTED_4 = "{\"field\":[null,true,false,null,-3,null,[null,[],null]]}";
    string FORMATTED_5 = "{\"field\":\"/Hello&&&&&&&& \\\"World\\\" T\"}";

    vector<json> l;
    for (string& str : correctnessTestSet) {
        l.push_back(json::create(str));
    }
    for (json& obj : l) {
        cout << obj.serialize() << endl;
    }

    json test1 = json::create(CASE_1);
    json test2 = json::create(CASE_2);
    json test3 = json::create(CASE_3);
    json test4 = json::create(CASE_4);
    json test5 = json::create(CASE_5);
    json test6 = json::create(CASE_6);

    string str1 = test1.serialize();
    string str2 = test2.serialize();
    string str3 = test3.serialize();
    string str4 = test4.serialize();
    string str5 = test5.serialize();
    string str6 = test6.serialize();

    //if (str1 != FORMATTED_1) { throw 1; }
    //if (str2 != FORMATTED_2) { throw 2; }
    //if (str3 != FORMATTED_3) { throw 3; }
    //if (str4 != FORMATTED_4) { throw 4; }
    //if (str5 != FORMATTED_5) { throw 5; }

    auto tp1 = system_clock::now();
    for (int i = 0; i < 1000000; ++i)
    {
        str1 = test1.serialize();
        str2 = test2.serialize();
        str3 = test3.serialize();
        str4 = test4.serialize();
        str5 = test5.serialize();
        //str6 = test6.serialize();
    }
    auto tp2 = system_clock::now();
    cout << (tp2 - tp1).count() << endl;

    auto tp3 = system_clock::now();
    for (int i = 0; i < 1000000; ++i)
    {
        test1 = json::create(CASE_1);
        test2 = json::create(CASE_2);
        test3 = json::create(CASE_3);
        test4 = json::create(CASE_4);
        test5 = json::create(CASE_5);
        //test6 = json::create(CASE_6);
    }
    auto tp4 = system_clock::now();
    cout << (tp4 - tp3).count() << endl;

//#ifndef output
//	while (true)
//#endif
//	{
//		json test1 = json::create("[[-1 ,true,-3, false,\"er\\\"\\\\\\\"ewqe\"], [{\"id\":123456 , \"name\" : \"Jack\"}, {\"strings\"  :{\"string1\": \"I am 1\", \"string2\" :\"I am 2\", \"string3\" :\"I am 3\"  } } ]]");
//		json test2 = json::create("{ \"name\" : null , \"alexa\":10000,\"sites\":{\"site1\":\"www.runoob.com\",\"site2\":\"m.runoob.com\",\"site3\" : \"c.runoob.com\" } }");
//		json test3 = json::create("{ \"send\\\"_id\":10001,\"send_name\":\"Pei\\u0026\\\"nan\"}");
//		json test4 = json::create("[null, true, false, null, 3, null, [null, [], null]]");
//		json test5 = json(vector<json>{ {1},{1} });
//		json test6 = json(unordered_map<string, json>{ { "str1", {1} }, { "str2", {1} }, { "str3", test5 } });
//		vector<json> f{ {1},{1} };
//		json test7 = json(move(f));
//		unordered_map<string, json> m{ { "str1", { 1 } }, { "str2", {1} }, { "str3", test7 } };
//		json test8 = json(m);
//		json test9(test8);
//		json test10(move(test7));
//		test9 = test7;
//		test7 = move(test10);
//
//#ifdef output
//		cout
//			<< test1[0][0].get_int32() << ' '
//			<< test1[0][1].get_bool() << ' '
//			<< test1[0][2].get_int32() << ' '
//			<< test1[0][3].get_bool() << ' '
//			<< test1[0][4].as_string() << ' '
//			<< test1[1][0]["id"].get_int32() << ' '
//			<< test1[1][0]["name"].as_string() << ' '
//			<< test1[1][1]["strings"]["string1"].as_string() << ' '
//			<< test1[1][1]["strings"]["string2"].as_string() << ' '
//			<< test1[1][1]["strings"]["string3"].as_string() << endl;
//
//		cout << test1.serialize() << endl;
//		cout << test2.serialize() << endl;
//		cout << test3.serialize() << endl;
//		cout << test4.serialize() << endl;
//		cout << test5.serialize() << endl;
//		cout << test6.serialize() << endl;
//		cout << test7.serialize() << endl;
//		cout << test8.serialize() << endl;
//		cout << test9.serialize() << endl;
//		cout << test10.serialize() << endl;
//#endif
//	}

    system("pause");
    
    return 0;
}

#include "json_old.h"

#include <iostream>

using namespace std;

string string_reader_old(const string& input, int& index) {
	string ret;

	++index;

	while (input[index] != '"') {
		// Escape Characters
		if (input[index] == '\\') {
			ret.push_back('\\');
			++index;
		}
		ret.push_back(input[index]);
		++index;
	}

	++index;

	return ret;
}

json_old::json_old(int type) {
	this->json_type = type;

	switch (this->json_type)
	{
	case -1:
		break;
	case 0:
		cout << "Create an int json object with default value 0!" << endl;
		this->object = new int(0);
		break;
	case 1:
		cout << "Create a string json object with default value (empty string)!" << endl;
		this->object = new string("");
		break;
	case 2:
		cout << "Create a bool json object with default value (false)!" << endl;
		this->object = new bool(false);
		break;
	case 3:
		this->object = new vector<json_old*>();
		break;
	case 4:
		this->object = new unordered_map<string, json_old*>();
		break;
	default:
		this->json_type = -1;
		break;
	}
}

json_old::json_old(int type, int value) {
	this->json_type = 0;
	this->object = new int(value);
}

json_old::json_old(int type, string value) {
	this->json_type = 1;
	this->object = new string(value);
}

json_old::json_old(bool value) {
	this->json_type = 2;
	this->object = new bool(value);
}

json_old::json_old(int type, void* object) {
	this->json_type = type;
	this->object = object;
}

json_old::json_old(const string& input) {
	int index = 0;

	this->json_type = read_type(input, index);

	switch (this->json_type)
	{
	case -1:
		this->object = nullptr;
	case 0:
		this->object = int_parser(input, index);
		break;
	case 1:
		this->object = string_parser(input, index);
		break;
	case 2:
		this->object = bool_parser(input, index);
		break;
	case 3:
		this->object = array_parser(input, index);
		break;
	case 4:
		this->object = object_parser(input, index);
		break;
	default:
		cout << "Invalid json Input String!" << endl;
		break;
	}
}

json_old::json_old(const char* input) : json_old(string(input)) { }

json_old::json_old(const json_old& that) {
	this->json_type = that.json_type;

	switch (this->json_type)
	{
	case -1:
		this->object = nullptr;
		break;
	case 0:
		this->object = new int(that.get_int());
		break;
	case 1:
		this->object = new string(that.get_string());
		break;
	case 2:
		this->object = new bool(that.get_bool());
		break;
	case 3:
		this->object = new vector<json_old*>();
		for (auto it = that.convert_vector()->begin(); it != that.convert_vector()->end(); ++it) {
			this->convert_vector()->push_back(new json_old(**it));
		}
		break;
	case 4:
		this->object = new unordered_map<string, json_old*>();
		for (auto it = that.convert_object()->begin(); it != that.convert_object()->end(); ++it) {
			this->convert_object()->insert({ it->first, new json_old(*(it->second)) });
		}
		break;
	default:
		this->json_type = -1;
		break;
	}
}

json_old::json_old(json_old&& that) {
	this->json_type = that.json_type;
	this->object = that.object;
	that.json_type = -1;
	that.object = nullptr;
}

int json_old::read_type(const string& input, int& index) {
	while (input[index] == ' ' || input[index] == '\r' || input[index] == '\n' || input[index] == '\t')
		++index;

	switch (input[index])
	{
	case '"':
		return 1;
	case 't':
		return 2;
	case 'f':
		return 2;
	case '[':
		return 3;
	case '{':
		return 4;
	case 'n':
		return -1;
	case '-':
		return 0;
	default:
		return input[index] >= '0' && input[index] <= '9' ? 0 : -1;
	}
}

string json_old::serialize() const {
	string output = "";

	switch (this->json_type)
	{
	case -1:
		output += "null";
		break;
	case 0:
		output += to_string(this->get_int());
		break;
	case 1:
		output += '"' + this->get_string() + '"';
		break;
	case 2:
		output += this->get_bool() ? "true" : "false";
		break;
	case 3: {
		output += '[';
		auto it = this->convert_vector()->begin();
		while (it != this->convert_vector()->end()) {
			output += (*it)->serialize();
			if (++it == this->convert_vector()->end())
				break;
			output += ',';
		}
		output += ']';
		break;
	}
	case 4:
		output += '{';
		auto it = this->convert_object()->begin();
		while (it != this->convert_object()->end()) {
			output += '"' + it->first + '"';
			output += ':';
			output += it->second->serialize();
			if (++it == this->convert_object()->end())
				break;
			output += ',';
		}
		output += '}';
		break;
	}

	return output;
}

json_old::~json_old() {
	switch (this->json_type)
	{
	case -1: {
		break;
	}
	case 0: {
		delete (int*)(this->object);
		break;
	}
	case 1: {
		delete (string*)(this->object);
		break;
	}
	case 2:
		delete (bool*)(this->object);
		break;
	case 3:
		for (auto it = this->convert_vector()->begin(); it != this->convert_vector()->end(); ++it) {
			delete (*it);
			*it = nullptr;
		}
		delete (this->convert_vector());
		break;
	case 4:
		for (auto it = this->convert_object()->begin(); it != this->convert_object()->end(); ++it) {
			delete (it->second);
			it->second = nullptr;
		}
		delete (this->convert_object());
		break;
	}

	this->json_type = -1;
	this->object = nullptr;
}

int json_old::get_type() const {
	return this->json_type;
}

int json_old::get_int() const {
	if (this->json_type != 0) {
		cout << "NOT an int json object, this will return 0!" << endl;
		return 0;
	}

	return *((int*)(this->object));
}

string json_old::get_string() const {
	if (this->json_type != 1) {
		cout << "NOT a string json object, this will return empty string!" << endl;
		return "";
	}

	return *((string*)(this->object));
}

bool json_old::get_bool() const {
	if (this->json_type != 2) {
		cout << "NOT a bool json object, this will return false!" << endl;
		return false;
	}

	return *((bool*)(this->object));
}

// Return Pointer
vector<json_old*>* json_old::convert_vector() const {
	if (this->json_type != 3) {
		cout << "NOT an array json object, this will return nullptr!" << endl;
		return nullptr;
	}

	return (vector<json_old*>*)(this->object);
}

// Return Pointer
unordered_map<string, json_old*>* json_old::convert_object() const {
	if (this->json_type != 4) {
		cout << "NOT a object format json object, this will return nullptr!" << endl;
		return nullptr;
	}

	return (unordered_map<string, json_old*>*)(this->object);
}

void* json_old::null_parser(const string& input, int& index) {
	index += 4;

	return nullptr;
}

void* json_old::int_parser(const string& input, int& index) {
	bool neg = input[index] == '-';

	if (neg)
		++index;

	int* p = new int(0);

	while (input[index] <= '9' && input[index] >= '0') {
		*p *= 10;
		*p += input[index] - '0';
		++index;
	}

	if (neg)
		*p *= -1;

	return p;
}

void* json_old::string_parser(const string& input, int& index) {
	string* p = new string();

	++index;

	while (input[index] != '"') {
		// Escape Characters
		if (input[index] == '\\') {
			p->push_back('\\');
			++index;
		}
		p->push_back(input[index]);
		++index;
	}

	++index;

	return p;
}

void* json_old::bool_parser(const string& input, int& index) {
	bool* p = new bool(input[index] == 't' ? true : false);

	index += *p ? 4 : 5;

	return p;
}

void* json_old::array_parser(const string& input, int& index) {
	vector<json_old*>* p = new vector<json_old*>();

	++index;

	while (input[index] != ']') {
		switch (input[index])
		{
		case ' ':
		case '\r':
		case '\n':
		case '\t':
		case ',':
			++index;
			break;
		case '[':
			p->push_back(new json_old(3, this->array_parser(input, index)));
			break;
		case '{':
			p->push_back(new json_old(4, this->object_parser(input, index)));
			break;
		case 't':
			p->push_back(new json_old(2, this->bool_parser(input, index)));
			break;
		case 'f':
			p->push_back(new json_old(2, this->bool_parser(input, index)));
			break;
		case '"':
			p->push_back(new json_old(1, this->string_parser(input, index)));
			break;
		case 'n':
			p->push_back(new json_old(-1, this->null_parser(input, index)));
			break;
		case '-':
			p->push_back(new json_old(0, this->int_parser(input, index)));
			break;
		default:
			p->push_back(new json_old(0, this->int_parser(input, index)));
			break;
		}
	}

	++index;

	return p;
}

void* json_old::object_parser(const string& input, int& index) {
	unordered_map<string, json_old*>* p = new unordered_map<string, json_old*>();

	++index;

	int flag = 0;
	string column = "";
	while (input[index] != '}') {
		if (flag == 1) {
			switch (input[index])
			{
			case ' ':
			case '\r':
			case '\n':
			case '\t':
			case ',':
			case ':':
				++index;
				break;
			case '[':
				p->insert({ column, new json_old(3, this->array_parser(input, index)) });
				flag = 0; column = "";
				break;
			case '{':
				p->insert({ column, new json_old(4, this->object_parser(input, index)) });
				flag = 0; column = "";
				break;
			case 't':
				p->insert({ column, new json_old(2, this->bool_parser(input, index)) });
				flag = 0; column = "";
				break;
			case 'f':
				p->insert({ column, new json_old(2, this->bool_parser(input, index)) });
				flag = 0; column = "";
				break;
			case '"':
				p->insert({ column, new json_old(1, this->string_parser(input, index)) });
				flag = 0; column = "";
				break;
			case 'n':
				p->insert({ column, new json_old(-1, this->null_parser(input, index)) });
				flag = 0; column = "";
				break;
			case '-':
				p->insert({ column, new json_old(0, this->int_parser(input, index)) });
				flag = 0; column = "";
				break;
			default:
				p->insert({ column, new json_old(0, this->int_parser(input, index)) });
				flag = 0; column = "";
				break;
			}
		}
		else {
			if (input[index] == '"') {
				column = string_reader_old(input, index);
				flag = 1;
			}
			else {
				++index;
			}
		}
	}

	++index;

	return p;
}
#ifndef __JSON_H
#define __JSON_H

#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>

using namespace std;

class json {
public:
	json(int type, void* object);
	json(const string& input);
	json(const json& that);
	json(json&& that);
	~json();
	json& operator=(const json& that) = delete;
	json& operator=(json&& that) = delete;
	string serialize() const;

	int get_type() const;
	int get_int() const;
	string get_string() const;
	bool get_bool() const;
	vector<json*>* convert_vector() const;
	unordered_map<string, json*>* convert_object() const;

private:
	// Type = -1, nullptr;
	// Type = 0, number;
	// Type = 1, string;
	// Type = 2, boolean;
	// Type = 3, array;
	// Type = 4, object;
	// Type = 5, null; -> To do
	int json_type = -1;
	void* object = nullptr;

	int read_type(const string& input, int& index);
	void* int_parser(const string& input, int& index);
	void* string_parser(const string& input, int& index);
	void* bool_parser(const string& input, int& index);
	void* array_parser(const string& input, int& index);
	void* object_parser(const string& input, int& index);
};

json::json(int type, void* object) {
	this->json_type = type;
	this->object = object;
}

json::json(const string& input) {
	int index = 0;

	this->json_type = read_type(input, index);

	switch (this->json_type)
	{
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
	}
}

json::json(const json& that) {
	switch (this->json_type)
	{
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
		this->object = new vector<json*>();
		for (auto it = that.convert_vector()->begin(); it != that.convert_vector()->end(); ++it) {
			this->convert_vector()->push_back(new json(**it));
		}
		break;
	case 4:
		this->object = new unordered_map<string, json*>();
		for (auto it = that.convert_object()->begin(); it != that.convert_object()->end(); ++it) {
			this->convert_object()->insert({ it->first, new json(*(it->second)) });
		}
		break;
	}
}

json::json(json&& that) {
	this->json_type = that.json_type;
	this->object = that.object;
	that.json_type = -1;
	that.object = nullptr;
}

int json::read_type(const string& input, int& index) {
	while (input[index] == ' ' || input[index] == '\r' || input[index] == '\n' || input[index] == '\t')
		++index;

	switch (input[index])
	{
	case '\"':
		return 1;
	case 't':
		return 2;
	case 'f':
		return 2;
	case '[':
		return 3;
	case '{':
		return 4;
	default:
		return input[index] >= '0' && input[index] <= '9' ? 0 : -1;
	}
}

string json::serialize() const {
	string output = "";

	switch (this->json_type)
	{
	case -1:
		return "Undefined JSON Data!";
	case 0:
		output += to_string(this->get_int());
		break;
	case 1:
		output += '\"' + this->get_string() + '\"';
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
			output += '\"' + it->first + '\"';
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

json::~json() {
	switch (this->json_type)
	{
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
		delete(this->convert_object());
		break;
	}

	this->json_type = -1;
	this->object = nullptr;
}

int json::get_type() const {
	return this->json_type;
}

int json::get_int() const {
	return *((int*)(this->object));
}

string json::get_string() const {
	return *((string*)(this->object));
}

bool json::get_bool() const {
	return *((bool*)(this->object));
}

// Return Pointer
vector<json*>* json::convert_vector() const {
	return (vector<json*>*)(this->object);
}

// Return Pointer
unordered_map<string, json*>* json::convert_object() const {
	return (unordered_map<string, json*>*)(this->object);
}

void* json::int_parser(const string& input, int& index) {
	int* p = new int(0);

	while (input[index] <= '9' && input[index] >= '0') {
		*p *= 10;
		*p += input[index] - '0';
		++index;
	}

	return p;
}

void* json::string_parser(const string& input, int& index) {
	string* p = new string();

	++index;

	while (input[index] != '\"') {
		p->push_back(input[index]);
		++index;
	}

	++index;

	return p;
}

void* json::bool_parser(const string& input, int& index) {
	bool* p = new bool(input[index] == 't' ? true : false);

	index += *p ? 4 : 5;

	return p;
}

void* json::array_parser(const string& input, int& index) {
	vector<json*>* p = new vector<json*>();

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
			p->push_back(new json(3, this->array_parser(input, index)));
			break;
		case '{':
			p->push_back(new json(4, this->object_parser(input, index)));
			break;
		case 't':
			p->push_back(new json(2, this->bool_parser(input, index)));
			break;
		case 'f':
			p->push_back(new json(2, this->bool_parser(input, index)));
			break;
		case '\"':
			p->push_back(new json(1, this->string_parser(input, index)));
			break;
		default:
			p->push_back(new json(0, this->int_parser(input, index)));
			break;
		}
	}

	++index;

	return p;
}

void* json::object_parser(const string& input, int& index) {
	unordered_map<string, json*>* p = new unordered_map<string, json*>();

	++index;

	int flag = 0;
	string column = "";
	while (input[index] != '}') {
		if (flag == 2) {
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
				p->insert({ column, new json(3, this->array_parser(input, index)) });
				flag = 0; column = "";
				break;
			case '{':
				p->insert({ column, new json(4, this->object_parser(input, index)) });
				flag = 0; column = "";
				break;
			case 't':
				p->insert({ column, new json(2, this->bool_parser(input, index)) });
				flag = 0; column = "";
				break;
			case 'f"':
				p->insert({ column, new json(2, this->bool_parser(input, index)) });
				flag = 0; column = "";
				break;
			case '\"':
				p->insert({ column, new json(1, this->string_parser(input, index)) });
				flag = 0; column = "";
				break;
			default:
				p->insert({ column, new json(0, this->int_parser(input, index)) });
				flag = 0; column = "";
				break;
			}
		}
		else if (flag == 1) {
			if (input[index] == '\"')
				flag = 2;
			else
				column.push_back(input[index]);
			++index;
		}
		else {
			if (input[index] == '\"')
				flag = 1;
			++index;
		}
	}

	++index;

	return p;
}

#endif // __JSON_H
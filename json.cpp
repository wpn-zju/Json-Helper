#include "json.h"

#include <iostream>

using namespace std;

string string_reader(const string& input, int& index) {
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

json::json() : json_type(value_type::Null), object(nullptr) {

}

json::json(bool value) : json_type(value_type::Boolean), object(new bool(value)) {

}

json::json(int value) : json_type(value_type::Number), object(new int(value)) {

}

json::json(const string& value) : json_type(value_type::String), object(new string(value)) {

};

json::json(string&& value) : json_type(value_type::String), object(new string(move(value))) {

}

json::json(const vector<json>& vec) : json_type(value_type::Array), object(new vector<json>(vec)) {

};

json::json(vector<json>&& vec) : json_type(value_type::Array), object(new vector<json>(move(vec))) {

}

json::json(const unordered_map<string, json>& value) : json_type(value_type::Object), object(new unordered_map<string, json>(value)) {

};

json::json(unordered_map<std::string, json>&& obj) : json_type(value_type::Object), object(new unordered_map<string, json>(move(obj))) {

}

json::json(value_type valueType, void* object) : json_type(valueType), object(object) {

}

json json::create(const string& input) {
	int index = 0;

	switch (read_type(input, index))
	{
	case value_type::Null:
		return {};
	case value_type::Boolean:
		return { value_type::Boolean, bool_parser(input, index) };
	case value_type::Number:
		return { value_type::Number,int_parser(input, index) };
	case value_type::String:
		return { value_type::String,string_parser(input, index) };
	case value_type::Array:
		return { value_type::Array,array_parser(input, index) };
	case value_type::Object:
		return { value_type::Object,object_parser(input, index) };
	case value_type::Invalid:
		cout << "Invalid JSON Input String!" << endl;
		return {};
	}
}

json json::create(const char* input) { return create(string(input)); }

json::json(const json& that) {
	this->json_type = that.json_type == value_type::Invalid ? value_type::Null : that.json_type;

	switch (this->json_type)
	{
	case value_type::Null:
		this->object = nullptr;
		break;
	case value_type::Boolean:
		this->object = new bool(that.get_bool());
		break;
	case value_type::Number:
		this->object = new int(that.get_int());
		break;
	case value_type::String:
		this->object = new string(that.get_string());
		break;
	case value_type::Array:
		this->object = new vector<json>();
		for (auto it = that.convert_vector()->begin(); it != that.convert_vector()->end(); ++it) {
			this->convert_vector()->push_back(*it);
		}
		break;
	case value_type::Object:
		this->object = new unordered_map<string, json>();
		for (auto it = that.convert_object()->begin(); it != that.convert_object()->end(); ++it) {
			this->convert_object()->insert({ it->first, it->second });
		}
		break;
	case value_type::Invalid:
		cout << "Invalid JSON Input String!" << endl;
		break;
	}
}

json::json(json&& that) {
	this->json_type = that.json_type;
	this->object = that.object;
	that.json_type = value_type::Null;
	that.object = nullptr;
}

json& json::operator=(const json& that) {
	this->json_type = that.json_type == value_type::Invalid ? value_type::Null : that.json_type;

	switch (this->json_type)
	{
	case value_type::Null:
		this->object = nullptr;
		break;
	case value_type::Boolean:
		this->object = new int(that.get_int());
		break;
	case value_type::Number:
		this->object = new string(that.get_string());
		break;
	case value_type::String:
		this->object = new bool(that.get_bool());
		break;
	case value_type::Array:
		this->object = new vector<json>();
		for (auto it = that.convert_vector()->begin(); it != that.convert_vector()->end(); ++it) {
			this->convert_vector()->push_back(*it);
		}
		break;
	case value_type::Object:
		this->object = new unordered_map<string, json>();
		for (auto it = that.convert_object()->begin(); it != that.convert_object()->end(); ++it) {
			this->convert_object()->insert({ it->first, it->second });
		}
		break;
	case value_type::Invalid:
		cout << "Invalid JSON Input String!" << endl;
		break;
	}
	return *this;
}

json& json::operator=(json&& that) {
	this->json_type = that.json_type;
	this->object = that.object;
	that.json_type = value_type::Null;
	that.object = nullptr;
	return *this;
}

value_type json::read_type(const string& input, int& index) {
	while (input[index] == ' ' || input[index] == '\r' || input[index] == '\n' || input[index] == '\t')
		++index;

	switch (input[index])
	{
	case '"':
		return value_type::String;
	case 't':
		return value_type::Boolean;
	case 'f':
		return value_type::Boolean;
	case '[':
		return value_type::Array;
	case '{':
		return value_type::Object;
	case 'n':
		return value_type::Null;
	case '-':
		return value_type::Number;
	default:
		return input[index] >= '0' && input[index] <= '9' ? value_type::Number : value_type::Invalid;
	}
}

string json::serialize() const {
	string output = "";

	switch (this->json_type)
	{
	case value_type::Invalid:
	case value_type::Null:
		output += "null";
		break;
	case value_type::Boolean:
		output += this->get_bool() ? "true" : "false";
		break;
	case value_type::Number:
		output += to_string(this->get_int());
		break;
	case value_type::String:
		output += '"' + *this->convert_string() + '"';
		break;
	case value_type::Array: {
		output += '[';
		auto it = this->convert_vector()->begin();
		while (it != this->convert_vector()->end()) {
			output += it->serialize();
			if (++it == this->convert_vector()->end())
				break;
			output += ',';
		}
		output += ']';
		break;
	}
	case value_type::Object: {
		output += '{';
		auto it = this->convert_object()->begin();
		while (it != this->convert_object()->end()) {
			output += '"' + it->first + '"';
			output += ':';
			output += it->second.serialize();
			if (++it == this->convert_object()->end())
				break;
			output += ',';
		}
		output += '}';
		break;
	}
	}

	return output;
}

json::~json() {
	switch (this->json_type)
	{
	case value_type::Invalid:
	case value_type::Null:
		break;
	case value_type::Boolean:
		delete (bool*)(this->object);
		break;
	case value_type::Number:
		delete (int*)(this->object);
		break;
	case value_type::String:
		delete (this->convert_string());
		break;
	case value_type::Array:
		delete (this->convert_vector());
		break;
	case value_type::Object:
		delete (this->convert_object());
		break;
	}

	this->json_type = value_type::Null;
	this->object = nullptr;
}

value_type json::get_type() const {
	return this->json_type;
}

bool json::get_bool() const {
	if (this->json_type != value_type::Boolean) {
		cout << "NOT a bool json object, this will return false!" << endl;
		return false;
	}

	return *((bool*)(this->object));
}

int json::get_int() const {
	if (this->json_type != value_type::Number) {
		cout << "NOT an int json object, this will return 0!" << endl;
		return 0;
	}

	return *((int*)(this->object));
}

// Returning Object - Copy needed
string json::get_string() const {
	if (this->json_type != value_type::String) {
		cout << "NOT a string json object, this will return an empty string!" << endl;
		return {};
	}

	return *((string*)(this->object));
}

// Returning Object - Copy needed
vector<json> json::get_vector() const {
	if (this->json_type != value_type::Array) {
		cout << "NOT an array json object, this will return an empty vector!" << endl;
		return {};
	}

	return *((vector<json>*)(this->object));
}

// Returning Object - Copy needed
unordered_map<string, json> json::get_object() const {
	if (this->json_type != value_type::Object) {
		cout << "NOT a object format json object, this will return en empty unordered_map!" << endl;
		return {};
	}

	return *((unordered_map<string, json>*)(this->object));
}

// Returning Pointer - Copy free
string* json::convert_string() const {
	if (this->json_type != value_type::String) {
		cout << "NOT a string json object, this will return nullptr!" << endl;
		return nullptr;
	}

	return (string*)(this->object);
}

// Returning Pointer - Copy free
vector<json>* json::convert_vector() const {
	if (this->json_type != value_type::Array) {
		cout << "NOT an array json object, this will return nullptr!" << endl;
		return nullptr;
	}

	return (vector<json>*)(this->object);
}

// Returning Pointer - Copy free
unordered_map<string, json>* json::convert_object() const {
	if (this->json_type != value_type::Object) {
		cout << "NOT a object format json object, this will return nullptr!" << endl;
		return nullptr;
	}

	return (unordered_map<string, json>*)(this->object);
}

// Ver 2.0
json& json::operator[](int index) {
	if (this->json_type == value_type::Array) {
		if (index >= 0 && index < this->convert_vector()->size()) {
			return (*this->convert_vector())[index];
		}
		else {
			cout << "ERROR! Will return *this." << endl;
			return *this;
		}
	}
	else {
		cout << "ERROR! Will return *this." << endl;
		return *this;
	}
}

json& json::operator[](const string& index) {
	if (this->json_type == value_type::Object) {
		return (*this->convert_object())[index];
	}
	else {
		cout << "ERROR! Will return *this." << endl;
		return *this;
	}
}

json& json::operator[](string&& index) {
	if (this->json_type == value_type::Object) {
		return (*this->convert_object())[index];
	}
	else {
		cout << "ERROR! Will return *this." << endl;
		return *this;
	}
}

const json& json::operator[](int index) const {
	if (this->json_type == value_type::Array) {
		if (index >= 0 && index < this->convert_vector()->size()) {
			return (*this->convert_vector())[index];
		}
		else {
			cout << "ERROR! Will return *this." << endl;
			return *this;
		}
	}
	else {
		cout << "ERROR! Will return *this." << endl;
		return *this;
	}
}

const json& json::operator[](const string& index) const {
	if (this->json_type == value_type::Object) {
		return (*this->convert_object())[index];
	}
	else {
		cout << "ERROR! Will return *this." << endl;
		return *this;
	}
}

const json& json::operator[](string&& index) const {
	if (this->json_type == value_type::Object) {
		return (*this->convert_object())[index];
	}
	else {
		cout << "ERROR! Will return *this." << endl;
		return *this;
	}
}

void json::push_back(const json& that) {
	if (this->json_type == value_type::Array) {
		this->convert_vector()->push_back(that);
	}
	else {
		// EXCEPTION
		cout << "ERROR! JSON::emplace_back(JSON&&) will return." << endl;
	}
}

void json::emplace_back(json&& that) {
	if (this->json_type == value_type::Array) {
		this->convert_vector()->emplace_back(that);
	}
	else {
		// EXCEPTION
		cout << "ERROR! JSON::emplace_back(JSON&&) will return." << endl;
	}
}

void json::insert(const std::string& key, const json& that) {
	if (this->json_type == value_type::Object) {
		this->convert_object()->insert(pair<string, json>(key, that));
	}
	else {
		// EXCEPTION
		cout << "ERROR! JSON::insert(string&&, JSON&&) will return." << endl;
	}
}

void json::insert(const std::string& key, json&& that) {
	if (this->json_type == value_type::Object) {
		this->convert_object()->insert(pair<string, json>(key, that));
	}
	else {
		// EXCEPTION
		cout << "ERROR! JSON::insert(string&&, JSON&&) will return." << endl;
	}
}

void json::insert(std::string&& key, const json& that) {
	if (this->json_type == value_type::Object) {
		this->convert_object()->insert(pair<string, json>( key, that ));
	}
	else {
		// EXCEPTION
		cout << "ERROR! JSON::insert(string&&, JSON&&) will return." << endl;
	}
}

void json::insert(string&& key, json&& that) {
	if (this->json_type == value_type::Object) {
		this->convert_object()->insert(pair<string, json>( key, that ));
	}
	else {
		// EXCEPTION
		cout << "ERROR! JSON::insert(string&&, JSON&&) will return." << endl;
	}
}

void* json::null_parser(const string& input, int& index) {
	index += 4;

	return nullptr;
}

int* json::int_parser(const string& input, int& index) {
	bool neg = input[index] == '-';

	if (neg)
		++index;

	int *p = new int(0);

	while (input[index] <= '9' && input[index] >= '0') {
		*p *= 10;
		*p += input[index] - '0';
		++index;
	}

	if (neg) *p *= -1;

	return p;
}

bool* json::bool_parser(const string& input, int& index) {
	bool *p = new bool(input[index] == 't');

	index += *p ? 4 : 5;

	return p;
}

string* json::string_parser(const string& input, int& index) {
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

std::vector<json>* json::array_parser(const string& input, int& index) {
	vector<json>* p = new vector<json>();

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
			p->push_back(json(value_type::Array, array_parser(input, index)));
			break;
		case '{':
			p->push_back(json(value_type::Object, object_parser(input, index)));
			break;
		case 't':
			p->push_back(json(value_type::Boolean, bool_parser(input, index)));
			break;
		case 'f':
			p->push_back(json(value_type::Boolean, bool_parser(input, index)));
			break;
		case '"':
			p->push_back(json(value_type::String, string_parser(input, index)));
			break;
		case 'n':
			p->push_back(json(value_type::Null, null_parser(input, index)));
			break;
		case '-':
			p->push_back(json(value_type::Number, int_parser(input, index)));
			break;
		default:
			p->push_back(json(value_type::Number, int_parser(input, index)));
			break;
		}
	}

	++index;

	return p;
}

std::unordered_map<std::string, json>* json::object_parser(const string& input, int& index) {
	unordered_map<string, json>* p = new unordered_map<string, json>();

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
				p->insert({ column, json(value_type::Array, array_parser(input, index)) });
				flag = 0; column = "";
				break;
			case '{':
				p->insert({ column, json(value_type::Object, object_parser(input, index)) });
				flag = 0; column = "";
				break;
			case 't':
				p->insert({ column, json(value_type::Boolean, bool_parser(input, index)) });
				flag = 0; column = "";
				break;
			case 'f':
				p->insert({ column, json(value_type::Boolean, bool_parser(input, index)) });
				flag = 0; column = "";
				break;
			case '"':
				p->insert({ column, json(value_type::String, string_parser(input, index)) });
				flag = 0; column = "";
				break;
			case 'n':
				p->insert({ column, json(value_type::Null, null_parser(input, index)) });
				flag = 0; column = "";
				break;
			case '-':
				p->insert({ column, json(value_type::Number, int_parser(input, index)) });
				flag = 0; column = "";
				break;
			default:
				p->insert({ column, json(value_type::Number, int_parser(input, index)) });
				flag = 0; column = "";
				break;
			}
		}
		else {
			if (input[index] == '"') {
				column = string_reader(input, index);
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
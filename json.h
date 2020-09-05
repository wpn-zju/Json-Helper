#pragma once

#include <string>
#include <vector>
#include <unordered_map>

enum struct value_type;

class json {
public:
	json();
	json(bool value);
	json(int value);
	json(const std::string& value);
	json(std::string&& value);
	json(const std::vector<json>& vec);
	json(std::vector<json>&& vec);
	json(const std::unordered_map<std::string, json>& obj);
	json(std::unordered_map<std::string, json>&& obj);
	json(value_type valueType, void* object);
	json(const json& that);
	json(json&& that);
	json& operator=(const json& that);
	json& operator=(json&& that);
	~json();

	static json create(const std::string& input);
	static json create(const char* input);

	std::string serialize() const;

	value_type get_type() const;

	bool get_bool() const;
	int get_int() const;
	std::string get_string() const;
	std::vector<json> get_vector() const;
	std::unordered_map<std::string, json> get_object() const;
	std::string* convert_string() const;
	std::vector<json>* convert_vector() const;
	std::unordered_map<std::string, json>* convert_object() const;

	// Ver 2.0
	json& operator[](int index);
	json& operator[](const std::string& index);
	json& operator[](std::string&& index);
	const json& operator[](int index) const;
	const json& operator[](const std::string& index) const;
	const json& operator[](std::string&& index) const;

	void push_back(const json& that);
	void emplace_back(json&& that);

	void insert(const std::string& key, const json& that);
	void insert(const std::string& key, json&& that);
	void insert(std::string&& key, const json& that);
	void insert(std::string&& key, json&& that);

private:
	value_type json_type;
	void* object;

	static value_type read_type(const std::string& input, int& index);
	static void* null_parser(const std::string& input, int& index);
	static bool* bool_parser(const std::string& input, int& index);
	static int* int_parser(const std::string& input, int& index);
	static std::string* string_parser(const std::string& input, int& index);
	static std::vector<json>* array_parser(const std::string& input, int& index);
	static std::unordered_map<std::string, json>* object_parser(const std::string& input, int& index);
};

enum struct value_type {
	Null,
	Boolean,
	Number,
	String,
	Array,
	Object,
	Invalid
};
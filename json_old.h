#pragma once

#include <string>
#include <vector>
#include <unordered_map>

class json_old {
public:
	json_old(int type);
	json_old(int type, int value);
	json_old(int type, std::string value);
	json_old(bool value);
	json_old(int type, void* object);
	json_old(const std::string& input);
	json_old(const char* input);
	json_old(const json_old& that);
	json_old(json_old&& that);
	~json_old();
	json_old& operator=(const json_old& that) = delete;
	json_old& operator=(json_old&& that) = delete;
	std::string serialize() const;

	int get_type() const;
	int get_int() const;
	std::string get_string() const;
	bool get_bool() const;
	std::vector<json_old*>* convert_vector() const;
	std::unordered_map<std::string, json_old*>* convert_object() const;

private:
	// Type = -1, nullptr / null;
	// Type = 0, number;
	// Type = 1, string;
	// Type = 2, boolean;
	// Type = 3, array;
	// Type = 4, object;
	int json_type = -1;
	void* object = nullptr;

	int read_type(const std::string& input, int& index);
	void* null_parser(const std::string& input, int& index);
	void* int_parser(const std::string& input, int& index);
	void* string_parser(const std::string& input, int& index);
	void* bool_parser(const std::string& input, int& index);
	void* array_parser(const std::string& input, int& index);
	void* object_parser(const std::string& input, int& index);
};

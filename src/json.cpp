#include "json.h"

#include <algorithm>

using namespace std;

namespace peinan {
	json_parse_exception::json_parse_exception(string message)
		: runtime_error(message) {
	
	}

	number::number(const string& value) : value(value) {

	}

	number::number(string&& value) : value(move(value)) {

	}

	number::number(const number& that) : value(that.value) {
		
	}

	int8_t number::int8_value() const {
		return (int8_t) stol(value);
	}

	int16_t number::int16_value() const {
		return (int16_t)stol(value);
	}

	int32_t number::int32_value() const {
		return stol(value);
	}

	int64_t number::int64_value() const {
		return stoll(value);
	}

	uint8_t number::uint8_value() const {
		return (uint8_t) stoul(value);
	}

	uint16_t number::uint16_value() const {
		return (uint16_t) stoul(value);
	}

	uint32_t number::uint32_value() const {
		return stoul(value);
	}

	uint64_t number::uint64_value() const {
		return stoull(value);
	}

	float number::float_value() const {
		return stof(value);
	}

	double number::double_value() const {
		return stod(value);
	}

	const string& number::as_string() const {
		return value;
	}

	string number::to_string() const {
		return value;
	}

	index::index(size_t length) : value(0), length(length) {

	}

	size_t index::get() {
		return value;
	}

	size_t index::saveGet() {
		if (value >= length) {
			throw json_parse_exception("Index error - Index overflow");
		}
		else {
			return value;
		}
	}

	void index::increment() {
		plus(1);
	}

	void index::plus(size_t addend) {
		if (value + addend > length) {
			throw json_parse_exception("Index error - Index overflow");
		}
		else {
			value += addend;
		}
	}

	json::json() : json_type(value_type::Null), json_object(nullptr) {

	}

	json::json(bool value) : json_type(value_type::Boolean), json_object(make_shared<bool>(value)) {

	}

	json::json(int8_t value) : json_type(value_type::Number), json_object(make_shared<number>(to_string(value))) {

	}

	json::json(int16_t value) : json_type(value_type::Number), json_object(make_shared<number>(to_string(value))) {
		
	}

	json::json(int32_t value) : json_type(value_type::Number), json_object(make_shared<number>(to_string(value))) {
		
	}

	json::json(int64_t value) : json_type(value_type::Number), json_object(make_shared<number>(to_string(value))) {

	}

	json::json(uint8_t value) : json_type(value_type::Number), json_object(make_shared<number>(to_string(value))) {

	}

	json::json(uint16_t value) : json_type(value_type::Number), json_object(make_shared<number>(to_string(value))) {

	}

	json::json(uint32_t value) : json_type(value_type::Number), json_object(make_shared<number>(to_string(value))) {

	}

	json::json(uint64_t value) : json_type(value_type::Number), json_object(make_shared<number>(to_string(value))) {

	}

	json::json(const number& value) : json_type(value_type::Number), json_object(make_shared<number>(value)) {

	}

	json::json(number&& value) : json_type(value_type::Number), json_object(make_shared<number>(move(value))) {
		
	}

	json::json(const string& value) : json_type(value_type::String), json_object(make_shared<string>(value)) {

	};

	json::json(string&& value) : json_type(value_type::String), json_object(make_shared<string>(move(value))) {

	}

	json::json(const vector<json>& vec) : json_type(value_type::Array), json_object(make_shared<vector<json>>(vec)) {

	};

	json::json(vector<json>&& vec) : json_type(value_type::Array), json_object(make_shared<vector<json>>(move(vec))) {

	}

	json::json(const unordered_map<string, json>& obj) : json_type(value_type::Object), json_object(make_shared<unordered_map<string, json>>(obj)) {

	};

	json::json(unordered_map<string, json>&& obj) : json_type(value_type::Object), json_object(make_shared<unordered_map<string, json>>(move(obj))) {

	}

	json::json(value_type valueType, shared_ptr<void> object) : json_type(valueType), json_object(move(object)) {

	}

	json json::create(const string& input) {
		index index(input.size());

		json result(parse_value(input, index));

		if (index.get() == input.size()) {
			return result;
		}
		else {
			throw json_parse_exception(
				build_error_message("Redundant component", input, index.get()));
		}
	}

	json json::parse_value(const string& input, index& index) {
		read_whitespace(input, index);
		json result = parse_value_body(input, index);
		read_whitespace(input, index);
		return result;
	}

	void json::read_whitespace(const string& input, index& index) {
		if (index.get() == input.size()) { return; }
		while (input[index.saveGet()] == ' ' ||
			input[index.saveGet()] == '\n' ||
			input[index.saveGet()] == '\r' ||
			input[index.saveGet()] == '\t') {
			index.increment();
			if (index.get() == input.size()) { return; }
		}
	}

	json json::parse_value_body(const string& input, index& index) {
		switch (input[index.saveGet()])
		{
		case 'n':
			return { value_type::Null, parse_null(input, index) };
		case 't':
			return { value_type::Boolean, parse_boolean(input, index) };
		case 'f':
			return { value_type::Boolean, parse_boolean(input, index) };
		case '-':
			return { value_type::Number, parse_number(input, index) };
		case '"':
			return { value_type::String, parse_string(input, index) };
		case '[':
			return { value_type::Array, parse_array(input, index) };
		case '{':
			return { value_type::Object, parse_object(input, index) };
		default:
			if (input[index.saveGet()] >= '0' && input[index.saveGet()] <= '9') {
				return { value_type::Number, parse_number(input, index) };
			}
			else {
				throw json_parse_exception(
					build_error_message("Read type error", input, index.get()));
			}
		}
	}

	json json::create(const char* input) { return create(string(input)); }

	json::json(const json& that) : json_type(that.json_type) {
		switch (this->json_type)
		{
		case value_type::Null:
			this->json_object = make_shared<nullptr_t>(nullptr);
			break;
		case value_type::Boolean:
			this->json_object = make_shared<bool>(that.get_bool());
			break;
		case value_type::Number:
			this->json_object = make_shared<number>(that.get_number());
			break;
		case value_type::String:
			this->json_object = make_shared<string>(that.get_string());
			break;
		case value_type::Array:
			this->json_object = make_shared<vector<json>>(that.get_vector());
			break;
		case value_type::Object:
			this->json_object = make_shared<unordered_map<string, json>>(that.get_object());
			break;
		}
	}

	json::json(json&& that) noexcept {
		this->json_type = that.json_type;
		this->json_object = that.json_object;
		
		that.json_type = value_type::Null;
		that.json_object = nullptr;
	}

	json& json::operator=(const json& that) {
		this->json_type = that.json_type;

		switch (this->json_type)
		{
		case value_type::Null:
			this->json_object = make_shared<nullptr_t>(nullptr);
			break;
		case value_type::Boolean:
			this->json_object = make_shared<bool>(that.get_bool());
			break;
		case value_type::Number:
			this->json_object = make_shared<number>(that.get_number());
			break;
		case value_type::String:
			this->json_object = make_shared<string>(that.get_string());
			break;
		case value_type::Array:
			this->json_object = make_shared<vector<json>>(that.get_vector());
			break;
		case value_type::Object:
			this->json_object = make_shared<unordered_map<string, json>>(that.get_object());
			break;
		}

		return *this;
	}

	json& json::operator=(json&& that) noexcept {
		this->json_type = that.json_type;
		this->json_object = that.json_object;

		that.json_type = value_type::Null;
		that.json_object = nullptr;

		return *this;
	}

	string json::escape_string(const string& input) {
		string output;

		for (char c : input) {
			switch (c)
			{
			case '"':
				output += "\\\"";
				break;
			case '\\':
				output += "\\\\";
				break;
			case '\b':
				output += "\\b";
				break;
			case '\f':
				output += "\\f";
				break;
			case '\n':
				output += "\\n";
				break;
			case '\r':
				output += "\\r";
				break;
			case '\t':
				output += "\\t";
				break;
			default:
				output += c;
				break;
			}
		}

		return output;
	}

	string json::serialize() const {
		string output;

		switch (this->json_type)
		{
		case value_type::Null:
			output += "null";
			break;
		case value_type::Boolean:
			output += this->get_bool() ? "true" : "false";
			break;
		case value_type::Number:
			output += this->as_number().as_string();
			break;
		case value_type::String:
			output += '"' + escape_string(this->as_string()) + '"';
			break;
		case value_type::Array: {
			output += '[';
			auto it = this->as_vector().cbegin();
			while (it != this->as_vector().cend()) {
				output += it->serialize();
				if (++it == this->as_vector().cend())
					break;
				output += ',';
			}
			output += ']';
			break;
		}
		case value_type::Object: {
			output += '{';
			auto it = this->as_object().cbegin();
			while (it != this->as_object().cend()) {
				output += '"' + it->first + '"';
				output += ':';
				output += it->second.serialize();
				if (++it == this->as_object().cend())
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

	}

	value_type json::get_type() const {
		return this->json_type;
	}

	bool json::get_bool() const {
		if (this->json_type != value_type::Boolean) {
			throw runtime_error("Invalid calling json::get_bool(), source: " + serialize());
		}

		return *static_pointer_cast<bool>(this->json_object).get();
	}

	int8_t json::get_int8() const {
		return as_number().int8_value();
	}

	int16_t json::get_int16() const {
		return as_number().int16_value();
	}

	int32_t json::get_int32() const {
		return as_number().int32_value();
	}

	int64_t json::get_int64() const {
		return as_number().int64_value();
	}

	uint8_t json::get_uint8() const {
		return as_number().uint8_value();
	}

	uint16_t json::get_uint16() const {
		return as_number().uint16_value();
	}

	uint32_t json::get_uint32() const {
		return as_number().uint32_value();
	}

	uint64_t json::get_uint64() const {
		return as_number().uint64_value();
	}

	// Returning Object - Copy needed
	number json::get_number() const {
		if (this->json_type != value_type::Number) {
			throw runtime_error("Invalid calling json::convert_number(), source: " + serialize());
		}

		return *static_pointer_cast<number>(this->json_object).get();
	}

	// Returning Object - Copy needed
	string json::get_string() const {
		if (this->json_type != value_type::String) {
			throw runtime_error("Invalid calling json::get_string(), source: " + serialize());
		}

		return *static_pointer_cast<string>(this->json_object).get();
	}

	// Returning Object - Copy needed
	vector<json> json::get_vector() const {
		if (this->json_type != value_type::Array) {
			throw runtime_error("Invalid calling json::get_vector(), source: " + serialize());
		}

		return *static_pointer_cast<vector<json>>(this->json_object).get();
	}

	// Returning Object - Copy needed
	unordered_map<string, json> json::get_object() const {
		if (this->json_type != value_type::Object) {
			throw runtime_error("Invalid calling json::get_object(), source: " + serialize());
		}

		return *static_pointer_cast<unordered_map<string, json>>(this->json_object).get();
	}

	// Returning Reference - Copy free
	const number& json::as_number() const {
		if (this->json_type != value_type::Number) {
			throw runtime_error("Invalid calling json::get_int(), source: " + serialize());
		}

		return *static_pointer_cast<number>(this->json_object).get();
	}

	// Returning Reference - Copy free
	const string& json::as_string() const {
		if (this->json_type != value_type::String) {
			throw runtime_error("Invalid calling json::convert_string(), source: " + serialize());
		}

		return *static_pointer_cast<string>(this->json_object).get();
	}

	// Returning Reference - Copy free
	const vector<json>& json::as_vector() const {
		if (this->json_type != value_type::Array) {
			throw runtime_error("Invalid calling json::convert_vector(), source: " + serialize());
		}

		return *static_pointer_cast<vector<json>>(this->json_object).get();
	}

	// Returning Reference - Copy free
	const unordered_map<string, json>& json::as_object() const {
		if (this->json_type != value_type::Object) {
			throw runtime_error("Invalid calling json::convert_object(), source: " + serialize());
		}

		return *static_pointer_cast<unordered_map<string, json>>(this->json_object).get();
	}

	// Returning Reference - Copy free
	number& json::as_number() {
		if (this->json_type != value_type::Number) {
			throw runtime_error("Invalid calling json::convert_number(), source: " + serialize());
		}

		return *static_pointer_cast<number>(this->json_object).get();
	}

	// Returning Reference - Copy free
	string& json::as_string() {
		if (this->json_type != value_type::String) {
			throw runtime_error("Invalid calling json::convert_string(), source: " + serialize());
		}

		return *static_pointer_cast<string>(this->json_object).get();
	}

	// Returning Reference - Copy free
	vector<json>& json::as_vector() {
		if (this->json_type != value_type::Array) {
			throw runtime_error("Invalid calling json::convert_vector(), source: " + serialize());
		}

		return *static_pointer_cast<vector<json>>(this->json_object).get();
	}

	// Returning Reference - Copy free
	unordered_map<string, json>& json::as_object() {
		if (this->json_type != value_type::Object) {
			throw runtime_error("Invalid calling json::convert_object(), source: " + serialize());
		}

		return *static_pointer_cast<unordered_map<string, json>>(this->json_object).get();
	}

	// Indexer
	json& json::operator[](size_t index) {
		if (this->json_type == value_type::Array) {
			if (index >= 0 && index < this->as_vector().size()) {
				return this->as_vector()[index];
			}

			throw runtime_error("Invalid index, want to get: " + to_string(index) + ", actual size: " + to_string(this->as_vector().size()));
		}

		throw runtime_error("You can not call integer-indexer for a non-array json, source: " + serialize());
	}

	json& json::operator[](const string& index) {
		if (this->json_type == value_type::Object) {
			return this->as_object()[index];
		}

		throw runtime_error("You can not call string-indexer for a non-object json, source: " + serialize());
	}

	json& json::operator[](string&& index) {
		if (this->json_type == value_type::Object) {
			return this->as_object()[index];
		}

		throw runtime_error("You can not call string-indexer for a non-object json, source: " + serialize());
	}

	// Const Indexer
	const json& json::operator[](size_t index) const {
		if (this->json_type == value_type::Array) {
			if (index >= 0 && index < this->as_vector().size()) {
				return this->as_vector()[index];
			}

			throw runtime_error("Invalid index, want to get: " + to_string(index) + ", actual size: " + to_string(this->as_vector().size()));
		}

		throw runtime_error("You can not call integer-indexer for a non-array json, source: " + serialize());
	}

	const json& json::operator[](const string& index) const {
		if (this->json_type == value_type::Object) {
			return this->as_object().at(index);
		}

		throw runtime_error("You can not call string-indexer for a non-object json, source: " + serialize());
	}

	const json& json::operator[](string&& index) const {
		if (this->json_type == value_type::Object) {
			return this->as_object().at(index);
		}

		throw runtime_error("You can not call string-indexer for a non-object json, source: " + serialize());
	}

	void json::push_back(const json& that) {
		if (this->json_type == value_type::Array) {
			this->as_vector().push_back(that);
		}

		throw runtime_error("You can not call json::push_back(const json&) for a non-array json, source: " + serialize());
	}

	void json::emplace_back(json&& that) {
		if (this->json_type == value_type::Array) {
			this->as_vector().emplace_back(that);
		}

		throw runtime_error("You can not call json::emplace_back(json&&) for a non-array json, source: " + serialize());
	}

	void json::insert(const string& key, const json& that) {
		if (this->json_type == value_type::Object) {
			this->as_object().insert(pair<string, json>(key, that));
		}

		throw runtime_error("You can not call json::insert(const std::string& key, const json& that) for a non-object json, source: " + serialize());
	}

	void json::insert(const string& key, json&& that) {
		if (this->json_type == value_type::Object) {
			this->as_object().insert(pair<string, json>(key, that));
		}

		throw runtime_error("You can not call json::insert(const std::string& key, json&& that) for a non-object json, source: " + serialize());
	}

	void json::insert(string&& key, const json& that) {
		if (this->json_type == value_type::Object) {
			this->as_object().insert(pair<string, json>(key, that));
		}

		throw runtime_error("You can not call json::insert(std::string&& key, const json& that) for a non-object json, source: " + serialize());
	}

	void json::insert(string&& key, json&& that) {
		if (this->json_type == value_type::Object) {
			this->as_object().insert(pair<string, json>(key, that));
		}

		throw runtime_error("You can not call json::insert(std::string&& key, json&& that) for a non-object json, source: " + serialize());
	}

	shared_ptr<nullptr_t> json::parse_null(const string& input, index& index) {
		if (input[index.saveGet()] != 'n') 
			throw json_parse_exception(build_error_message("Invalid null node", input, index.get()));
		index.increment();
		if (input[index.saveGet()] != 'u')
			throw json_parse_exception(build_error_message("Invalid null node", input, index.get()));
		index.increment();
		if (input[index.saveGet()] != 'l')
			throw json_parse_exception(build_error_message("Invalid null node", input, index.get()));
		index.increment();
		if (input[index.saveGet()] != 'l')
			throw json_parse_exception(build_error_message("Invalid null node", input, index.get()));
		index.increment();
		return make_shared<nullptr_t>(nullptr);
	}

	shared_ptr<bool> json::parse_boolean(const string& input, index& index) {
		bool t = input[index.saveGet()] == 't';
		if (t) {
			if (input[index.saveGet()] != 't')
				throw json_parse_exception(build_error_message("Invalid true node", input, index.get()));
			index.increment();
			if (input[index.saveGet()] != 'r')
				throw json_parse_exception(build_error_message("Invalid true node", input, index.get()));
			index.increment();
			if (input[index.saveGet()] != 'u')
				throw json_parse_exception(build_error_message("Invalid true node", input, index.get()));
			index.increment();
			if (input[index.saveGet()] != 'e')
				throw json_parse_exception(build_error_message("Invalid true node", input, index.get()));
			index.increment();
		}
		else {
			if (input[index.saveGet()] != 'f')
				throw json_parse_exception(build_error_message("Invalid false node", input, index.get()));
			index.increment();
			if (input[index.saveGet()] != 'a')
				throw json_parse_exception(build_error_message("Invalid false node", input, index.get()));
			index.increment();
			if (input[index.saveGet()] != 'l')
				throw json_parse_exception(build_error_message("Invalid false node", input, index.get()));
			index.increment();
			if (input[index.saveGet()] != 's')
				throw json_parse_exception(build_error_message("Invalid false node", input, index.get()));
			index.increment();
			if (input[index.saveGet()] != 'e')
				throw json_parse_exception(build_error_message("Invalid false node", input, index.get()));
			index.increment();
		}
		return make_shared<bool>(t);
	}

	shared_ptr<number> json::parse_number(const string& input, index& index) {
		size_t startIndex = index.get();
		size_t currentIndex = startIndex;

		string output;

		if (input[currentIndex] == '-') {
			output += input[currentIndex++];
		}

		if (currentIndex < input.size()
			&& input[currentIndex] >= '0'
			&& input[currentIndex] <= '9') {
			while (currentIndex < input.size()
				&& input[currentIndex] >= '0'
				&& input[currentIndex] <= '9') {
				output += input[currentIndex++];
			}
		}
		else {
			throw json_parse_exception(build_error_message("Incomplete number", input, currentIndex));
		}

		if (currentIndex < input.size()
			&& input[currentIndex] == '.') {
			output += input[currentIndex++];
			if (currentIndex < input.size()
				&& input[currentIndex] >= '0'
				&& input[currentIndex] <= '9') {
				while (currentIndex < input.size()
					&& input[currentIndex] >= '0'
					&& input[currentIndex] <= '9') {
					output += input[currentIndex++];
				}
			}
			else {
				throw json_parse_exception(build_error_message("Incomplete fraction", input, currentIndex));
			}
		}

		if (currentIndex < input.size()
			&& (input[currentIndex] == 'e' || input[currentIndex] == 'E')) {
			output += input[currentIndex++];
			if (input[currentIndex] == '+') {
				output += input[currentIndex++];
			}
			else if (input[currentIndex] == '-') {
				output += input[currentIndex++];
			}
			if (currentIndex < input.size()
				&& input[currentIndex] >= '0'
				&& input[currentIndex] <= '9') {
				while (currentIndex < input.size()
					&& input[currentIndex] >= '0'
					&& input[currentIndex] <= '9') {
					output += input[currentIndex++];
				}
			}
			else {
				throw json_parse_exception(build_error_message("Incomplete exponent", input, currentIndex));
			}
		}

		index.plus(currentIndex - startIndex);

		return make_shared<number>(move(output));
	}

	string json::build_error_message(string error_type, const string& source, size_t position) {
		size_t near_start = position >= 3 ? position - 3 : 0;
		size_t near_end = position + 3 < source.size() ? position + 3 : source.size() - 1;
		return "Parse Error - " + error_type + " at position "
			+ to_string(position) + " near \""
			+ source.substr(near_start, near_end - near_start + 1) + "\", source = \""
			+ source + '\"' + '.';
	}

	char json::hex_to_char(char input) {
		if (input >= 'A' && input <= 'F')
		{
			return input - 'A' + 10;
		}
		else if (input >= 'a' && input <= 'f')
		{
			return input - 'a' + 10;
		}
		else if (input >= '0' && input <= '9')
		{
			return input - '0';
		}
		else
		{
			throw json_parse_exception("Invalid unicode escaped character");
		}
	}

	string json::read_string(const string& input, index& index) {
		if (input[index.saveGet()] != '"') {
			throw json_parse_exception(build_error_message("Invalid string node", input, index.get()));
		}
		index.increment();
		string output;
		while (input[index.saveGet()] != '"') {
			if (input[index.saveGet()] == '\\') {
				index.increment();
				switch (input[index.saveGet()]) {
				case '"':
					output += '"';
					break;
				case '\\':
					output += '\\';
					break;
					// Front Slash Rules
					// - Front slash will NOT be escaped in serialization.
					// - However, an escaped front slash is acceptable '\/' in deserialization.
					// - The behavior is the same as most third-party JSON libs like Jackson and Gson.
				case '/':
					output += '/';
					break;
				case 'b':
					output += '\b';
					break;
				case 'f':
					output += '\f';
					break;
				case 'n':
					output += '\n';
					break;
				case 'r':
					output += '\r';
					break;
				case 't':
					output += '\t';
					break;
				case 'u': {
					try {
						index.increment();
						char h1 = hex_to_char(input[index.saveGet()]);
						index.increment();
						char h2 = hex_to_char(input[index.saveGet()]);
						index.increment();
						char h3 = hex_to_char(input[index.saveGet()]);
						index.increment();
						char h4 = hex_to_char(input[index.saveGet()]);
						output += (char)((h1 << 12) + (h2 << 8) + (h3 << 4) + h4);
					}
					catch (json_parse_exception& e) {
						throw json_parse_exception(build_error_message("Invalid escaped character", input, index.get()));
					}
					break;
				}
				default:
					throw json_parse_exception(build_error_message("Invalid escaped character", input, index.get()));
				}
			}
			else {
				output += input[index.saveGet()];
			}
			index.increment();
		}
		index.increment();
		return output;
	}

	shared_ptr<string> json::parse_string(const string& input, index& index) {
		return make_shared<string>(read_string(input, index));
	}

	shared_ptr<vector<json>> json::parse_array(const string& input, index& index) {
		if (input[index.saveGet()] != '[') {
			throw json_parse_exception(build_error_message("Invalid list node", input, index.get()));
		}
		index.increment();
		shared_ptr<vector<json>> p = make_shared<vector<json>>();
		read_whitespace(input, index);
		if (input[index.saveGet()] != ']') {
			while (true) {
				p->push_back(parse_value(input, index));
				if (input[index.saveGet()] == ']') {
					break;
				}
				else if (input[index.saveGet()] == ',') {
					index.increment();
				}
				else {
					throw json_parse_exception(build_error_message("Missing comma", input, index.get()));
				}
			}
		}
		index.increment();
		return p;
	}

	shared_ptr<unordered_map<string, json>> json::parse_object(const string& input, index& index) {
		if (input[index.saveGet()] != '{') {
			throw json_parse_exception(build_error_message("Invalid object node", input, index.get()));
		}
		index.increment();
		shared_ptr<unordered_map<string, json>> p = make_shared<unordered_map<string, json>>();
		read_whitespace(input, index);
		if (input[index.saveGet()] != '}') {
			string key;
			while (true) {
				read_whitespace(input, index);
				key = read_string(input, index);
				read_whitespace(input, index);
				if (input[index.saveGet()] != ':') {
					throw json_parse_exception(build_error_message("Missing colon", input, index.get()));
				}
				index.increment();
				p->insert({ move(key), parse_value(input, index) });
				if (input[index.saveGet()] == '}') {
					break;
				}
				else if (input[index.saveGet()] == ',') {
					index.increment();
				}
				else {
					throw json_parse_exception(build_error_message("Missing comma", input, index.get()));
				}
			}
		}
		index.increment();
		return p;
	}
}

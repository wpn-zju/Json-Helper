#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <stdexcept>
#include <memory>

namespace peinan {
	enum struct value_type;

	class json_parse_exception : public std::runtime_error {
	public:
		json_parse_exception(std::string);
	};
	
	class number {
	public:
		number(const std::string& value);
		number(std::string&& value);
		number(const number& that);
		int8_t int8_value() const;
		int16_t int16_value() const;
		int32_t int32_value() const;
		int64_t int64_value() const;
		uint8_t uint8_value() const;
		uint16_t uint16_value() const;
		uint32_t uint32_value() const;
		uint64_t uint64_value() const;
		float float_value() const;
		double double_value() const;
		std::string to_string() const;
		const std::string& as_string() const;

	private:
		const std::string value;
	};

	class index {
	public:
		index(size_t length);
		size_t get();
		size_t saveGet();
		void increment();
		void plus(size_t addend);

	private:
		size_t value;
		const size_t length;
	};

	class json {
	public:
		json();
		json(bool);
		json(int8_t);
		json(int16_t);
		json(int32_t);
		json(int64_t);
		json(uint8_t);
		json(uint16_t);
		json(uint32_t);
		json(uint64_t);
		json(const number&);
		json(number&&);
		json(const std::string&);
		json(std::string&&);
		json(const std::vector<json>&);
		json(std::vector<json>&&);
		json(const std::unordered_map<std::string, json>&);
		json(std::unordered_map<std::string, json>&&);
		json(const json&);
		json(json&&) noexcept;
		json& operator=(const json&);
		json& operator=(json&&) noexcept;
		~json();

		static json create(const std::string&);
		static json create(const char*);

		std::string serialize() const;

		value_type get_type() const;

		bool get_bool() const;
		int8_t get_int8() const;
		int16_t get_int16() const;
		int32_t get_int32() const;
		int64_t get_int64() const;
		uint8_t get_uint8() const;
		uint16_t get_uint16() const;
		uint32_t get_uint32() const;
		uint64_t get_uint64() const;
		number get_number() const;
		std::string get_string() const;
		std::vector<json> get_vector() const;
		std::unordered_map<std::string, json> get_object() const;
		const number& as_number() const;
		const std::string& as_string() const;
		const std::vector<json>& as_vector() const;
		const std::unordered_map<std::string, json>& as_object() const;
		number& as_number();
		std::string& as_string();
		std::vector<json>& as_vector();
		std::unordered_map<std::string, json>& as_object();

		json& operator[](std::size_t index);
		json& operator[](const std::string& index);
		json& operator[](std::string&& index);
		const json& operator[](std::size_t index) const;
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
		std::shared_ptr<void> json_object;

		json(value_type, std::shared_ptr<void>);

		static std::string build_error_message(std::string, const std::string&, std::size_t);
		static char hex_to_char(char input);
		static void read_whitespace(const std::string& input, index& index);
		static json parse_value(const std::string& input, index& index);
		static json parse_value_body(const std::string& input, index& index);
		static std::shared_ptr<nullptr_t> parse_null(const std::string& input, index& index);
		static std::shared_ptr<bool> parse_boolean(const std::string& input, index& index);
		static std::shared_ptr<number> parse_number(const std::string& input, index& index);
		static std::string escape_string(const std::string& input);
		static std::string read_string(const std::string& input, index& index);
		static std::shared_ptr<std::string> parse_string(const std::string& input, index& index);
		static std::shared_ptr<std::vector<json>> parse_array(const std::string& input, index& index);
		static std::shared_ptr<std::unordered_map<std::string, json>> parse_object(const std::string& input, index& index);
	};

	enum struct value_type {
		Null,
		Boolean,
		Number,
		String,
		Array,
		Object
	};
}

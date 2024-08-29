#pragma once

#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>
#include <numeric>
#include <cstdint>

namespace utf8 {

    class string {
    public:
        // Constructors
        inline string(const char* str) : data(str) {
            size_t i = 0;
            while (i < data.size()) {
                size_t char_size = char_bytes(static_cast<unsigned char>(data[i]));
                data_size.emplace_back(char_size);
                i += char_size;
            }
        }

        inline string() : data("") {}

        inline string(const utf8::string& other) : data(other.data), data_size(other.data_size) {}

        inline string(utf8::string&& other) noexcept : data(std::move(other.data)), data_size(std::move(other.data_size)) {}

        // Destructor
        inline ~string() {}

        // Type conversion operator
        inline operator std::string() const {
            return data;
        }

        // Assignment operators
        inline utf8::string& operator=(const utf8::string& other) {
            if (this != &other) {
                data = other.data;
                data_size = other.data_size;
            }
            return *this;
        }

        inline utf8::string& operator=(utf8::string&& other) noexcept {
            if (this != &other) {
                data = std::move(other.data);
                data_size = std::move(other.data_size);
            }
            return *this;
        }

        // Element access
        inline std::string operator[](size_t index) const {
            return at(index);
        }

        inline std::string at(size_t index) const {
            if (index >= data_size.size()) {
                throw std::out_of_range("Index out of range. No character exists at the specified index.");
            }

            size_t c_index = std::accumulate(data_size.begin(), data_size.begin() + index, 0);
            size_t char_size = data_size[index];

            return data.substr(c_index, char_size);
        }

        // Concatenation operators
        inline utf8::string& operator+=(const utf8::string& other) {
            size_t i = data.size();
            data += other.data;
            while (i < data.size()) {
                size_t char_size = char_bytes(static_cast<unsigned char>(data[i]));
                data_size.emplace_back(char_size);
                i += char_size;
            }
            return *this;
        }

        inline utf8::string& operator+=(const std::string& str) {
            size_t i = data.size();
            data += str;
            while (i < data.size()) {
                size_t char_size = char_bytes(static_cast<unsigned char>(data[i]));
                data_size.emplace_back(char_size);
                i += char_size;
            }
            return *this;
        }

        inline utf8::string& operator+=(const char* str) {
            size_t i = data.size();
            data += str;
            while (i < data.size()) {
                size_t char_size = char_bytes(static_cast<unsigned char>(data[i]));
                data_size.emplace_back(char_size);
                i += char_size;
            }
            return *this;
        }

        // Comparison operators
        friend inline bool operator==(const utf8::string& lhs, const utf8::string& rhs) {
            return lhs.data == rhs.data;
        }

        friend inline bool operator!=(const utf8::string& lhs, const utf8::string& rhs) {
            return !(lhs == rhs);
        }

        friend inline bool operator==(const std::string& lhs, const utf8::string& rhs) {
            return lhs == rhs.data;
        }

        friend inline bool operator!=(const std::string& lhs, const utf8::string& rhs) {
            return !(lhs == rhs);
        }

        friend inline bool operator==(const utf8::string& lhs, const char* rhs) {
            return lhs.data == rhs;
        }

        friend inline bool operator!=(const utf8::string& lhs, const char* rhs) {
            return !(lhs == rhs);
        }

        // Concatenation friend functions
        friend inline utf8::string operator+(const utf8::string& lhs, const utf8::string& rhs) {
            utf8::string result = lhs;
            result += rhs;
            return result;
        }

        friend inline utf8::string operator+(const utf8::string& lhs, const std::string& rhs) {
            utf8::string result = lhs;
            result += rhs;
            return result;
        }

        friend inline utf8::string operator+(const std::string& lhs, const utf8::string& rhs) {
            utf8::string result(lhs.data());
            result += rhs;
            return result;
        }

        friend inline utf8::string operator+(const utf8::string& lhs, const char* rhs) {
            utf8::string result = lhs;
            result += rhs;
            return result;
        }

        friend inline utf8::string operator+(const char* lhs, const utf8::string& rhs) {
            utf8::string result(lhs);
            result += rhs;
            return result;
        }

        // Size and capacity
        inline size_t size() const {
            return data_size.size();
        }

        inline bool empty() const {
            return data.empty();
        }

        inline void clear() {
            data.clear();
            data_size.clear();
        }

        // Substring
        inline utf8::string substr(size_t pos, size_t len = std::string::npos) const {
            if (pos >= size()) {
                throw std::out_of_range("Index out of range. Starting position exceeds string size.");
            }

            size_t start = std::accumulate(data_size.begin(), data_size.begin() + pos, 0);
            size_t end = start;

            if (len == std::string::npos) {
                len = size() - pos;  
            }

            size_t char_count = std::min(len, size() - pos);

            for (size_t i = 0; i < char_count; ++i) {
                end += data_size[pos + i];
            }

            return utf8::string(data.substr(start, end - start).data());
        }

        // Searching
        inline size_t find(const std::string& substring) const {
            size_t current_index = 0;
            size_t i = 0;
            
            while (i < data.size()) {
                if (data.substr(i, substring.size()) == substring) {
                    return current_index;
                }

                size_t char_size = data_size[current_index];
                i += char_size;
                ++current_index;
            }
            
            return std::string::npos;
        }

        inline bool contains(const std::string& str) {
            return data.find(str) != std::string::npos;
        }

        inline bool starts_with(const std::string& str) {
            return data.find(str) == 0;
        }

        inline bool ends_with(const std::string& str) {
            return data.size() >= str.size() && data.compare(data.size() - str.size(), str.size(), str) == 0;
        }

        // Element access
        inline std::string front() const {
            return empty()? "" : at(0);
        }

        inline std::string back() const {
            return empty()? "" : at(data_size.size() - 1);
        }

        // Erase a single UTF-8 character at the given index
        inline void erase(size_t index) {
            if (index >= data_size.size()) {
                throw std::out_of_range("Index out of range.");
            }

            size_t start = std::accumulate(data_size.begin(), data_size.begin() + index, 0);
            size_t char_size = data_size[index];

            data.erase(start, char_size);
            data_size.erase(data_size.begin() + index);
        }

        // Erase a range of UTF-8 characters [start_index, end_index)
        inline void erase(size_t start_index, size_t end_index) {
            if (start_index >= data_size.size() || end_index > data_size.size() || start_index >= end_index) {
                throw std::out_of_range("Invalid range.");
            }

            size_t start = std::accumulate(data_size.begin(), data_size.begin() + start_index, 0);
            size_t end = std::accumulate(data_size.begin(), data_size.begin() + end_index, 0);

            data.erase(start, end - start);
            data_size.erase(data_size.begin() + start_index, data_size.begin() + end_index);
        }

        inline uint32_t codepoint(size_t index) const {
            if (index >= data_size.size()) {
                throw std::out_of_range("Index out of range.");
            }

            size_t start = std::accumulate(data_size.begin(), data_size.begin() + index, 0);
            size_t char_size = data_size[index];

            const unsigned char* bytes = reinterpret_cast<const unsigned char*>(data.data()) + start;

            uint32_t codepoint = 0;
            if (char_size == 1) {
                codepoint = bytes[0];
            } else if (char_size == 2) {
                codepoint = ((bytes[0] & 0x1F) << 6) |
                            (bytes[1] & 0x3F);
            } else if (char_size == 3) {
                codepoint = ((bytes[0] & 0x0F) << 12) |
                            ((bytes[1] & 0x3F) << 6) |
                            (bytes[2] & 0x3F);
            } else if (char_size == 4) {
                codepoint = ((bytes[0] & 0x07) << 18) |
                            ((bytes[1] & 0x3F) << 12) |
                            ((bytes[2] & 0x3F) << 6) |
                            (bytes[3] & 0x3F);
            } else {
                throw std::runtime_error("Invalid UTF-8 character.");
            }

            return codepoint;
        }

        inline bool is_symbol(const uint32_t index) const {
            uint32_t curr_codepoint = codepoint(index);
            return
                (curr_codepoint >= 0x0021 && curr_codepoint <= 0x002F) || // ! " # $ % & ' ( ) * + , - . /
                (curr_codepoint >= 0x003A && curr_codepoint <= 0x0040) || // : ; < = > ? @
                (curr_codepoint >= 0x005B && curr_codepoint <= 0x0060) || // [ \ ] ^ _ `
                (curr_codepoint >= 0x007B && curr_codepoint <= 0x007E) || // { | } ~
                (curr_codepoint == 0x20AC) || // €
                (curr_codepoint >= 0x2000 && curr_codepoint <= 0x206F) || // General Punctuation
                (curr_codepoint >= 0x2100 && curr_codepoint <= 0x214F) || // Letterlike Symbols
                (curr_codepoint >= 0x2200 && curr_codepoint <= 0x22FF) || // Mathematical Operators
                (curr_codepoint >= 0x2300 && curr_codepoint <= 0x23FF) || // Miscellaneous Technical
                (curr_codepoint >= 0x2400 && curr_codepoint <= 0x243F) || // Control Pictures
                (curr_codepoint >= 0x2440 && curr_codepoint <= 0x245F) || // Optical Character Recognition
                (curr_codepoint >= 0x2500 && curr_codepoint <= 0x257F) || // Box Drawing
                (curr_codepoint >= 0x2580 && curr_codepoint <= 0x259F) || // Block Elements
                (curr_codepoint >= 0x25A0 && curr_codepoint <= 0x25FF) || // Geometric Shapes
                (curr_codepoint >= 0x2600 && curr_codepoint <= 0x26FF) || // Miscellaneous Symbols
                (curr_codepoint >= 0x2700 && curr_codepoint <= 0x27BF) || // Dingbats
                (curr_codepoint >= 0x2B50 && curr_codepoint <= 0x2B59) || // Miscellaneous Symbols and Pictographs
                (curr_codepoint >= 0x1F300 && curr_codepoint <= 0x1F5FF) || // Miscellaneous Symbols and Pictographs
                (curr_codepoint >= 0x1F600 && curr_codepoint <= 0x1F64F) || // Emoticons
                (curr_codepoint >= 0x1F680 && curr_codepoint <= 0x1F6FF) || // Transport and Map Symbols
                (curr_codepoint >= 0x1F700 && curr_codepoint <= 0x1F77F);   // Alchemical Symbols
        }

        inline bool is_uppercase(const uint32_t index) const {
            uint32_t curr_codepoint = codepoint(index);
            return
                (curr_codepoint >= 0x0041 && curr_codepoint <= 0x005A) || // Basic Latin A-Z
                (curr_codepoint >= 0x00C0 && curr_codepoint <= 0x00D6) || // Latin-1 Supplement À-Ö
                (curr_codepoint >= 0x00D8 && curr_codepoint <= 0x00DE) || // Latin-1 Supplement Ø-Þ
                (curr_codepoint >= 0x0100 && curr_codepoint <= 0x017F) || // Latin Extended-A
                (curr_codepoint >= 0x0180 && curr_codepoint <= 0x024F) || // Latin Extended-B
                (curr_codepoint >= 0x0410 && curr_codepoint <= 0x042F) || // Cyrillic А-Я
                (curr_codepoint >= 0x0391 && curr_codepoint <= 0x03A9) || // Greek and Coptic Α-Ω
                (curr_codepoint >= 0x0531 && curr_codepoint <= 0x0556) || // Armenian
                (curr_codepoint >= 0x05D0 && curr_codepoint <= 0x05EA) || // Hebrew
                (curr_codepoint >= 0x0600 && curr_codepoint <= 0x06C0) || // Arabic (some uppercase)
                (curr_codepoint >= 0x0780 && curr_codepoint <= 0x07A5) || // Thaana
                (curr_codepoint >= 0x0905 && curr_codepoint <= 0x0939) || // Devanagari
                (curr_codepoint >= 0x0985 && curr_codepoint <= 0x0995) || // Bengali
                (curr_codepoint >= 0x0A05 && curr_codepoint <= 0x0A0A) || // Gurmukhi
                (curr_codepoint >= 0x0A85 && curr_codepoint <= 0x0A8D) || // Gujarati
                (curr_codepoint >= 0x0B05 && curr_codepoint <= 0x0B0C) || // Oriya
                (curr_codepoint >= 0x0B85 && curr_codepoint <= 0x0B9A) || // Tamil
                (curr_codepoint >= 0x0C05 && curr_codepoint <= 0x0C0C) || // Telugu
                (curr_codepoint >= 0x0C85 && curr_codepoint <= 0x0C8C) || // Kannada
                (curr_codepoint >= 0x0D05 && curr_codepoint <= 0x0D0C) || // Malayalam
                (curr_codepoint >= 0x1000 && curr_codepoint <= 0x102A) || // Myanmar
                (curr_codepoint >= 0x10A0 && curr_codepoint <= 0x10C5) || // Georgian
                (curr_codepoint >= 0x1100 && curr_codepoint <= 0x1159) || // Hangul Jamo
                (curr_codepoint >= 0xAC00 && curr_codepoint <= 0xD7A3);   // Hangul Syllables
        }

        // Stream insertion operator
        friend inline std::ostream& operator<<(std::ostream& os, const utf8::string& utf8str) {
            os << utf8str.data;
            return os;
        }

    private:
        std::string data;
        std::vector<size_t> data_size;

        // Helper function to determine the number of bytes in a UTF-8 character
        inline int32_t char_bytes(unsigned char byte) const {
            if ((byte & 0x80) == 0x00) {
                return 1;
            } else if ((byte & 0xE0) == 0xC0) {
                return 2;
            } else if ((byte & 0xF0) == 0xE0) {
                return 3;
            } else if ((byte & 0xF8) == 0xF0) {
                return 4;
            } else {
                throw std::runtime_error("Invalid UTF-8 character.");
            }
        }
    };

    inline bool is_uppercase(const utf8::string& utf8str) {
        return utf8str.is_uppercase(0);
    }

    inline bool is_symbol(const utf8::string& utf8str) {
        return utf8str.is_symbol(0);
    }
}

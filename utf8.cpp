#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>
#include <numeric>

namespace utf8 {

    class string {
    public:
        string(const char* str) : data(str) {
            size_t i = 0;
            while (i < data.size()) {
                size_t char_size = char_bytes(static_cast<unsigned char>(data[i]));
                data_size.emplace_back(char_size);
                i += char_size;
            }
        }
    
        string() : data("") {}
    
        string(const utf8::string& other) : data(other.data), data_size(other.data_size) {}
    
        string(utf8::string&& other) noexcept : data(std::move(other.data)), data_size(std::move(other.data_size)) {}
    
        ~string() {}

        operator std::string() const {
            return data;
        }

        utf8::string& operator=(const utf8::string& other) {
            if (this != &other) {
                data = other.data;
                data_size = other.data_size;
            }
            return *this;
        }
    
        utf8::string& operator=(utf8::string&& other) noexcept {
            if (this != &other) {
                data = std::move(other.data);
                data_size = std::move(other.data_size);
            }
            return *this;
        }
    
        std::string operator[](size_t index) const {
            return at(index);
        }
    
        utf8::string& operator+=(const utf8::string& other) {
            size_t i = data.size();
            data += other.data;
            while (i < data.size()) {
                size_t char_size = char_bytes(static_cast<unsigned char>(data[i]));
                data_size.emplace_back(char_size);
                i += char_size;
            }
            return *this;
        }
    
        utf8::string& operator+=(const std::string& str) {
            size_t i = data.size();
            data += str;
            while (i < data.size()) {
                size_t char_size = char_bytes(static_cast<unsigned char>(data[i]));
                data_size.emplace_back(char_size);
                i += char_size;
            }
            return *this;
        }
    
        utf8::string& operator+=(const char* str) {
            size_t i = data.size();
            data += str;
            while (i < data.size()) {
                size_t char_size = char_bytes(static_cast<unsigned char>(data[i]));
                data_size.emplace_back(char_size);
                i += char_size;
            }
            return *this;
        }
    
        friend bool operator==(const utf8::string& lhs, const utf8::string& rhs) {
            return lhs.data == rhs.data;
        }

        friend bool operator!=(const utf8::string& lhs, const utf8::string& rhs) {
            return !(lhs == rhs);
        }

        friend bool operator==(const std::string& lhs, const utf8::string& rhs) {
            return lhs == rhs.data;
        }

        friend bool operator!=(const std::string& lhs, const utf8::string& rhs) {
            return !(lhs == rhs);
        }

        friend bool operator==(const utf8::string& lhs, const char* rhs) {
            return lhs.data == rhs;
        }

        friend bool operator!=(const utf8::string& lhs, const char* rhs) {
            return !(lhs == rhs);
        }

        friend utf8::string operator+(const utf8::string& lhs, const utf8::string& rhs) {
            utf8::string result = lhs;
            result += rhs;
            return result;
        }
    
        friend utf8::string operator+(const utf8::string& lhs, const std::string& rhs) {
            utf8::string result = lhs;
            result += rhs;
            return result;
        }
    
        friend utf8::string operator+(const std::string& lhs, const utf8::string& rhs) {
            utf8::string result(lhs.data());
            result += rhs;
            return result;
        }
    
        friend utf8::string operator+(const utf8::string& lhs, const char* rhs) {
            utf8::string result = lhs;
            result += rhs;
            return result;
        }
    
        friend utf8::string operator+(const char* lhs, const utf8::string& rhs) {
            utf8::string result(lhs);
            result += rhs;
            return result;
        }
    
        size_t size() const {
            return data_size.size();
        }
    
        std::string at(size_t index) const {
            if (index >= data_size.size()) {
                throw std::out_of_range("Index out of range. No character exists at the specified index.");
            }
    
            size_t c_index = std::accumulate(data_size.begin(), data_size.begin() + index, 0);
            size_t char_size = data_size[index];
    
            return data.substr(c_index, char_size);
        }
    
        size_t find(const std::string& substring) const {
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
    
        bool empty() const {
            return data.empty();
        }
    
        void clear() {
            data.clear();
            data_size.clear();
        }

        bool contains(const std::string& str) {
            return data.contains(str);
        }

        bool starts_with(const std::string& str) {
            return data.starts_with(str);
        }
        
        bool ends_with(const std::string& str) {
            return data.ends_with(str);
        }

        std::string front() const {
            return empty()? "" : at(0);
        }
        
        std::string back() const {
            return empty()? "" : at(data_size.size() - 1);
        }

        utf8::string substr(size_t pos, size_t len = std::string::npos) const {
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

        friend std::ostream& operator<<(std::ostream& os, const utf8::string& utf8str) {
            os << utf8str.data;
            return os;
        }
    
    private:
        std::string data;
        std::vector<size_t> data_size;
    
        int32_t char_bytes(unsigned char byte) const {
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
}

int32_t main() {
    utf8::string str = "Hάel😎 lo, 世界!"; 
    std::cout << str << '\n';
    
    std::string stdstr = "κόσμος ";
    
    str =  stdstr + str + "mama mia";
    
    std::cout << "Length: " << str.size() << '\n';
    std::cout << "Character at index 11: " << str[11] << '\n';

    std::cout << str << '\n';

    size_t pos = str.find("😎");
    if (pos != std::string::npos) {
        std::cout << "Found '😎 ' at position: " << pos << '\n';
    }

}

Example: 

#include "utf8_string.hpp"

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

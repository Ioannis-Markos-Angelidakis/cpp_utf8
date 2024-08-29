Example: 
```
#include "utf8_string.hpp"

int32_t main() {
    utf8::string str = "Hάel😎 lo, 世界!"; 
    
    std::cout << str << '\n';

    str.erase(10);
    
    std::string stdstr = "κόσμος ";
    
    str =  stdstr + str + "mama mia";
    
    std::cout << "Length: " << str.size() << '\n';
    std::cout << "Character at index 11: " << str[11] << '\n';

    std::cout << str << '\n';

    size_t pos = str.find("😎");
    if (pos != std::string::npos) {
        std::cout << "Found '😎 ' at position: " << pos << '\n';
    }

    std::cout << "Is symbol? `" << str.at(11) << "` " << (str.is_symbol(11)? "YES\n" : "NO\n");
}

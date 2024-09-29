<h1>UTF-8 String Handling Library</h1>

<p>This header file provides a UTF-8 compliant string class in C++. It offers an easy-to-use interface for manipulating and processing UTF-8 encoded strings, while ensuring proper handling of multibyte characters.</p>

<h2>Features</h2>
<ul>
  <li>Efficient handling of UTF-8 strings</li>
  <li>Basic string operations: concatenation, comparison, and access</li>
  <li>Support for UTF-8 character boundaries, ensuring correct indexing and substring operations</li>
  <li>Helper functions to check for uppercase letters, symbols, and codepoints</li>
</ul>

<h2>Key Methods and Operators</h2>
<ul>
  <li><code>operator[](size_t index)</code> - Returns the character at the given UTF-8 index.</li>
  <li><code>substr(size_t pos, size_t len)</code> - Returns a substring starting from the UTF-8 character at position <code>pos</code>.</li>
  <li><code>size()</code> - Returns the number of UTF-8 characters in the string.</li>
  <li><code>operator+=</code> - Supports concatenation of UTF-8 strings and regular <code>std::string</code> values.</li>
  <li><code>find(const std::string& substring)</code> - Finds a substring within the UTF-8 string and returns its position.</li>
  <li><code>erase(size_t index)</code> - Removes a UTF-8 character at the specified index.</li>
  <li><code>codepoint(size_t index)</code> - Returns the Unicode codepoint for the character at the specified index.</li>
  <li><code>is_uppercase(size_t index)</code> - Checks if the character at the specified index is an uppercase letter.</li>
  <li><code>is_symbol(size_t index)</code> - Checks if the character at the specified index is a symbol.</li>
</ul>

<h2>UTF-8 Compliance</h2>
<p>This library ensures proper handling of UTF-8 encoded strings by correctly identifying the number of bytes in each character using the <code>char_bytes()</code> helper function. It supports characters of varying byte lengths (1-4 bytes) and offers safety checks to prevent out-of-bound access or invalid character manipulation.</p>

<h2>Stream Insertion</h2>
<p>The library provides support for streaming UTF-8 strings using the <code>operator&lt;&lt;</code> for <code>std::ostream</code>.</p>

<h2>Example Usage</h2>
<pre><code>
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
    std::cout << "Is uppercase? `" << "Ц " << (utf8::is_uppercase("И")? "YES\n" : "NO\n");
}
</code></pre>

<h2>Compilation</h2>
<p>This header is designed to be lightweight and does not require external dependencies. Simply include the header in your project:</p>
<pre><code>#include "utf8_string.hpp"</code></pre>

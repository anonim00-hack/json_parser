#include <iostream>
#include <map>
#include <string>

// ===================== // For string // ===================== //
std::string extractString(const std::string& s, int& i) {
    std::string result = "";
    i++; 
    while (i < s.size() && s[i] != '"') {
        result += s[i];
        i++;
    }
    i++; 
    return result;
}

// ===================== // For numbers // ===================== // 
std::string extractNumber(const std::string& s, int& i) {
    std::string result = "";
    while (i < s.size() && std::isdigit(s[i])) {
        result += s[i];
        i++;
    }
    return result;
}

int main() {
    // ===================== // Any Json. (I just couldn't find a json file so) // ===================== //
    std::string rawJson = R"({"name": "IVAN", "age": 400, "type": "STUPID"})";
    std::map<std::string, std::string> data;

    for (int i = 0; i < rawJson.size(); i++) {
        // Нашли ключ
        if (rawJson[i] == '"') {
            std::string key = extractString(rawJson, i);

            // ===================== // while current will be : or continue // ===================== //
            while (i < rawJson.size() && rawJson[i] != ':') i++;
            i++; // проскочили ':'
            
            // ===================== // if current is space also continue // ===================== //
            while (i < rawJson.size() && std::isspace(rawJson[i])) i++;

            // ===================== // is current number or string // ===================== //
            if (rawJson[i] == '"') {
                data[key] = extractString(rawJson, i);
            } else if (std::isdigit(rawJson[i])) {
                data[key] = extractNumber(rawJson, i);
            }
        }
    }
    // ===================== // show in console // ===================== //
    std::cout << "Name: " << data["name"] << std::endl;
    std::cout << "Age: " << data["age"] << std::endl;
    std::cout << "Type: " << data["type"] << std::endl;

    return 0;
}
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <variant>
#include <vector>
#include <cctype>

struct JsonMap;
using JsonObject = std::map<std::string, JsonMap>;
using JsonArray = std::vector<JsonMap>;

struct JsonMap {
    std::variant<std::string, JsonObject, JsonArray> data;

    JsonMap() : data("") {}
    JsonMap(std::string s) : data(s) {}
    JsonMap(JsonObject o) : data(o) {}
    JsonMap(JsonArray a) : data(a) {}

    bool isString() { return data.index() == 0; }
    bool isObject() { return data.index() == 1; }
    bool isArray()  { return data.index() == 2; }

    std::string asString() { return std::get<0>(data); }
    JsonObject& asObject() { return std::get<1>(data); }
    JsonArray&  asArray()  { return std::get<2>(data); }
};

JsonObject parseObject(const std::string& s, int& i);
JsonArray parseArray(const std::string& s, int& i);

void skipSpace(const std::string& s, int& i) {
    while (i < s.size() && std::isspace(s[i])) i++;
}

std::string extractString(const std::string &s, int &i) {
    std::string result = "";
    i++;
    while (i < s.size() && s[i] != '"') {
        result += s[i];
        i++;
    }
    i++;
    return result;
}

std::string extractNumber(const std::string &s, int &i) {
    std::string result = "";
    while (i < s.size() && (std::isdigit(s[i]) || s[i] == '.')) {
        result += s[i];
        i++;
    }
    return result;
}

JsonMap parseValue(const std::string& s, int& i) {
    skipSpace(s, i);
    if (s[i] == '"') return extractString(s, i);
    if (s[i] == '{') return parseObject(s, i);
    if (s[i] == '[') return parseArray(s, i);
    if (std::isdigit(s[i])) return extractNumber(s, i);
    return s;
}

JsonObject parseObject(const std::string& s, int& i) {
    JsonObject obj;
    i++;
    while (i < s.size() && s[i] != '}') {
        skipSpace(s, i);
        if (s[i] == '"') {
            std::string key = extractString(s, i);
            while (i < s.size() && s[i] != ':') i++;
            i++;
            obj[key] = parseValue(s, i);
        } else {
            i++;
        }
        skipSpace(s, i);
        if (s[i] == ',') i++;
    }
    i++;
    return obj;
}

JsonArray parseArray(const std::string& s, int& i) {
    JsonArray arr;
    i++;
    while (i < s.size() && s[i] != ']') {
        skipSpace(s, i);
        if (s[i] != ']') {
            arr.push_back(parseValue(s, i));
        }
        skipSpace(s, i);
        if (s[i] == ',') i++;
    }
    i++;
    return arr;
}

std::string FileRead(std::string file_name) {
    std::ifstream file(file_name);
    if (!file.is_open()) {
        return "Error: Could not open file!";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main() {
    std::string rawJson = FileRead("json_file.json");

    int index = 0;
    JsonObject data = parseObject(rawJson, index);

    std::cout << "Name: " << data["name"].asString() << std::endl;
    std::cout << "Age: " << data["age"].asString() << std::endl;

    if (data["friends"].isArray()) {
        JsonArray& friends = data["friends"].asArray();
        std::cout << "friends: " << friends[0].asObject()["name"].asString() << std::endl;
        std::cout << "friends: " << friends[1].asObject()["name"].asString() << std::endl;
    }

    return 0;
}
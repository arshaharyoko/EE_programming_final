#ifndef JSON_PARSER_H
#define JSON_PARSER_H
#include <string>
#include <vector>
#include <iomanip>
#include <fstream>
#include <iostream>

typedef struct data {
    std::string function;
    std::string label;
} data;

class JSONobject{
    private:
        std::vector<data> dataVector;
    public:
        std::string openFile(std::string fileName);
        void parseJSON(std::string stringJSON);
};

#endif
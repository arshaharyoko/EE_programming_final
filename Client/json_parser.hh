#ifndef JSON_PARSER_H
#define JSON_PARSER_H
#include <string>
#include <vector>
#include <iomanip>
#include <fstream>
#include <iostream>

typedef struct Data {
    std::string function;
    std::string label;
};

class JSONDataObject{
    private:
        std::vector<Data> dataVector;
    public:
        std::string openFile(std::string fileName);
        void parseJSON(std::string stringJSON);
        int get_label_idx(std::string label); // Linear search O(1)... O(n) apabila user diberi opsi untuk memasukkan fungsi
        // idx: function's key index
        std::string get_function(int idx);
};

#endif
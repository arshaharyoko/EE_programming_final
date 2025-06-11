#include "json_parser.hh"

std::string JSONDataObject::get_function(int idx) {
    return dataVector[idx].function;
}

int JSONDataObject::get_label_idx(std::string label) {
    for(int i=0;i!=dataVector.size();i++) {
        if(dataVector[i].label==label) {
            return i;
        }
    }
    throw std::runtime_error("Invalid function label.");
}

std::string JSONDataObject::openFile(std::string fileName){
    FILE* file = fopen(fileName.c_str(), "r");
    if (!file) {
        throw std::runtime_error("Gagal membuka file: " + fileName);
    }

    char buffer[1024];
    std::string result;
    size_t bytesRead;

    while ((bytesRead = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        result.append(buffer, bytesRead);
    }

    fclose(file);
    return result;
}

void JSONDataObject::parseJSON(std::string stringJSON) {
    size_t pos = 0;

    while ((pos = stringJSON.find("\"function\"", pos)) != std::string::npos) {
        Data d;

        size_t start = stringJSON.find(":", pos) + 1;
        start = stringJSON.find("\"", start) + 1;
        size_t end = stringJSON.find("\"", start);
        d.function = stringJSON.substr(start, end - start);
        pos = end;

        pos = stringJSON.find("\"label\"", pos);
        if (pos == std::string::npos) break;
        start = stringJSON.find(":", pos) + 1;
        start = stringJSON.find("\"", start) + 1;
        end = stringJSON.find("\"", start);
        d.label = stringJSON.substr(start, end - start);

        dataVector.push_back(d);
        pos = end;
    }
}
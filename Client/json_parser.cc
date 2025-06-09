#include "json_parser.hh"

std::string JSONobject::openFile(std::string fileName){
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

void JSONobject::parseJSON(std::string stringJSON){
    size_t pos = 0;

    while ((pos = stringJSON.find("\"func\"", pos)) != std::string::npos) {
        data d;

        size_t start = stringJSON.find(":", pos) + 1;
        start = stringJSON.find("\"", start) + 1;
        size_t end = stringJSON.find("\"", start);
        d.function = stringJSON.substr(start, end - start);

        pos = stringJSON.find("\"x0\"", end);
        start = stringJSON.find(":", pos) + 1;
        end = stringJSON.find(",", start);
        d.x0 = std::stod(stringJSON.substr(start, end - start));

        pos = stringJSON.find("\"y0\"", end);
        start = stringJSON.find(":", pos) + 1;
        end = stringJSON.find("}", start);
        d.y0 = std::stod(stringJSON.substr(start, end - start));

        dataVector.push_back(d);

        pos = end;
    }
}
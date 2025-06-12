#include "./Client/json_parser.cc"
#include <fstream>
#include <vector>
#include <algorithm>
#include <iostream>

struct Record {
    std::string function;
    double solution;
};

int partition(std::vector<Record>& arr, int low, int high, bool longest) {
    double pivot = arr[high].solution;
    int i = low - 1;
    for(int j=low;j<=high-1;j++) {
        if(arr[j].solution < pivot) {
            i++;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return (i + 1);
}

void quickSort(std::vector<Record>& arr, int low, int high, bool longest) {
    if (low < high) {
        int pivot = partition(arr, low, high, longest);
        quickSort(arr, low, pivot - 1, longest);
        quickSort(arr, pivot + 1, high, longest);
    }
}

static std::string find_label(JSONDataObject& js, const std::string& func) {
    std::string file = js.openFile("./formulas.json");
    size_t pos = 0;
    while ((pos = file.find("\"function\"", pos)) != std::string::npos) {
        size_t start = file.find("\"", file.find(':', pos)) + 1;
        size_t end = file.find("\"", start);
        std::string f = file.substr(start, end - start);
        if (f == func) {
            // find label next
            size_t lp = file.find("\"label\"", end);
            size_t ls = file.find("\"", file.find(':', lp)) + 1;
            size_t le = file.find("\"", ls);
            return file.substr(ls, le - ls);
        }
        pos = end;
    }
    return "<unknown>";
}

int main(int argc, char* argv[]) {
    bool longest = (argc>1 && std::string(argv[1])=="--longest");

    std::ifstream in("records.bin", std::ios::binary);
    if(!in) {
        std::cerr << "Failed to open records.bin\n";
        return 1;
    }

    std::vector<Record> recs;
    while (true) {
        int len;
        if (!in.read(reinterpret_cast<char*>(&len), sizeof(len))) break;
        std::string buf(len, '\0');
        if (!in.read(&buf[0], len)) break;

        Record r;
        size_t p1 = buf.find("\"function\"");
        size_t s1 = buf.find('"', buf.find(':', p1)) + 1;
        size_t e1 = buf.find('"', s1);
        r.function = buf.substr(s1, e1 - s1);
        size_t p2 = buf.find("\"solution\"");
        size_t s2 = buf.find(':', p2) + 1;
        size_t e2 = buf.find_first_of(",}\n", s2);
        r.solution = std::stod(buf.substr(s2, e2 - s2));
        recs.push_back(r);
    }

    // sort by solution
    if(!recs.empty()) {
        quickSort(recs, 0, recs.size()-1, longest);
    }

    JSONDataObject js;
    // header
    std::cout << (longest ? "Labels by longest solution time:\n" : "Labels by shortest solution time:\n");
    for(auto& r : recs) {
        std::string label = find_label(js, r.function);
        std::cout << label << ": " << r.solution << "\n";
    }
    return 0;
}

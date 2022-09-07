#ifndef CSV_READER
#define CSV_READER

#include <string>
#include <vector>

/*
 * A class to read data from a csv file.
 */
class CSVReader {
    std::string fileName;
    char delimeter;
    bool header;

   public:
    // object constructor
    CSVReader(std::string filename, char delm = ',', bool hdr = true) : fileName(filename), delimeter(delm), header(hdr) {
    }

    // Function to fetch data from a CSV File
    std::vector<std::vector<std::string>> getData();
};

#endif
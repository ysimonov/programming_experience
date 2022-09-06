#include <cstring>
#include <fstream>
#include <iostream>
#include <istream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

/*
 * A class to read data from a csv file.
 */
class CSVReader
{
    std::string fileName;
    char delimeter;
    bool header;

  public:
    // object constructor
    CSVReader(std::string filename, char delm = ',', bool hdr = true) : fileName(filename), delimeter(delm), header(hdr)
    {
    }

    // Function to fetch data from a CSV File
    std::vector<std::vector<std::string>> getData();
};

/*
 * To detect and ignore byte order mark character
 */
bool SkipBOM(std::istream &in)
{
    char test[4] = {0};
    in.read(test, 3);
    if (strcmp(test, "\xEF\xBB\xBF") == 0)
        return true;
    in.seekg(0);
    return false;
}

/*
 * Parses through csv file line by line and returns the data
 * in vector of vector of strings.
 */
std::vector<std::vector<std::string>> CSVReader::getData()
{

    std::ifstream file(fileName, std::ios::in);

    // if file does not exist
    if (!file.is_open())
    {
        throw std::runtime_error("Could not open file");
    }

    std::vector<std::vector<std::string>> data_list;
    std::string line = "";
    std::string val;
    std::string colname;
    std::vector<std::string> column_names;

    // Read the column names
    if (header)
    {

        if (file.good())
        {

            // Extract the first line in the file
            std::getline(file, line);

            // Create a stringstream from line
            std::stringstream ss(line);

            SkipBOM(ss);

            // Extract each column name
            while (std::getline(ss, colname, delimeter))
            {
                column_names.emplace_back(colname);
            }
        }

        std::cout << "Column names: " << std::endl;
        for (const auto &column_name : column_names)
        {
            std::cout << column_name << " ";
        }
        std::cout << std::endl;
    }

    // Iterate through each line and split the content using delimeter
    while (std::getline(file, line))
    {

        // Create a stringstream of the current line
        std::stringstream ss(line);

        // Extract each string
        std::vector<std::string> row;

        // Track column index
        int col_idx = 0;

        while (std::getline(ss, val, delimeter))
        {
            row.emplace_back(val);
        }

        // check if the row is not empty
        if (!row.empty())
            data_list.emplace_back(row);
    }

    // Close the File
    file.close();

    return data_list;
}

int main()
{

    // std::string filename = "file.csv";
    // CSVReader reader(filename, ',', true);

    std::string filename = "MOCK_DATA.csv";
    CSVReader reader(filename, ',', true);

    // get data from csv file
    try
    {
        std::vector<std::vector<std::string>> data_list = reader.getData();

        // print contents of the file row by row
        for (const auto &row : data_list)
        {
            for (const auto &str : row)
            {
                std::cout << str << " ";
            }
            std::cout << std::endl;
        }
    }
    catch (std::runtime_error)
    {
        std::cout << "File " << filename << " does not exist!\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
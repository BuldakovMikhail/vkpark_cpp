#define ARGUMENTS_ERROR 101
#define FILE_DOES_NOT_EXIST 102
#define FILE_ERROR 11
#define AKAS_HEADER_ERROR 12
#define BASICS_HEADER_ERROR 13
#define RATINGS_HEADER_ERROR 14

#define RATING_MIN_VOTES_NUM 1000

#include <iostream>
#include <cstring>
#include <fstream>
#include <vector>
#include <sstream>
#include <stdexcept>
#include <set>
#include <map>

void stringSplitByTabs(std::vector<std::string> &arr, std::string str)
{
    std::istringstream strStream(str);
    std::string token;

    while (getline(strStream, token, '\t'))
        arr.push_back(token);
}

class Akas
{
public:
    explicit Akas(const char *filename) : file(filename),
                                          titleIdIndex(-1),
                                          languageIndex(-1),
                                          regionIndex(-1),
                                          titleIndex(-1)
    {
        if (!file.is_open())
            throw std::runtime_error("Could not open file");
        parseHeader();
    }
    std::vector<std::string> findNextRussian();

private:
    void parseHeader();
    int titleIdIndex;
    int titleIndex;
    int languageIndex;
    int regionIndex;

    std::ifstream file;
};

void Akas::parseHeader()
{
    std::string header;

    file.clear();
    file.seekg(0);

    if (!std::getline(file, header))
        throw std::runtime_error("Could not open file");

    std::vector<std::string> headerSplit;
    stringSplitByTabs(headerSplit, header);

    for (int i = 0; i < headerSplit.size(); ++i)
    {
        if (headerSplit[i] == "titleId")
            titleIdIndex = i;
        else if (headerSplit[i] == "language")
            languageIndex = i;
        else if (headerSplit[i] == "region")
            regionIndex = i;
        else if (headerSplit[i] == "title")
            titleIndex = i;
    }

    if (titleIdIndex == -1 || languageIndex == -1 || regionIndex == -1 || titleIndex == -1)
        throw std::runtime_error("Error in file");
}

std::vector<std::string> Akas::findNextRussian()
{
    std::string line;

    while (std::getline(file, line))
    {
        std::vector<std::string> splitedLine;
        stringSplitByTabs(splitedLine, line);

        if (splitedLine[languageIndex] == "RU" || splitedLine[regionIndex] == "RU")
            return {splitedLine[titleIdIndex], splitedLine[titleIndex]};
    }

    return {};
}

class Basics
{
public:
    explicit Basics(const char *filename) : file(filename),
                                            tconstIndex(-1),
                                            titleTypeIndex(-1),
                                            isAdultIndex(-1),
                                            runtimeMinutesIndex(-1)
    {
        if (!file.is_open())
            throw std::runtime_error("Could not open file");
        parseHeader();
    }

    bool checkMovie(std::string id, int runtime);

private:
    void parseHeader();
    int tconstIndex;
    int titleTypeIndex;
    int isAdultIndex;
    int runtimeMinutesIndex;

    std::ifstream file;
};

void Basics::parseHeader()
{
    std::string header;

    file.clear();
    file.seekg(0);

    if (!std::getline(file, header))
        throw std::runtime_error("Could not open file");

    std::vector<std::string> headerSplit;
    stringSplitByTabs(headerSplit, header);

    for (int i = 0; i < headerSplit.size(); ++i)
    {
        if (headerSplit[i] == "titleType")
            titleTypeIndex = i;
        else if (headerSplit[i] == "tconst")
            tconstIndex = i;
        else if (headerSplit[i] == "isAdult")
            isAdultIndex = i;
        else if (headerSplit[i] == "runtimeMinutes")
            runtimeMinutesIndex = i;
    }

    if (titleTypeIndex == -1 || tconstIndex == -1 || isAdultIndex == -1 || runtimeMinutesIndex == -1)
        throw std::runtime_error("Error in file");
}

class Ratings
{
public:
    explicit Ratings(const char *filename) : file(filename),
                                             tconstIndex(-1),
                                             averageRatingIndex(-1),
                                             numVotesIndex(-1)
    {
        if (!file.is_open())
            throw std::runtime_error("Could not open file");
        parseHeader();
    }

    double getRating(std::string id);

private:
    void parseHeader();

    int tconstIndex;
    int averageRatingIndex;
    int numVotesIndex;

    std::ifstream file;
};

void Ratings::parseHeader()
{
    std::string header;

    file.clear();
    file.seekg(0);

    if (!std::getline(file, header))
        throw std::runtime_error("Could not open file");

    std::vector<std::string> headerSplit;
    stringSplitByTabs(headerSplit, header);

    for (int i = 0; i < headerSplit.size(); ++i)
    {
        if (headerSplit[i] == "tconst")
            tconstIndex = i;
        else if (headerSplit[i] == "averageRating")
            averageRatingIndex = i;
        else if (headerSplit[i] == "numVotes")
            numVotesIndex = i;
    }

    if (averageRatingIndex == -1 || tconstIndex == -1 || numVotesIndex == -1)
        throw std::runtime_error("Error in file");
}

bool Basics::checkMovie(std::string id, int runtime)
{
    std::string line;
    while (std::getline(file, line))
    {
        std::vector<std::string> splitedLine;
        stringSplitByTabs(splitedLine, line);

        if (splitedLine[tconstIndex] == id)
        {
            if (splitedLine[isAdultIndex] == "0" &&
                splitedLine[titleTypeIndex] == "tvSeries" &&
                std::stoi(splitedLine[runtimeMinutesIndex].c_str()) < runtime)
            {
                return true;
            }

            break;
        }
    }

    return false;
}

double Ratings::getRating(std::string id)
{
    int rating = -1;

    std::string line;
    while (std::getline(file, line))
    {
        std::vector<std::string> splitedLine;
        stringSplitByTabs(splitedLine, line);

        if (splitedLine[tconstIndex] == id)
        {
            if (std::stod(splitedLine[numVotesIndex]) > RATING_MIN_VOTES_NUM)
                rating = std::stoi(splitedLine[averageRatingIndex]);

            break;
        }
    }

    return rating;
}

int insertInSorted(std::vector<double> &arr, double value)
{
    if (arr.empty())
        arr.push_back(value);
    else
    {
        for (int i = 0; i < arr.size(); ++i)
        {
            if (arr[i] > value)
            {
                arr.insert(arr.begin() + i, value);
                break;
            }
        }
    }
}

std::vector<std::string> find10Lines(Akas &akas,
                                     Basics &basics,
                                     Ratings &ratings,
                                     int runtime)
{
    std::vector<double> ratingsVec;
    std::vector<std::string> titles;

    std::vector<std::string> curLine = akas.findNextRussian(); // id, title

    while (!curLine.empty())
    {
        if (basics.checkMovie(curLine[0], runtime))
        {
            int rating = ratings.getRating(curLine[0]);
            if (rating > 0 && ratingsVec.size() < 10)
            {
                int pos = insertInSorted(ratingsVec, rating);
                titles.insert(titles.begin() + pos, curLine[1]);
            }
            else if (rating > 0 && ratingsVec.size() >= 10 && rating > ratingsVec[0])
            {
                ratingsVec.erase(ratingsVec.begin());
                titles.erase(titles.begin());

                int pos = insertInSorted(ratingsVec, rating);
                titles.insert(titles.begin() + pos, curLine[1]);
            }
        }
        curLine = akas.findNextRussian();
    }

    return titles;
}

void printVector(std::vector<std::string> vector)
{
    for (int i = 0; i < vector.size(); ++i)
        std::cout << vector[i] << std::endl;
}

// akas - title language, basics - runtime and isadult, ratings
int main(int argc, char const *argv[])
{
    int akasIndex = -1;
    int basicsIndex = -1;
    int ratingsIndex = -1;

    int runtime = -1;
    int i = 1;

    if (argc != 8)
        return ARGUMENTS_ERROR;

    while (i < argc)
    {
        if (strcmp(argv[i], "-akas") == 0)
        {
            akasIndex = argc != i + 1 ? i + 1 : -1;
            i += 2;
        }
        else if (strcmp(argv[i], "-basics") == 0)
        {
            basicsIndex = argc != i + 1 ? i + 1 : -1;
            i += 2;
        }
        else if (strcmp(argv[i], "-ratings") == 0)
        {
            ratingsIndex = argc != i + 1 ? i + 1 : -1;
            i += 2;
        }
        else
        {
            try
            {
                runtime = std::stoi(argv[i]);
            }
            catch (...)
            {
                // cout << "Ошибка во введенных аргументах\n";
                return ARGUMENTS_ERROR;
            }
            i++;
        }
    }

    if (akasIndex == -1 || basicsIndex == -1 || ratingsIndex == -1)
        return ARGUMENTS_ERROR;

    try
    {
        Akas akas(argv[akasIndex]);
        Basics basics(argv[basicsIndex]);
        Ratings ratings(argv[ratingsIndex]);

        std::vector<std::string> arr;
        arr = find10Lines(akas, basics, ratings, runtime);

        printVector(arr);
    }
    catch (...)
    {
        return FILE_ERROR;
    }

    return EXIT_SUCCESS;
}

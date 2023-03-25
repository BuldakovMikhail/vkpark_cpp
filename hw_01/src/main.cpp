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

struct akas_t
{
    int titleIdIndex = -1;
    int languageIndex = -1;
    int regionIndex = -1;

    std::ifstream file;
};

struct basics_t
{
    int tconstIndex;
    int titleTypeIndex;
    int isAdultIndex;
    int runtimeMinutesIndex;

    std::ifstream file;
};

struct ratings_t
{
    int tconstIndex;
    int averageRatingIndex;
    int numVotesIndex;

    std::ifstream file;
};

void stringSplitByTabs(std::vector<std::string> &arr, std::string str)
{
    std::istringstream strStream(str);
    std::string token;

    while (getline(strStream, token, '\t'))
        arr.push_back(token);
}

int parseHeaderAkas(akas_t &akas, std::string header)
{
    std::vector<std::string> headerSplit;
    stringSplitByTabs(headerSplit, header);

    int titleIdIndex = -1;
    int languageIndex = -1;
    int regionIndex = -1;

    for (int i = 0; i < headerSplit.size(); ++i)
    {
        if (headerSplit[i] == "titleId")
            titleIdIndex = i;
        else if (headerSplit[i] == "language")
            languageIndex = i;
        else if (headerSplit[i] == "region")
            regionIndex = i;
    }

    if (titleIdIndex == -1 || languageIndex == -1 || regionIndex == -1)
        return AKAS_HEADER_ERROR;

    akas.titleIdIndex = titleIdIndex;
    akas.languageIndex = languageIndex;
    akas.regionIndex = regionIndex;

    return EXIT_SUCCESS;
}

int parseHeaderBasics(basics_t &basics, std::string header)
{
    std::vector<std::string> headerSplit;
    stringSplitByTabs(headerSplit, header);

    int titleTypeIndex = -1;
    int tconstIndex = -1;
    int isAdultIndex = -1;
    int runtimeMinutesIndex = -1;

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
        return BASICS_HEADER_ERROR;

    basics.titleTypeIndex = titleTypeIndex;
    basics.tconstIndex = tconstIndex;
    basics.isAdultIndex = isAdultIndex;
    basics.runtimeMinutesIndex = runtimeMinutesIndex;

    return EXIT_SUCCESS;
}

int parseHeaderRatings(ratings_t &ratings, std::string header)
{
    std::vector<std::string> headerSplit;
    stringSplitByTabs(headerSplit, header);

    int tconstIndex = -1;
    int averageRatingIndex = -1;
    int numVotesIndex = -1;

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
        return RATINGS_HEADER_ERROR;

    ratings.tconstIndex = tconstIndex;
    ratings.averageRatingIndex = averageRatingIndex;
    ratings.numVotesIndex = numVotesIndex;

    return EXIT_SUCCESS;
}

bool checkMovie(basics_t &basics, int runtime, std::string id)
{
    basics.file.clear();
    basics.file.seekg(0);

    std::string line;
    while (std::getline(basics.file, line))
    {
        std::vector<std::string> splitedLine;
        stringSplitByTabs(splitedLine, line);

        if (splitedLine[basics.tconstIndex] == id)
        {
            if (splitedLine[basics.isAdultIndex] == "0" &&
                splitedLine[basics.titleTypeIndex] == "tvseries" &&
                std::stoi(splitedLine[basics.runtimeMinutesIndex].c_str()) < runtime)
            {
                return true;
            }

            break;
        }
    }

    return false;
}

int getRating(ratings_t &ratings, std::string id)
{
    ratings.file.clear();
    ratings.file.seekg(0);

    int rating = -1;

    std::string line;
    while (std::getline(ratings.file, line))
    {
        std::vector<std::string> splitedLine;
        stringSplitByTabs(splitedLine, line);

        if (splitedLine[ratings.tconstIndex] == id)
        {
            if (std::stoi(splitedLine[ratings.numVotesIndex]) > RATING_MIN_VOTES_NUM)
                rating = std::stoi(splitedLine[ratings.averageRatingIndex]);

            break;
        }
    }

    return rating;
}

int find10Lines(std::vector<std::string> &arr, std::ifstream &akas,
                std::ifstream &ratings, std::ifstream &basics, int runtime)
{
    if (!(akas.is_open() && ratings.is_open() && basics.is_open()) || runtime <= 0)
        return FILE_ERROR;

    std::string header;
    int rc = 0;

    akas.clear();
    akas.seekg(0);

    if (!std::getline(akas, header))
        return FILE_ERROR;

    akas_t akasStruct;

    if ((rc = parseHeaderAkas(akasStruct, header)))
        return rc;

    basics.clear();
    basics.seekg(0);

    if (!getline(basics, header))
        return FILE_ERROR;

    if ((rc = parseHeaderBasics(tconstBasicsIndex, titleTypeIndex, isAdultIndex, runtimeMinutesIndex, header)))
        return rc;

    ratings.clear();
    ratings.seekg(0);

    if (!getline(ratings, header))
        return FILE_ERROR;

    int averageRatingIndex = -1;
    int tconstRatingsIndex = -1;
    int numVotesIndex = -1;

    if ((rc = parseHeaderRatings(tconstRatingsIndex, averageRatingIndex, numVotesIndex, header)))
        return rc;

    return EXIT_SUCCESS;
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
    {
        // cout << "Ошибка во введенных аргументах\n";
        return ARGUMENTS_ERROR;
    }

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
    {
        // cout << "Ошибка во введенных аргументах\n";
        return ARGUMENTS_ERROR;
    }

    // cout << argv[akasIndex] << '\n';
    // cout << argv[basicsIndex] << '\n';
    // cout << argv[ratingsIndex] << '\n';

    std::ifstream akasFile(argv[akasIndex]);
    if (!akasFile.is_open())
        return FILE_DOES_NOT_EXIST;

    std::ifstream basicsFile(argv[basicsIndex]);
    if (!basicsFile.is_open())
    {
        akasFile.close();
        return FILE_DOES_NOT_EXIST;
    }

    std::ifstream ratingsFile(argv[ratingsIndex]);
    if (!ratingsFile.is_open())
    {
        akasFile.close();
        basicsFile.close();
        return FILE_DOES_NOT_EXIST;
    }

    std::vector<std::string> arr;
    find10Lines(arr, akasFile, ratingsFile, basicsFile, runtime);

    return EXIT_SUCCESS;
}

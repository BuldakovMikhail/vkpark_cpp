#define ARGUMENTS_ERROR 101
#define FILE_DOES_NOT_EXIST 102

#define RUNTIME_ERROR 10

#define BASICS_ID_INDEX 0
#define BASICS_TYPE_INDEX 1
#define BASICS_TITLE_INDEX 2
#define BASICS_IS_ADULT_INDEX 4
#define BASICS_RUNTIME_INDEX 7

#define RATINGS_ID_INDEX 0
#define RATINGS_RATING_INDEX 1
#define RATINGS_NUM_VOTES_INDEX 2

#define DATA_ID_INDEX 0
#define DATA_PARENT_ID_INDEX 1

#define AKAS_ID_INDEX 0
#define AKAS_TITLE_INDEX 2
#define AKAS_REGION_INDEX 3

#define RATING_MIN_VOTES_NUM 1000

#include <iostream>
#include <cstring>
#include <fstream>
#include <vector>
#include <sstream>
#include <stdexcept>
#include <set>
#include <map>
#include <algorithm>

// akas - titleId, ordering, title, region, language, types, attributes, isOriginalTitle
// basics - tconst, titleType, primaryTitle, originalTytle, isAdult, startYear, endYear, runtimeMinutes, genres
// ratings - tconst, averageRating, numVotes
// data - tconst, parentTconst, seasonNumber, episodeNumber

void stringSplitByTabs(std::vector<std::string> &arr, std::string str)
{
    std::istringstream strStream(str);
    std::string token;

    while (getline(strStream, token, '\t'))
        arr.push_back(token);
}

int getRating(std::ifstream &ratings, std::string id)
{
    int rating = -1;

    std::string line;
    while (std::getline(ratings, line))
    {
        std::vector<std::string> splitedLine;
        stringSplitByTabs(splitedLine, line);

        if (splitedLine[RATINGS_ID_INDEX] == id)
        {
            if (std::stod(splitedLine[RATINGS_NUM_VOTES_INDEX]) > RATING_MIN_VOTES_NUM)
                rating = std::stoi(splitedLine[RATINGS_RATING_INDEX]);

            break;
        }

        if (splitedLine[RATINGS_ID_INDEX] > id)
        {
            ratings.clear();
            ratings.seekg(0);
            break;
        }
    }

    return rating;
}

int getRuntime(std::ifstream &basics, std::string id)
{
    std::string line;
    int runtime = -1;

    while (std::getline(basics, line))
    {
        std::vector<std::string> splitedLine;
        stringSplitByTabs(splitedLine, line);

        if (splitedLine[BASICS_ID_INDEX] == id)
        {
            runtime = std::stoi(splitedLine[BASICS_RUNTIME_INDEX]);
            break;
        }
    }

    return runtime;
}

int sumRuntime(std::ifstream &basics, std::ifstream &data, std::string id)
{
    std::string line;

    int sumRuntime = 0;

    while (std::getline(data, line))
    {
        std::vector<std::string> splitedLine;
        stringSplitByTabs(splitedLine, line);

        if (splitedLine[DATA_PARENT_ID_INDEX] == id)
        {
            int runtime = getRuntime(basics, splitedLine[DATA_ID_INDEX]);
            if (runtime > 0)
                sumRuntime += runtime;
            else
                return RUNTIME_ERROR;
        }
    }

    return sumRuntime;
}

std::string getTitle(std::ifstream &akas, std::string id)
{
    std::string line;

    while (std::getline(akas, line))
    {
        std::vector<std::string> splitedLine;
        stringSplitByTabs(splitedLine, line);

        if (splitedLine[AKAS_ID_INDEX] == id && splitedLine[AKAS_REGION_INDEX] == "RU")
            return splitedLine[AKAS_TITLE_INDEX];

        if (splitedLine[AKAS_ID_INDEX] > id)
        {
            akas.clear();
            akas.seekg(0);

            break;
        }
    }

    return "";
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

std::vector<std::string> find10Series(std::ifstream &akas,
                                      std::ifstream &basics,
                                      std::ifstream &ratings,
                                      std::ifstream &data,
                                      int runtime)
{
    std::vector<double> ratingsVec;
    std::vector<std::string> titles;
    std::vector<std::string> ids;

    std::string line;

    while (std::getline(basics, line))
    {
        std::vector<std::string> splitedLine;
        stringSplitByTabs(splitedLine, line);

        if (splitedLine[BASICS_TYPE_INDEX] == "tvSeries" && splitedLine[BASICS_IS_ADULT_INDEX] == "0")
        {
            std::string title = getTitle(akas, splitedLine[BASICS_ID_INDEX]);
            if (title == "")
                title = splitedLine[BASICS_TITLE_INDEX];

            if (std::find(titles.begin(), titles.end(), title) != titles.end())
                continue;

            int rating = getRating(ratings, splitedLine[BASICS_ID_INDEX]);
            if (rating < 0)
                continue;

            int curRuntime = sumRuntime(basics, data, splitedLine[BASICS_ID_INDEX]);
            if (curRuntime <= 0 && curRuntime > runtime)
                continue;

            if (ratingsVec.size() < 10)
            {
                int pos = insertInSorted(ratingsVec, rating);
                titles.insert(titles.begin() + pos, title);
                ids.insert(ids.begin() + pos, splitedLine[BASICS_ID_INDEX]);
            }
            else if (ratingsVec.size() >= 10 && rating > ratingsVec[0])
            {
                ratingsVec.erase(ratingsVec.begin());
                titles.erase(titles.begin());
                ids.erase(ids.)

                    int pos = insertInSorted(ratingsVec, rating);
                titles.insert(titles.begin() + pos, title);
                ids.insert(ids.begin() + pos, splitedLine[BASICS_ID_INDEX]);
            }
        }
    }

    return titles;
}

void printVector(std::vector<std::string> vector)
{
    for (int i = 0; i < vector.size(); ++i)
        std::cout << vector[i] << std::endl;
}

int main(int argc, char const *argv[])
{
    int akasIndex = -1;
    int basicsIndex = -1;
    int ratingsIndex = -1;
    int dataIndex = -1;

    int runtime = -1;
    int i = 1;

    if (argc != 10)
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
        else if (strcmp(argv[i], "-data") == 0)
        {
            dataIndex = argc != i + 1 ? i + 1 : -1;
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

    if (akasIndex == -1 || basicsIndex == -1 || ratingsIndex == -1 || dataIndex == -1)
        return ARGUMENTS_ERROR;

    std::ifstream akasFile(argv[akasIndex]);
    std::ifstream basicsFile(argv[basicsIndex]);
    std::ifstream ratingsFile(argv[ratingsIndex]);
    std::ifstream dataFile(argv[dataIndex]);

    if (!(akasFile.is_open() && basicsFile.is_open() &&
          ratingsFile.is_open() && dataFile.is_open()))
        return FILE_DOES_NOT_EXIST;

    try
    {
        std::vector<std::string> arr = find10Series(akasFile, basicsFile, ratingsFile, dataFile, runtime);
        printVector(arr);
    }
    catch (...)
    {
        return FILE_DOES_NOT_EXIST;
    }

    return EXIT_SUCCESS;
}
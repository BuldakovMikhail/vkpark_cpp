#include "indexes.h"
#include "string_processing.h"

#include "file_handlers.h"

std::map<std::string, int> filterByRuntime(std::map<std::string, int> dictionary, int maxRuntime)
{
    std::map<std::string, int> newDictionary;
    for (auto x : dictionary)
    {
        if (x.second <= maxRuntime)
            newDictionary.insert(x);
    }

    return newDictionary;
}

std::vector<std::string> getLineFromBasics(std::ifstream &basics, std::string id)
{
    std::string line;

    while (std::getline(basics, line))
    {
        std::vector<std::string> splitedLine = stringSplitByTabs(line);

        if (splitedLine.at(BASICS_ID_INDEX) == id)
            return splitedLine;
    }

    return {};
}

double getRating(std::ifstream &ratings, std::string id)
{
    double rating = -1;

    std::string line;
    auto prevPos = ratings.tellg();

    while (std::getline(ratings, line))
    {
        std::vector<std::string> splitedLine = stringSplitByTabs(line);

        if (splitedLine.at(RATINGS_ID_INDEX) == id)
        {
            if (splitedLine.at(RATINGS_NUM_VOTES_INDEX) != "\\N" && std::stod(splitedLine.at(RATINGS_NUM_VOTES_INDEX)) > RATING_MIN_VOTES_NUM)
            {
                if (splitedLine.at(RATINGS_RATING_INDEX) != "\\N")
                    rating = std::stod(splitedLine.at(RATINGS_RATING_INDEX));
            }

            break;
        }

        if (splitedLine.at(RATINGS_ID_INDEX) > id)
        {
            ratings.clear(); // If id doesn't founded make backstep
            ratings.seekg(prevPos);
            break;
        }

        prevPos = ratings.tellg();
    }

    return rating;
}

std::string getTitle(std::ifstream &akas, std::string id)
{
    std::string line;
    auto prevPos = akas.tellg();

    while (std::getline(akas, line))
    {
        std::vector<std::string> splitedLine = stringSplitByTabs(line);

        if (splitedLine.at(AKAS_ID_INDEX) == id && splitedLine.at(AKAS_REGION_INDEX) == "RU")
            return splitedLine.at(AKAS_TITLE_INDEX);

        if (splitedLine.at(AKAS_ID_INDEX) > id)
        {
            akas.clear(); // If id doesn't founded make backstep
            akas.seekg(prevPos);
            break;
        }

        prevPos = akas.tellg();
    }

    return "";
}

int getRuntime(std::ifstream &basics, std::string id)
{
    std::string line;
    int runtime = -1;

    auto prevPos = basics.tellg();

    while (std::getline(basics, line))
    {
        std::vector<std::string> splitedLine = stringSplitByTabs(line);

        if (splitedLine.at(BASICS_ID_INDEX) == id)
        {
            if (splitedLine.at(BASICS_RUNTIME_INDEX) != "\\N")
                runtime = std::stoi(splitedLine.at(BASICS_RUNTIME_INDEX));
            break;
        }

        if (splitedLine.at(BASICS_ID_INDEX) > id)
        {
            basics.clear(); // If id doesn't founded make back step
            basics.seekg(prevPos);
            break;
        }

        prevPos = basics.tellg();
    }

    return runtime;
}

bool checkHeaderAkas(std::ifstream &akas)
{
    std::string temp;
    if (!std::getline(akas, temp))
        return false;

    std::vector<std::string> splitedLine = stringSplitByTabs(temp);
    if (splitedLine.at(AKAS_ID_INDEX) != "titleId" ||
        splitedLine.at(AKAS_REGION_INDEX) != "region" ||
        splitedLine.at(AKAS_TITLE_INDEX) != "title")
        return false;
    else
        return true;
}

bool checkHeaderBasics(std::ifstream &basics)
{
    std::string temp;
    if (!std::getline(basics, temp))
        return false;

    std::vector<std::string> splitedLine = stringSplitByTabs(temp);
    if (splitedLine.at(BASICS_ID_INDEX) != "tconst" ||
        splitedLine.at(BASICS_RUNTIME_INDEX) != "runtimeMinutes" ||
        splitedLine.at(BASICS_IS_ADULT_INDEX) != "isAdult" ||
        splitedLine.at(BASICS_TITLE_INDEX) != "primaryTitle")
        return false;
    else
        return true;
}

bool checkHeaderRatings(std::ifstream &ratings)
{
    std::string temp;
    if (!std::getline(ratings, temp))
        return false;

    std::vector<std::string> splitedLine = stringSplitByTabs(temp);
    if (splitedLine.at(RATINGS_ID_INDEX) != "tconst" ||
        splitedLine.at(RATINGS_NUM_VOTES_INDEX) != "numVotes" ||
        splitedLine.at(RATINGS_RATING_INDEX) != "averageRating")
        return false;
    else
        return true;
}

bool checkHeaderData(std::ifstream &data)
{
    std::string temp;
    if (!std::getline(data, temp))
        return false;

    std::vector<std::string> splitedLine = stringSplitByTabs(temp);
    if (splitedLine.at(DATA_ID_INDEX) != "tconst" ||
        splitedLine.at(DATA_PARENT_ID_INDEX) != "parentTconst")
        return false;
    else
        return true;
}

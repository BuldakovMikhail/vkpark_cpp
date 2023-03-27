#include "indexes.h"
#include "file_handlers.h"
#include "string_processing.h"
#include "vector_operations.h"

#include "files_handlers.h"
#include "exception.h"

std::vector<std::string> find10Series(std::ifstream &akas,
                                      std::ifstream &basics,
                                      std::ifstream &ratings,
                                      std::ifstream &data,
                                      int runtime)
{

    std::vector<double> ratingsVec;  // Sorted current the biggest ratings
    std::vector<std::string> titles; // Current titles

    // Check headers
    if (!checkHeaderRatings(ratings))
        throw arguments_exception("Неверный файл");
    if (!checkHeaderAkas(akas))
        throw arguments_exception("Неверный файл");
    if (!checkHeaderBasics(basics))
        throw arguments_exception("Неверный файл");
    if (!checkHeaderData(data))
        throw arguments_exception("Неверный файл");

    std::map<std::string, int> dictionary = createSeriesRuntimeMap(data, basics); // Using map for optimization reasons
    dictionary = filterByRuntime(dictionary, runtime);

    for (auto x : dictionary)
    {
        std::vector<std::string> basicLine = getLineFromBasics(basics, x.first);

        if (basicLine.empty())
            continue;

        if (basicLine.at(BASICS_IS_ADULT_INDEX) == "0")
        {
            double rating = getRating(ratings, x.first);

            if (rating < 0)
                continue;

            if (ratingsVec.size() < 10)
            {
                std::string title = getTitle(akas, x.first); // find title only when it's needed
                if (title.empty())
                    title = basicLine.at(BASICS_TITLE_INDEX);

                int pos = insertInSorted(ratingsVec, rating);
                titles.insert(titles.begin() + pos, title);
            }
            else if (ratingsVec.size() >= 10 && rating > ratingsVec[0])
            {
                std::string title = getTitle(akas, x.first);
                if (title.empty())
                    title = basicLine.at(BASICS_TITLE_INDEX);

                ratingsVec.erase(ratingsVec.begin());
                titles.erase(titles.begin());

                int pos = insertInSorted(ratingsVec, rating);
                titles.insert(titles.begin() + pos, title);
            }
        }
    }
    return titles;
}

std::map<std::string, int> createSeriesRuntimeMap(std::ifstream &data, std::ifstream &basics)
{
    std::map<std::string, int> dictionary;

    std::streampos dataPos = data.tellg(); // Save carriage position
    std::streampos basicsPos = basics.tellg();

    std::string line;

    while (std::getline(data, line))
    {
        std::vector<std::string> splitedLine = stringSplitByTabs(line);

        int runtime = getRuntime(basics, splitedLine.at(DATA_ID_INDEX));
        if (runtime < 0)
            continue;

        if (dictionary.find(splitedLine.at(DATA_PARENT_ID_INDEX)) == dictionary.end())
            dictionary[splitedLine.at(DATA_PARENT_ID_INDEX)] = runtime;
        else
            dictionary[splitedLine.at(DATA_PARENT_ID_INDEX)] += runtime;
    }

    // Return files on passed condition
    data.clear();
    data.seekg(dataPos);

    basics.clear();
    basics.seekg(basicsPos);

    return dictionary;
}

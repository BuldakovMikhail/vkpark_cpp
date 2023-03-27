/*
Вывести русские названия (при наличии) 10 самых рейтинговых сериалов, суммарная длительность всех эпизодов которого не превышает числа минут, переданного в аргументах командной строки
*/

const int ARGUMENTS_ERROR = 101;
const int FILE_DOES_NOT_EXIST = 102;
const int FILE_ERROR = 103;

#include <iostream>
#include <vector>

#include "string_processing.h"
#include "files_handlers.h"
#include "vector_operations.h"
#include "arguments_handler.h"

// akas - titleId, ordering, title, region, language, types, attributes, isOriginalTitle
// basics - tconst, titleType, primaryTitle, originalTitle, isAdult, startYear, endYear, runtimeMinutes, genres
// ratings - tconst, averageRating, numVotes
// data - tconst, parentTconst, seasonNumber, episodeNumber

int main(int argc, char const *argv[])
{
    Arguments arguments;

    try{
        arguments = parseArguments(argc, argv);
    }
    catch(...)
    {
        std::cerr << "Ошибка в переданных аргументах" << std::endl;
        return ARGUMENTS_ERROR;
    }


    std::ifstream akasFile(arguments.akas);
    std::ifstream basicsFile(arguments.basics);
    std::ifstream ratingsFile(arguments.ratings);
    std::ifstream dataFile(arguments.data);

    if (!(akasFile.is_open() && basicsFile.is_open() &&
          ratingsFile.is_open() && dataFile.is_open()))
    {
        std::cerr << "Некорректные файлы" << std::endl;
        return FILE_DOES_NOT_EXIST;
    }

    try
    {
        std::vector<std::string> arr = find10Series(akasFile, basicsFile, ratingsFile, dataFile, arguments.runtime);
        if (arr.empty())
        {
            std::cerr << "Переданы неккорректные файлы" << std::endl;
            return FILE_ERROR;
        }
        else
            printVector(arr);
    }
    catch (...)
    {
        std::cerr << "Ошибка внутри файла" << std::endl;
        return FILE_ERROR;
    }
    return EXIT_SUCCESS;
}

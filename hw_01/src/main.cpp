/*
Вывести русские названия (при наличии) 10 самых рейтинговых сериалов, суммарная длительность всех эпизодов которого не превышает числа минут, переданного в аргументах командной строки
*/

#define ARGUMENTS_ERROR 101
#define FILE_DOES_NOT_EXIST 102
#define FILE_ERROR 103
#define EMPTY_RESULT 104

#include <iostream>
#include <cstring>
#include <fstream>
#include <vector>

#include "string_processing.h"
#include "files_handlers.h"
#include "vector_operations.h"

// akas - titleId, ordering, title, region, language, types, attributes, isOriginalTitle
// basics - tconst, titleType, primaryTitle, originalTytle, isAdult, startYear, endYear, runtimeMinutes, genres
// ratings - tconst, averageRating, numVotes
// data - tconst, parentTconst, seasonNumber, episodeNumber

int main(int argc, char const *argv[])
{
    int akasIndex = -1;
    int basicsIndex = -1;
    int ratingsIndex = -1;
    int dataIndex = -1;

    int runtime = -1;
    int i = 1;

    if (argc != 10)
    {
        std::cout << "Передано неверное количество аргументов" << std::endl;
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
                std::cout << "Ошибка во введенных аргументах" << std::endl;
                return ARGUMENTS_ERROR;
            }
            i++;
        }
    }

    if (akasIndex == -1 || basicsIndex == -1 || ratingsIndex == -1 || dataIndex == -1)
    {
        std::cout << "Передано неверное количество файлов" << std::endl;
        return ARGUMENTS_ERROR;
    }

    if (runtime <= 0)
    {
        std::cout << "Неверно передано максимальное время" << std::endl;
        return ARGUMENTS_ERROR;
    }

    std::ifstream akasFile(argv[akasIndex]);
    std::ifstream basicsFile(argv[basicsIndex]);
    std::ifstream ratingsFile(argv[ratingsIndex]);
    std::ifstream dataFile(argv[dataIndex]);

    if (!(akasFile.is_open() && basicsFile.is_open() &&
          ratingsFile.is_open() && dataFile.is_open()))
    {
        std::cout << "Некорректные файлы" << std::endl;
        return FILE_DOES_NOT_EXIST;
    }

    try
    {
        std::vector<std::string> arr = find10Series(akasFile, basicsFile, ratingsFile, dataFile, runtime);
        if (arr.empty())
        {
            std::cout << "Ничего не было найдено" << std::endl;
            return EMPTY_RESULT;
        }
        else
            printVector(arr);
    }
    catch (...)
    {
        std::cout << "Ошибка внутри файла" << std::endl;
        return FILE_ERROR;
    }
    return EXIT_SUCCESS;
}
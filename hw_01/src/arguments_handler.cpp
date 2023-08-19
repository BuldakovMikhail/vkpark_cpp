#include <iostream>
#include <cstring>

#include "arguments_handler.h"
#include "exception.h"

Arguments parseArguments(int argc, char const *argv[])
{
    Arguments arguments;

    if (argc != 10)
        throw arguments_exception("Неверноe количество аргументов");

    int akasIndex = -1;
    int basicsIndex = -1;
    int ratingsIndex = -1;
    int dataIndex = -1;

    int runtime = -1;

    int i = 1;
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
            runtime = std::stoi(argv[i]);
            i++;
        }
    }

    if (akasIndex == -1 || basicsIndex == -1 || ratingsIndex == -1 || dataIndex == -1)
        throw arguments_exception("Передано неверное количество файлов");

    if (runtime <= 0)
        throw arguments_exception("Неверно передано максимальное время");


    arguments.runtime = runtime;
    arguments.akas = argv[akasIndex];
    arguments.basics = argv[basicsIndex];
    arguments.data = argv[dataIndex];
    arguments.ratings = argv[ratingsIndex];
    
    return arguments;
}

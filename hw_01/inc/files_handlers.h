#include <fstream>
#include <iostream>
#include <vector>
#include <map>

#ifndef __FILES_HANDLERS_H__
#define __FILES_HANDLERS_H__
std::vector<std::string> find10Series(std::ifstream &akas,
                                      std::ifstream &basics,
                                      std::ifstream &ratings,
                                      std::ifstream &data,
                                      int runtime);

std::map<std::string, int> createSeriesRuntimeMap(std::ifstream &data, std::ifstream &basics);

#endif

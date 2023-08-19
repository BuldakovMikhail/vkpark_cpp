#include <iostream>
#include <vector>
#include <map>
#include <fstream>

#ifndef __FILE_HANDLERS_H__
#define __FILE_HANDLERS_H__

inline const int RATING_MIN_VOTES_NUM = 1000;

std::map<std::string, int> filterByRuntime(std::map<std::string, int> dictionary, int maxRuntime);
std::vector<std::string> getLineFromBasics(std::ifstream &basics, std::string id);
double getRating(std::ifstream &ratings, std::string id);
std::string getTitle(std::ifstream &akas, std::string id);
int getRuntime(std::ifstream &basics, std::string id);

bool checkHeaderAkas(std::ifstream &akas);
bool checkHeaderBasics(std::ifstream &basics);
bool checkHeaderRatings(std::ifstream &ratings);
bool checkHeaderData(std::ifstream &data);

#endif

#include <string>
#include <vector>

#ifndef SERVER_H
#define SERVER_H

std::vector<std::string> parseMessage(std::string message);
std::string operatorName(std::string opcode);
bool searchInFile(std::string filepath, std::string op,
                  std::vector<std::string>* search_list,
                  std::vector<std::string>* found);

bool searchLine(std::string line, std::string op,
                std::vector<std::string>* search_list);

#endif  // SERVER_H
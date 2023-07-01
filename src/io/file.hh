#pragma once

#include <fstream>
#include <string>

class File {
 private:
  std::ofstream f_;

 public:
  void write(std::string);
  File(std::string);
  ~File();
};

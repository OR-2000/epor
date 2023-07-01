#include "file.hh"

File::File(std::string file) { f_.open(file, std::ios::out | std::ios::app); }
File::~File() { f_.close(); }

void File::write(std::string line) { f_ << line << std::endl; };

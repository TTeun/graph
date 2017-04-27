#ifndef EQMAPS__H
#define EQMAPS__H

#include <unordered_map>

class EqMaps{
public:
  typedef double (*f_bin)(double a, double b);
  typedef double (*f_un)(double a);
  typedef std::unordered_map<std::string, f_bin> map_bin;
  typedef std::unordered_map<std::string, f_un> map_un;

  static std::unordered_map<std::string, f_bin> bin_maps;
  static std::unordered_map<std::string, f_un> un_maps;
};






#endif

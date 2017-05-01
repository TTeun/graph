#ifndef EQMAPS__H
#define EQMAPS__H

#include <unordered_map>
#include "../exptree/utils/diff.h"

class EqMaps{
public:
  typedef double (*f_bin)(double a, double b);
  typedef double (*f_un)(double a);

  typedef std::unordered_map<std::string, f_bin> map_bin;
  typedef std::unordered_map<std::string, f_un> map_un;

  static map_bin bin_maps;
  static map_un un_maps;


  typedef ExpTree::Node Node;
  typedef std::unique_ptr<Node> (*f_diff_bin)(Node const *node);
  typedef std::unordered_map<std::string, f_diff_bin> map_bin_diff;

  static map_bin_diff bin_diff_maps;
};

#endif

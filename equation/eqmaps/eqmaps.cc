#include "eqmaps.h"
#include <limits> // NaN
#include <cmath>

using namespace std;

EqMaps::map_bin EqMaps::bin_maps =
  {
    {"+", [](double a, double b){return a + b;}},
    {"-", [](double a, double b){return a - b;}},
    {"*", [](double a, double b){return a * b;}},
    {"/", [](double a, double b){return b == 0 ? numeric_limits<double>::quiet_NaN() : a / b;}},
    {"^", pow}
  };

EqMaps::map_un EqMaps::un_maps =
  {
    {"sin", sin},
    {"cos", cos},
    {"tan", tan},
    {"exp", exp},
    {"-u", [](double a){return -a;}},
    {"log", log}
  };

EqMaps::map_bin_diff EqMaps::bin_diff_maps = {
  {"+", diff::diffPlus},
  {"-", diff::diffMin},
  {"*", diff::diffMul},
  {"^", diff::diffPow},
  {"sin", diff::diffSin},
  {"cos", diff::diffCos},
  {"exp", diff::diffExp},
  {"log", diff::diffLog},
  {"/", diff::diffDiv},
  {"-u", [](Node* node){return node_util::newNode(node->token, diff::differentiateNode(node->right)); }}
};

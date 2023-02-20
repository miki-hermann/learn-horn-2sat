#pragma once

#include <string>
#include "matrix+formula.hpp"
#include "common.hpp"

using namespace std;

//------------------------------------------------------------------------------

typedef unordered_map<Pattern, set<Point>> Bucket;

void insert (const Clause &, Bucket &);
Formula get_formula (const Bucket &);
bool operator|= (const Row &, const Bucket &);
void print_bucket (const Bucket &);

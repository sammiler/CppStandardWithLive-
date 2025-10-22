#pragma once
#include <algorithm>
#include <functional>
#include <list>

inline std::list<std::function<void(void)>> funcList = {};

void initFuncList();

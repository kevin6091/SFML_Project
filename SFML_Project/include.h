#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio/Sound.hpp>

#include <map>	
#include <set>
#include <list>
#include <vector>	
#include <queue>
#include <stack>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>

#include <filesystem>
#include <fstream> 
#include <iostream>

#include "using.h"
#include "enum.h"
#include "util.h"


using namespace std;
using namespace sf;

#define WIDTH 960
#define HEIGHT 540






#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif
#endif
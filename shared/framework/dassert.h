#pragma once
// helper for debug assertions
// in debug mode: tests if expression is true and breaks/pauses the programm otherwise
#ifdef _DEBUG
#define dassert(expression) if(!(expression)) __debugbreak()
#else
#define dassert(expression) 
#endif

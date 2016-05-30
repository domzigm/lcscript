#pragma once

#include <iostream>
#include <string>
#include <stdint.h>
#include <vector>

#include "chaiscript.hpp"
#include "chaiscript_stdlib.hpp"

template <typename T>
void pushStackT(T val)
{
	stack.push_back(std::tuple<const type_info&, T>(typeid(T), val));
}

uint32_t callFunc(chaiscript::ChaiScript& chai, const char* funcName, uint8_t dataLen, uint8_t *dataPtr);

#ifdef __CALLGATE__
#define PUSHSTACK(x) void pushStack_##x(##x val) { pushStackT<##x>(val); }
#else
#define PUSHSTACK(x) extern void pushStack_##x(##x val);
#endif

PUSHSTACK(uint8_t);
PUSHSTACK(uint16_t);
PUSHSTACK(uint32_t);
PUSHSTACK(uint64_t);
PUSHSTACK(int8_t);
PUSHSTACK(int16_t);
PUSHSTACK(int32_t);
PUSHSTACK(int64_t);
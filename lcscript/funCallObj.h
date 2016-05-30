#pragma once

#include <stdint.h>
#include <string>

#include "dyncall.h"
#include "dynload.h"
#include "chaiscript.hpp"
#include "chaiscript_stdlib.hpp"

chaiscript::ModulePtr registerFunCall();

class funCallObj
{
public:
	funCallObj();
	bool       init(DLLib* library, const std::string& symbolName, int callMode);

	void       evalVoid();
	/*void*      evalPointer();*/
	float      evalFloat();
	double     evalDouble();
	int8_t     evalInt8();
	int16_t    evalInt16();
	int32_t    evalInt32();
	int64_t    evalInt64();
	uint8_t    evalUInt8();
	uint16_t   evalUInt16();
	uint32_t   evalUInt32();
	uint64_t   evalUInt64();

	/*void       argPointer(void* arg);*/
	void       argFloat(float arg);
	void       argDouble(double arg);
	void       argInt8(int8_t arg);
	void       argInt16(int16_t arg);
	void       argInt32(int32_t arg);
	void       argInt64(int64_t arg);
	void       argUInt8(uint8_t arg);
	void       argUInt16(uint16_t arg);
	void       argUInt32(uint32_t arg);
	void       argUInt64(uint64_t arg);

private:
	DCCallVM*	vm;
	DCpointer	func;
};

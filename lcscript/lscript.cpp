#include <stdint.h>
#include <string>
#include <vector>

#include "dyncall.h"
#include "dynload.h"

#include "chaiscript.hpp"
#include "chaiscript_stdlib.hpp"

#include "funCallObj.h"
#include "callGate.h"

chaiscript::ChaiScript chai(chaiscript::Std_Lib::library());

DLLib* chai_LoadLib(const std::string &lib)
{
	return dlLoadLibrary(lib.c_str());
}

funCallObj* chai_InitFunCall(DLLib* libPtr, const std::string& function, int callMode)
{
	funCallObj* obj = NULL;

	if (libPtr) {
		
		funCallObj* obj = new funCallObj();
		if (obj) {

			if (false == obj->init(libPtr, function, callMode)) {

				std::cout << "Failed to load function " << function.c_str() << std::endl;
				delete obj;
				obj = NULL;
			}
		}
	}
	return obj;
}

void chai_printUint32Hex(uint32_t var)
{
	std::printf("%08x\r\n", var);
}

void chai_printUint64Hex(uint64_t var)
{
	std::printf("%016I64x\r\n", var);
}

void chai_dbgAddr(DLLib* library)
{
	std::printf("Library address: %p\r\n", library);
}

UINT_PTR chai_uintPtr(chaiscript::Boxed_Value& bv)
{
	if (bv.get_type_info().bare_equal_type_info(typeid(std::string)))
	{
		const std::string& str = chaiscript::boxed_cast<const std::string&>(bv);
		return (UINT_PTR)str.c_str();
	}
	else if (bv.get_type_info().bare_equal_type_info(typeid(std::vector<chaiscript::Boxed_Value>)))
	{
#ifdef _DEBUG
		auto vec = chaiscript::boxed_cast<std::vector<chaiscript::Boxed_Value>&>(bv);
		for (auto& i : vec)
		{
			auto a = i.get_const_ptr();

			if (i.get_type_info().bare_equal_type_info(typeid(char)))
			{
				std::printf("%p = %hhd\r\n", a, *(int8_t*)i.get_const_ptr());
			}
			else if (i.get_type_info().bare_equal_type_info(typeid(unsigned char)))
			{
				std::printf("%p = %hhu\r\n", a, *(uint8_t*)i.get_const_ptr());
			}
			else if (i.get_type_info().bare_equal_type_info(typeid(short)))
			{
				std::printf("%p = %hu\r\n", a, *(int16_t*)i.get_const_ptr());
			}
			else if (i.get_type_info().bare_equal_type_info(typeid(unsigned short)))
			{
				std::printf("%p = %hu\r\n", a, *(uint16_t*)i.get_const_ptr());
			}
			else if (i.get_type_info().bare_equal_type_info(typeid(unsigned int)) || 
				i.get_type_info().bare_equal_type_info(typeid(unsigned long)))
			{
				std::printf("%p = %u\r\n", a, *(uint32_t*)i.get_const_ptr());
			}
			else if (i.get_type_info().bare_equal_type_info(typeid(int)) || 
					 i.get_type_info().bare_equal_type_info(typeid(long)))
			{
				std::printf("%p = %d\r\n", a, *(int32_t*)i.get_const_ptr());
			}
			else if (i.get_type_info().bare_equal_type_info(typeid(unsigned long long)) ||
				     i.get_type_info().bare_equal_type_info(typeid(unsigned long int)))
			{
				std::printf("%p = %llu\r\n", a, *(uint64_t*)i.get_const_ptr());
			}
			else if (i.get_type_info().bare_equal_type_info(typeid(long long)) ||
					 i.get_type_info().bare_equal_type_info(typeid(long int)))
			{
				std::printf("%p = %lld\r\n", a, *(int64_t*)i.get_const_ptr());
			}
		}
#endif
		return (UINT_PTR)0;
	}

	return chaiscript::boxed_cast<UINT_PTR>(bv);
}

void chai_pushArray(funCallObj* funCall, chaiscript::Boxed_Value& bv)
{
	if (bv.get_type_info().bare_equal_type_info(typeid(std::vector<chaiscript::Boxed_Value>)))
	{
		auto vec = chaiscript::boxed_cast<std::vector<chaiscript::Boxed_Value>&>(bv);
		for (auto& entry : vec)
		{
			if (entry.get_type_info() == typeid(char) ||
				entry.get_type_info() == typeid(unsigned char))
			{
				funCall->argUInt8(*(uint8_t*)entry.get_const_ptr());
			}
			else if (entry.get_type_info() == typeid(short) ||
				entry.get_type_info() == typeid(unsigned short))
			{
				funCall->argUInt16(*(uint16_t*)entry.get_const_ptr());
			}
			else if (entry.get_type_info() == typeid(long) ||
				entry.get_type_info() == typeid(int) ||
				entry.get_type_info() == typeid(unsigned long) ||
				entry.get_type_info() == typeid(unsigned int))
			{
				funCall->argUInt32(*(uint32_t*)entry.get_const_ptr());
			}
			else if (entry.get_type_info() == typeid(long long) ||
				entry.get_type_info() == typeid(long int) ||
				entry.get_type_info() == typeid(unsigned long long) ||
				entry.get_type_info() == typeid(unsigned long int))
			{
				funCall->argUInt64(*(uint64_t*)entry.get_const_ptr());
			}
			else if (entry.get_type_info() == typeid(std::string))
			{
				const std::string& str = chaiscript::boxed_cast<const std::string&>(bv);
				funCall->argUInt32((uint32_t)str.c_str());
			}
			/*else if (entry.get_type_info() == typeid(float))
			{
				funCall->argFloat(*(float*)entry.get_const_ptr());
			}
			else if (entry.get_type_info() == typeid(double))
			{
				funCall->argDouble(*(double*)entry.get_const_ptr());
			}*/
		} 
	}
}

int main(int argc, char** argv)
{
	chai.add(chaiscript::user_type<DLLib*>(), "libPtr");

	chai.add(chaiscript::fun(&chai_LoadLib), "LoadLibrary");
	chai.add(chaiscript::fun(&chai_dbgAddr), "DbgAddr");
	chai.add(chaiscript::fun(&chai_uintPtr), "uintPtr");
	chai.add(chaiscript::fun(&chai_pushArray), "pushArray");
	chai.add(chaiscript::fun(&chai_printUint32Hex), "printUint32Hex");
	chai.add(chaiscript::fun(&chai_printUint64Hex), "printUint64Hex");

	chai.add(registerFunCall());
	
	chai.eval(R"(
		var libUser = LoadLibrary("user32.dll");
		var MsgBox = funCall();
		MsgBox.init(libUser, "MessageBoxA", 2);
	)");

	chai.eval(R"(
		var text = "This is some crazy stuff\r\n"
		puts("User32.dll ");
		DbgAddr(libUser);
		
		var addr = uintPtr(text);
		puts("Address of text ");
		printUint32Hex(addr);

		var an_array = [uint8_t(1), -2u, 3ul, 4ull];
		printUint32Hex(uintPtr(an_array));
	)");

	chai.eval(R"(
		MsgBox.argUInt32(0);
		MsgBox.argUInt32(uintPtr("Hallo Welt"));
		MsgBox.argUInt32(uintPtr("Chai Demo"));
		MsgBox.argUInt32(0x00000004ul);
		var result = MsgBox.evalInt32();
		puts("MessageBox result: ");
		puts(result);
		puts("\r\n");
	)");

	auto ret1 = chai.eval(R"(
		def myfun(int a, int b) { return a + b; }
	)");

	pushStack_uint16_t(123);
	pushStack_int16_t(-53);
	int32_t data = 0;
	auto ret = callFunc(chai, "myfun", sizeof(data), (uint8_t*)&data);
	std::cout << "Result of myfun: " << data << std::endl;

	return 0;
}

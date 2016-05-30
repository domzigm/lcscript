#define __CALLGATE__
#include "callGate.h"

std::vector<std::tuple<const type_info&, uint64_t>> stack;

uint32_t callFunc(chaiscript::ChaiScript& chai, const char* funcName, uint8_t dataLen, uint8_t *dataPtr)
{
	uint32_t retVal = 0;
	if (funcName == NULL || dataLen == NULL || dataPtr == NULL || *funcName == NULL) {
		return 0;
	}

	std::stringstream stream;

	stream << funcName << "(";
	for (auto& arg : stack)
	{
		std::string cast = "0";
		auto& type = std::get<0>(arg);
		auto& val = std::get<1>(arg);

		cast = std::to_string(static_cast<uint8_t>(val));

		if (type == typeid(uint8_t)) {
			cast = std::to_string((uint8_t)val);
		}
		else if (type == typeid(uint16_t)) {
			cast = std::to_string((uint16_t)val);
		}
		else if (type == typeid(uint32_t)) {
			cast = std::to_string((uint32_t)val);
		}
		else if (type == typeid(uint64_t)) {
			cast = std::to_string((uint64_t)val);
		}
		else if (type == typeid(int8_t)) {
			cast = std::to_string((int8_t)val);
		}
		else if (type == typeid(int16_t)) {
			cast = std::to_string((int16_t)val);
		}
		else if (type == typeid(int32_t)) {
			cast = std::to_string((int32_t)val);
		}
		else if (type == typeid(int64_t)) {
			cast = std::to_string((int64_t)val);
		}
		/*
		else if (type == typeid(float)) {
			cast = std::to_string((float)val);
		}
		else if (type == typeid(double)) {
			cast = std::to_string((double)val);
		}
		*/
		stream << cast << ",";
	}
	stream.seekp(-1, std::ios_base::end);
	stream << ");";

	try
	{
		auto ret = chai.eval(stream.str());
		memcpy(dataPtr, ret.get_const_ptr(), dataLen);
		retVal = 1;
	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}
	catch (const std::string& e) {
		std::cout << e << std::endl;
	}
	catch (...) {
		std::cout << "Uncatchable exception occured" << std::endl;
	}

	stack.clear();

	return retVal;
}
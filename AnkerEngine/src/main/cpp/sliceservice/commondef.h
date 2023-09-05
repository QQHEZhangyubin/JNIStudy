#pragma once
#include <string>
#include <unordered_map>

namespace akss
{
	typedef std::unordered_map<std::string, std::string> ParamMap;
	typedef std::unordered_map <std::string, ParamMap> Type2ParamsMap;
}

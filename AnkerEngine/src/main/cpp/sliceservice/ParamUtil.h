#pragma once

#include <string>
#include <vector>
#include "commondef.h"

namespace akss
{
	#define GEN_TYPE_KEY(MachineType, NozzleSize, MaterialType, PrintMode) (MachineType+"_"+NozzleSize+"_"+MaterialType+"_"+PrintMode)

	class ParamUtil
	{
	public:
		//ÒÔASCIIÂë±£´æ
		static bool saveModelDataToFile(const std::string& fileName, const float* vert, const size_t vertNum);

		static bool loadIniFile(const std::string& iniFile, ParamMap& paramsMap);
		static bool getFileNames(const std::string& path, std::vector<std::string>& filenames);
		static bool loadParameters(const std::string& path, const ParamMap& parentParamsMap, Type2ParamsMap& paramsMap);

		//Base 64
		static std::string Base64Encode(const std::string& sInput);
		static std::string Base64Decode(const std::string& sInput);
	};
}


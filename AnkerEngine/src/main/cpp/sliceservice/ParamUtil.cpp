#include "AnkerCloudSlice.h"
#include "ParamUtil.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include <boost/filesystem.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/transform_width.hpp>

namespace akss
{
	bool ParamUtil::loadIniFile(const std::string& iniFile, ParamMap& paramMap)
	{
		try
		{
			boost::property_tree::ptree s;
			boost::property_tree::ini_parser::read_ini(iniFile, s);

			//遍历
			for (auto& node : s)
			{
				auto items = node.second;
				for (auto& item : items)
				{
					auto key = item.first;
					auto value = item.second.get_value<std::string>();

					if (paramMap.count(key) > 0){
						paramMap[key] = value;
					}
					else{
						paramMap.emplace(key, value);
					}
				}
			}
		}
		catch (const std::exception& error)
		{
			std::cout << __FUNCTION__ << error.what() << std::endl;
			return false;
		}

		return true;
	}

	bool ParamUtil::loadParameters(const std::string& path, const ParamMap& parentParamMap, Type2ParamsMap& type2paramsMap)
	{
		std::vector<std::string> fileNames;
		if (!getFileNames(path, fileNames) || fileNames.empty())
		{
			std::cout << "Failed to traversal folder" << std::endl;
			return false;
		}

		ParamMap paramMap;
		for (auto& fileName : fileNames) {

			paramMap = parentParamMap;
			if (!loadIniFile(path + fileName, paramMap)) {
				std::cout << "Failed to load file:" << fileName << std::endl;
				continue;
			}

			auto typeKey = GEN_TYPE_KEY(paramMap["meta_current_machine_name"], paramMap["machine_nozzle_size"],
				paramMap["meta_current_material_name"], paramMap["param_print_mode"]);

			type2paramsMap.emplace(typeKey, paramMap);
		}

		return true;
	}

	bool ParamUtil::getFileNames(const std::string& filePath, std::vector<std::string>& filenames)
	{
		try
		{
			//std::cout << filePath << std::endl;
			boost::filesystem::path path(filePath);
			for (const auto& iter : boost::filesystem::directory_iterator(path))
			{
				//std::cout << iter.path() << std::endl;
				if (boost::filesystem::is_directory(iter.path()))//过滤子文件夹
					continue;

				auto sFile = iter.path().filename().string();
				filenames.push_back(sFile);
				//std::cout << sFile << std::endl;
			}
			return true;
		}
		catch (const std::exception& error)
		{
			std::cout << __FUNCTION__ << error.what() << std::endl;
			return false;
		}

		return false;
	}

	bool ParamUtil::saveModelDataToFile(const std::string& fileName, const float* vert, const size_t vertNum)
	{
		assert(vert != NULL);
		assert(vertNum % 9 == 0);

		std::ofstream ofs;

		ofs.open(fileName, std::ios::out | std::ios::trunc);
		if (!ofs.is_open())
		{
			std::cout << "Failed to create file:" << fileName << std::endl;
			return false;
		}

		ofs << "solid anker temp model " << boost::posix_time::to_iso_string(boost::posix_time::second_clock::local_time()) << std::endl;

		auto p = vert;
		int faceSize = vertNum / 9;

		for (int i = 0; i < faceSize; i++)
		{
			ofs << "facet normal 0.0 0.0 0.0" << std::endl;
				ofs << "\touter loop" << std::endl;
				for (int j = 0; j < 3; j++)
				{
					ofs << "\t\tvertex ";
					ofs << *p++ << " " << * p++ << " " << *p++ << std::endl;
				}
				ofs << "\tend loop" << std::endl;
			ofs << "endfacet" << std::endl;
		}

		ofs.close();

		return true;
	}

	//encode
	std::string ParamUtil::Base64Encode(const std::string& sInput)
	{
		typedef boost::archive::iterators::base64_from_binary<boost::archive::iterators::transform_width<std::string::const_iterator, 6, 8> > Base64EncodeIterator;
		std::stringstream result;
		std::copy(Base64EncodeIterator(sInput.begin()), Base64EncodeIterator(sInput.end()), std::ostream_iterator<char>(result));
		size_t equal_count = (3 - sInput.length() % 3) % 3;
		for (size_t i = 0; i < equal_count; i++)
		{
			result.put('=');
		}

		return result.str();
	}



	//decode
	std::string ParamUtil::Base64Decode(const std::string& sInput)
	{
		typedef boost::archive::iterators::transform_width<boost::archive::iterators::binary_from_base64<std::string::const_iterator>, 8, 6> Base64DecodeIterator;
		std::stringstream result;

		try
		{
			std::string temp = sInput;
			int endIndex = temp.size() - 2;
			if (temp[endIndex] == '=')
			{
				temp.erase(endIndex);
			}

			endIndex = temp.size() - 1;
			if (temp[endIndex] == '=')
			{
				temp.erase(endIndex);
			}

			copy(Base64DecodeIterator(temp.begin()), Base64DecodeIterator(temp.end()), std::ostream_iterator<char>(result));
		}
		catch (...)
		{
			return std::string();
		}

		return result.str();
	}
}



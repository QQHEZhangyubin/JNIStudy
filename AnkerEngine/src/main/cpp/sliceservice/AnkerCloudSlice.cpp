#include "AnkerCloudSlice.h"
#include "ParamUtil.h"
#include "ProcessHelper.h"

#include <assert.h>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include "../AOsLog.h"

//boost begin
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/locale/encoding.hpp>
//boost end

#define ADD_DOUBLE_QUOTATION_MARK(str) ("\""+str+"\"")

namespace akss
{
	PROGRESS_CALLBACK AkSliceTracer::progressCallback = nullptr;
	MESSAGE_CALLBACK AkSliceTracer::messageCallback = nullptr;
	FAILED_CALLBACK AkSliceTracer::failedCallback = nullptr;
	SUCESSED_CALLBACK AkSliceTracer::sucessedCallback = nullptr;

	AnkerCloudSlicer::AnkerCloudSlicer(const std::string& configPath):
		m_process(true)
		, m_configPath(configPath)
	{
	}

	AnkerCloudSlicer::~AnkerCloudSlicer()
	{
	}

	bool AnkerCloudSlicer::setWorkPath(const std::string& workPath)
	{
		assert(!workPath.empty());
		m_workPath = workPath;

		//�ȼ���һ����ȫ����
		m_defaultParamMap.clear();
		if (!ParamUtil::loadIniFile( m_configPath + "/parameter/realtime/realtimeprofile.ini", m_defaultParamMap)){
			return false;
		}

		//Ȼ��������úõĲ������и��Ǹ���
		m_type2ParamsMap.clear();
		if (!ParamUtil::loadParameters(m_configPath + "/parameter/default/", m_defaultParamMap, m_type2ParamsMap)) {
			return false;
		}

		return true;
	}

	bool AnkerCloudSlicer::setParam(const std::string& paramName, const std::string& paramVal)
	{
		if (m_userParamMap.count(paramName) > 0){
			m_userParamMap[paramName] = paramVal;
		}
		else{
			m_userParamMap.emplace(paramName, paramVal);
		}

		return true;
	}

	bool AnkerCloudSlicer::setGCodeOutFileName(const std::string& gcodeName)
	{
		if (gcodeName.empty()){
			return false;
		}

		m_gcodeName = m_workPath + "/" + gcodeName;

		return true;
	}

	bool AnkerCloudSlicer::addMesh(const std::string& meshName, const float* matrix)
	{
		MeshInfo meshInfo;
		meshInfo.fileName = m_workPath + "/" + meshName;
		meshInfo.isPathFormUser = true;

		boost::format fmt("[[%1%,%2%,%3%],[%4%,%5%,%6%],[%7%,%8%,%9%]]");
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				fmt % matrix[i + j * 4];
			}
		}
		meshInfo.matrix = fmt.str();

		m_meshList.push_back(meshInfo);

		return true;
	}

	bool AnkerCloudSlicer::addMesh(const float* vert, const size_t vertNum)
	{
		//[[1,0,0], [0,1,0], [0,0,1]]
		boost::uuids::uuid a_uuid = boost::uuids::random_generator()();
		std::string uuid_string = boost::uuids::to_string(a_uuid);

		MeshInfo meshInfo;
		meshInfo.fileName = m_workPath + "/" + uuid_string + ".stl";
		meshInfo.isPathFormUser = false;

		if (!ParamUtil::saveModelDataToFile(meshInfo.fileName, vert, vertNum)) {
			return false;
		}

		meshInfo.matrix = "[[1,0,0],[0,1,0],[0,0,1]]";

		m_meshList.push_back(meshInfo);

		return true;
	}

	bool AnkerCloudSlicer::startSlice(AkSliceTracer* slicetracer)
	{
		if (!setGCodeOutFileName("anker.gcode")) {
			OS_LOG("------>>【%s】 error-0", __FUNCTION__);
			return false;
		}

		std::string cmdFileName = genCmdFileName();
		if (!saveArgumentsToFile(cmdFileName)) {
			std::cout << "Failed to save arghuments file" << std::endl;
			OS_LOG("------>>【%s】 error-1, cmdFileName = %s", __FUNCTION__, cmdFileName.c_str());

			return false;
		}

		if (slicetracer == nullptr) {
			slicetracer = this;
		}

		return m_process.start(m_workPath, cmdFileName, slicetracer);
	}

	void AnkerCloudSlicer::clearCache()
	{
		for (auto& meshInfo : m_meshList) {
			if (!meshInfo.isPathFormUser) {
				boost::filesystem::remove(meshInfo.fileName);
			}
		}

		m_meshList.clear();
	}

	bool AnkerCloudSlicer::saveArgumentsToFile(const std::string& cmdFileName)
	{
		auto args = makeArguments();
		if (args.size() < 1) {
			return false;
		}

		//д�����
		{
			//�ϲ�תbase64
			std::string cmdString = ParamUtil::Base64Encode(boost::join(args, " "));
			std::ofstream ofs(cmdFileName, std::ios::out | std::ios::trunc);
			if (!ofs.is_open()) {
				return false;
			}

			ofs << cmdString;
			ofs.close();
		}

		return true;
	}

	std::vector<std::string> AnkerCloudSlicer::makeArguments()
	{
		//�����ַ���
		std::vector<std::string> arguments;
		arguments.reserve(2000);

		arguments.push_back("slice");
		arguments.push_back("-v");
		arguments.push_back("-p");
		arguments.push_back("-j");
		arguments.push_back(m_configPath+"/SliceEngine/config/anker_fdmprinter.def.json");

		//��������
		{
			auto typeKey = GEN_TYPE_KEY(m_userParamMap["meta_current_machine_name"], m_userParamMap["machine_nozzle_size"],
				m_userParamMap["meta_current_material_name"], m_userParamMap["param_print_mode"]);

			if (m_type2ParamsMap.count(typeKey) < 1) {
				std::cout << "Failed to find type setting:" << typeKey << std::endl;
				arguments.clear();
				return arguments;
			}

			const auto& settings = m_type2ParamsMap[typeKey];
			for (auto& item : settings)
			{
				arguments.push_back("-s");
				if (item.second[0] != '"') {
					arguments.push_back(item.first + "=" + ADD_DOUBLE_QUOTATION_MARK(item.second));//����ո񴫲�����Ĵ������
				}
				else {
					arguments.push_back(item.first + "=" + item.second);
				}

			}
		}

		//ģ������
		for (auto& meshInfo : m_meshList)
		{
			arguments.push_back("-s");
			arguments.push_back("mesh_rotation_matrix=" + ADD_DOUBLE_QUOTATION_MARK(meshInfo.matrix));

			arguments.push_back("-l");
#ifdef __linux__
			arguments.push_back(ADD_DOUBLE_QUOTATION_MARK(boost::locale::conv::between(meshInfo.fileName, "UTF-8", "GBK")));
#else
			arguments.push_back(ADD_DOUBLE_QUOTATION_MARK(meshInfo.fileName));
#endif // __linux__
		}

		arguments.push_back("-o");
		arguments.push_back(ADD_DOUBLE_QUOTATION_MARK(m_gcodeName));

		arguments.push_back("-g");

		return arguments;
	}

	std::string AnkerCloudSlicer::genCmdFileName()
	{
		boost::uuids::uuid a_uuid = boost::uuids::random_generator()();
		std::string uuid_string = boost::uuids::to_string(a_uuid);
		std::string cmdFileName = m_workPath + "/" + uuid_string + ".cmd";

		return cmdFileName;
	}

	void AnkerCloudSlicer::progress(float progress)
	{
	      //printf("%s:%f\n", __FUNCTION__, progress);
		if (progressCallback != nullptr) {
			progressCallback(this, progress);
		 }
	}//�������

	void AnkerCloudSlicer::message(const std::string& msg) {
		//printf("%s:%s\n", __FUNCTION__, msg);
		if (messageCallback != nullptr) {
			messageCallback(this, msg.c_str());
		}
	};

	void AnkerCloudSlicer::failed(int errorCode) {
		//printf("%s:%s\n", __FUNCTION__, msg);
		if (failedCallback != nullptr) {
			failedCallback(this, errorCode);
		}
	};

	void AnkerCloudSlicer::success(const SliceResuslt& result) {
		//printf("%s\n", __FUNCTION__);
		if (sucessedCallback != nullptr) {
			sucessedCallback(this, result);
		}
	};//��Ƭ�ɹ�
}

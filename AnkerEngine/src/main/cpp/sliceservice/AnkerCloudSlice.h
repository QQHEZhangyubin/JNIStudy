#include "commondef.h"
#include "typedef.h"
#include "ProcessHelper.h"

#include<string>
#include <list>
#include<vector>
#include<map>
#include<functional>
#include<thread>

namespace akss
{
	class AkSliceTracer
	{
	public:
		virtual void progress(float r) = 0; //报告进度
		virtual void message(const std::string& msg) = 0;//切片信息
		virtual void failed(int errorCode) = 0;//切片中途失败
		virtual void success(const SliceResuslt& result) = 0;//切片成功

		static PROGRESS_CALLBACK progressCallback;
		static MESSAGE_CALLBACK  messageCallback;
		static FAILED_CALLBACK   failedCallback;
		static SUCESSED_CALLBACK sucessedCallback;
	};

	class AnkerCloudSlicer : public AkSliceTracer
	{
		struct MeshInfo
		{
			std::string fileName;
			std::string matrix;
			bool isPathFormUser; //路径是否来自用户,DLL生成临时路径切片完成后会自动删除
		};

	public:
		AnkerCloudSlicer(const std::string& configPath);
		~AnkerCloudSlicer();

		bool setWorkPath(const std::string& workPath);
		bool addMesh(const float* vert, const size_t vertNum);
		bool addMesh(const std::string& meshName,const float* matrix);
		bool setParam(const std::string& paramName, const std::string& paramVal);
		bool startSlice(AkSliceTracer* slicetracer = nullptr);

		inline bool isSlicing() const {
			return m_process.isRunning();
		}

		inline bool stopSlice() {
			return m_process.stop();
		}

		inline bool waitSliceEnd(int msTimeout) {
			return m_process.wait(msTimeout);
		}

		virtual void progress(float progress);
		virtual void message(const std::string& msg);
		virtual void failed(int errorCode);
		virtual void success(const SliceResuslt& result);

	private:
		std::string genCmdFileName();
		bool setGCodeOutFileName(const std::string& path = "");
		std::vector<std::string> makeArguments();
		bool saveArgumentsToFile(const std::string& cmdFileName);
		void clearCache();

	private:
		ProcessHelper m_process;
		std::string m_workPath;
		std::string m_configPath;
		std::string m_gcodeName;
		std::list<MeshInfo> m_meshList;

		//parameters
		Type2ParamsMap m_type2ParamsMap;
		ParamMap m_defaultParamMap;
		ParamMap m_userParamMap;
	};
}

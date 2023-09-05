#pragma once

#include <string>
#include <list>
#include <thread>

#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
//#include <boost/thread/thread.hpp>

#include "typedef.h"

namespace akss
{
	class AkSliceTracer;
	class ProcessHelper
	{
	public:
		ProcessHelper(bool isAsyn = false);
		~ProcessHelper();

		bool start(const std::string& workPath, const std::string& cmdFileName, AkSliceTracer* slicetracer = nullptr);
		bool stop();
		bool isRunning() const;
		bool wait(int msTimeout);

		friend void excute(const std::string& cmdFileName, ProcessHelper* pThis);
		friend void consoleRead(ProcessHelper* pThis);

	private:
		int parseOutputLine(const std::string& line);
		bool parseValue(const std::string& str, float& value);

	private:
		bool m_isAysn; //是否启动异步模式
		bool m_bIsRunning{ false };
		std::string m_workPath;

		SliceResuslt m_result;
		AkSliceTracer* m_slicetracer;

		boost::mutex m_mutex;
		boost::condition m_condition;


		float m_minX{ 0 };
		float m_maxX{ 0 };

		float m_minY{ 0 };
		float m_maxY{ 0 };

		float m_minZ{ 0 };
		float m_maxZ{ 0 };
	};
}



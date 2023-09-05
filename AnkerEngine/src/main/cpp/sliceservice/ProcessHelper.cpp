#include "Application.h"
#include "ProcessHelper.h"
#include "AnkerCloudSlice.h"
#include <iostream>
#include <boost/process.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#ifndef WIN32
	#include <unistd.h>
	#include <fcntl.h>
#endif // !WIN32

namespace akss
{
	namespace  bp = boost::process;

	void consoleRead(ProcessHelper* pThis)
	{
		auto tempFileName = pThis->m_workPath + "/AKSS_CLOUD_SLICER.temp";
		if (boost::filesystem::remove(tempFileName)) {
			std::cout << "Sucessed to remove file:" << tempFileName << std::endl;
		}

		FILE* file_write = freopen(tempFileName.c_str(), "w", stderr);
		if (file_write == NULL) {
			perror("open write failed");
			return;
		}

		FILE* file_read = fopen(tempFileName.c_str(), "r");
		if (file_read == NULL) {
			perror("open read failed");
			return;
		}

#ifndef WIN32
		// Save the old file descriptor
		int save_out = dup(fileno(stderr));

		// Redirect stdout to the file
		if (dup2(fileno(file_write), fileno(stderr)) == -1) {
			perror("dup2 failed");
			return;
		}

		std::cout << "Sucessed to dup2" << std::endl;
#endif // !WIN32

		char c = 0;
		int nErrorCount = 0, nReadBytes = 0;
		char buf[1024 * 10]{ 0 };
		const int MAX_ERROR_COUNT = 1000; //max enable timeout = 10s
		const int MAX_BUF_SIZE = sizeof(buf) - 1;

		while (nErrorCount < MAX_ERROR_COUNT)
		{
			nReadBytes = 0;
			while (read(fileno(file_read), &c, 1) > 0 && c != '\n' && nReadBytes < MAX_BUF_SIZE) {
				buf[nReadBytes++] = c;
			}

			buf[nReadBytes] = '\0';  // Null-terminate the string
			bool read_ok = nReadBytes >0;

			if (read_ok)
			{
				//std::cout << "read ok" << std::endl;
				nErrorCount = 0;
				if (pThis->parseOutputLine(std::string(buf)) != 0) {
					break;
				}
			}
			else
			{
				nErrorCount++;
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}
		}

		if (nErrorCount >= MAX_ERROR_COUNT) {
			if (pThis->m_slicetracer != nullptr) {
				perror("parse timeout");
				pThis->m_slicetracer->failed(-99);
			}
		}

		// Restore stderr
#ifdef WIN32
		freopen("CON", "w", stderr);

#else
		dup2(save_out, fileno(stderr));
		close(save_out);
#endif // WIN32

		fflush(file_write);
		fclose(file_write);
		fclose(file_read);
	}

	void excute(const std::string& cmdFileName, ProcessHelper* pThis)
	{
		boost::unique_lock<boost::mutex> lock(pThis->m_mutex);

		try
		{
			std::vector<std::string> args;
			args.push_back("extParam");
			args.push_back("-f");
			args.push_back(cmdFileName);

			int argc = args.size() + 1;
			char** argv = new char* [argc];
			for (int i = 0; i < argc; i++) {
				argv[i] = new char[256]{ 0 };
				if (i == 0)
				{
					strcpy(argv[i], boost::filesystem::initial_path<boost::filesystem::path>().string().c_str());
				}
				else
				{
					strcpy(argv[i], args[i - 1].c_str());
				}
			}

			std::thread t(consoleRead, pThis);

			cura::Application::getInstance().run(argc, argv);
			for (int i = 0; i < argc; i++) {
				delete []argv[i];
			}

			t.join();
		}
		catch (const boost::process::process_error& e)
		{
			std::cout << "Error: " << e.what() << std::endl;
			if (pThis->m_slicetracer) {
				pThis->m_slicetracer->failed(-2);
			}
		}
		catch (const std::exception& e) {
			std::cout << __FUNCTION__ << e.what() << std::endl;
			if (pThis->m_slicetracer) {
				pThis->m_slicetracer->failed(-3);
			}
		}

		pThis->m_bIsRunning = false;
		pThis->m_condition.notify_all();
	}

	ProcessHelper::ProcessHelper(bool isAsyn):
		m_isAysn(isAsyn)
		, m_slicetracer(nullptr)
	{

	}

	ProcessHelper::~ProcessHelper()
	{

	}

	bool ProcessHelper::start(const std::string& workPath, const std::string& cmdFileName, AkSliceTracer* slicetracer)
	{
		if (isRunning()) {
			return false;
		}

#ifdef DEBUG
		std::cout << __FUNCTION__ << std::endl;
#endif // DEBUG

		m_slicetracer = slicetracer;
		m_bIsRunning = true;
		m_workPath = workPath;

		if (m_isAysn) {
			std::thread t(excute, cmdFileName, this);
			t.detach();
		}
		else {
			excute(cmdFileName, this);
		}		

		return true;
	}

	bool ProcessHelper::stop()
	{
		if (!isRunning()){
			return true;
		}

		boost::unique_lock lock(m_mutex);
		m_condition.wait(lock);

		return true;
	}

	bool ProcessHelper::wait(int msTimeout)
	{
		boost::unique_lock lock(m_mutex);
		bool bRet = true;

		if (msTimeout < 0) {
			m_condition.wait(lock);
		}
		else {
			m_condition.timed_wait(lock, boost::get_system_time() + boost::posix_time::milliseconds(msTimeout));
		}

		return bRet;
	}

	bool ProcessHelper::isRunning() const
	{	
		return m_bIsRunning;
	}

	int ProcessHelper::parseOutputLine(const std::string& line)
	{
		if (m_slicetracer != nullptr) {
			m_slicetracer->message(line.c_str());
		}

		std::string header = line.substr(0, (std::min)((size_t)30, line.length()));
		if (header.find("[ERROR]") != -1) {
			stop();

			if (m_slicetracer != nullptr){
				m_slicetracer->failed(-1);
			}

			return -1;
		}
		else if(header.find("Progress:") != -1){

			int end = line.find("%");
			if (end == -1) {
				return 0;
			}

			int begin = -1;
			for (begin = end - 1; begin >= 0; begin--) {
				if (line[begin] == '\t') {
					break;
				}
			}

			if (begin == -1) {
				return 0;
			}

			std::string progress = line.substr(begin + 1, end-1);
			//std::cout << std::stof(progress) * 100 << std::endl;
			if (m_slicetracer != nullptr) {
				m_slicetracer->progress(std::stof(progress)*100);
			}
		}
		else if (header.find(";MINX:") != -1) {
			parseValue(line, m_minX);
		}
		else if (header.find(";MAXX:") != -1) {
			parseValue(line, m_maxX);
		}
		else if (header.find(";MINY:") != -1) {
			parseValue(line, m_minY);
		}
		else if (header.find(";MAXY:") != -1) {
			parseValue(line, m_maxY);
		}
		else if (header.find(";MINZ:") != -1) {
			parseValue(line, m_minZ);
		}
		else if (header.find(";MAXZ:") != -1) {
			parseValue(line, m_maxZ);
		}
		else if (header.find(";MAXSPEED:") != -1) {
			float speed = 0;
			if (parseValue(line, speed)){
				m_result.maxSpeed = speed / 60;
			}
		}
		else if (header.find("Print time (s):") != -1) {
			float time = 0;
			int index = line.find("Print time (s):");
			if (parseValue(line.substr(index), time)) {
				m_result.printTime = time;
			}
		}
		else if (header.find("Layer count:") != -1) {
			float layerSize = 0;
			int index = line.find("Layer count:");
			if (parseValue(line.substr(index), layerSize)) {
				m_result.layerSize = layerSize;
			}
		}
		else if (header.find(";Filament used:") != -1) {
			float filamentLen = 0;
			auto temp = line;
			temp.erase(remove_if(temp.begin(), temp.end(), [](char c) {
					return c == 'm';
			}), temp.end());

			if (parseValue(temp, filamentLen)) {
				m_result.filamentLen = filamentLen;
			}
		}
		else if (header.find(";Filament weight:") != -1) {
			float filamentWeight = 0;
			auto temp = line;
			temp.erase(remove_if(temp.begin(), temp.end(), [](char c) {
				return c == 'g';
				}), temp.end());

			if (parseValue(temp, filamentWeight)) {
				m_result.filamentWeight = filamentWeight;
			}
		}
		else if (header.find("Filament (mm^3):") != -1){
			m_result.x = m_maxX - m_minX;
			m_result.y = m_maxY - m_minY;
			m_result.z = m_maxZ - m_minZ;

			bool sucessed = m_result.x > 0 && m_result.y > 0 && m_result.z > 0;
			if (m_slicetracer != nullptr) {
				m_slicetracer->success(m_result);
			}

			return 1;
		}

		return 0;
	}

	bool ProcessHelper::parseValue(const std::string& str, float& value)
	{
		try
		{
			std::vector<std::string> stringList;
			boost::split(stringList, str, boost::is_any_of(":"));
			if (stringList.size() != 2) {
				return false;
			}

			auto str = stringList[1];
			str.erase(remove_if(str.begin(), str.end(), [](char c) {
				return c == '\r';
				}), str.end());

			value = std::stof(str);
		}
		catch (const std::exception& e) {
			std::cout << __FUNCTION__ << e.what() << std::endl;
			return false;
		}

		return true;
	}
}
#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include "Globals.h"

class Logger
{
public:
	static Logger* LogInstance;

	inline static Logger* GetLogger() {
		if (Logger::LogInstance == nullptr) {
			return Logger::LogInstance = new Logger();
		}
		else {
			return Logger::LogInstance;
		}
	}

	template<typename...Args>
	inline void LogMaster(const std::string& logType, const std::string& format, Args&&... args)
	{
		std::string CstrnLogType = "[" + logType + "] ";
		printf("[%s] ", logType.c_str());
		printf(format.c_str(), std::forward<Args>(args)...);
		printf("\n");

		char buff[256];

		snprintf(buff, sizeof(buff), CstrnLogType.c_str());
		m_LogString.append(buff);
		snprintf(buff, sizeof(buff), format.c_str(), std::forward<Args>(args)...);
		m_LogString.append(buff);

		m_LogString.append("\n");
	}

	template<typename...Args>
	inline void LogMasterList(const std::string& format, Args&&... args)
	{
		printf("\t");
		printf(format.c_str(), std::forward<Args>(args)...);
		printf("\n");

		char buff[256];

	//	snprintf(buff, sizeof(buff), CstrnLogType.c_str());
	//	m_LogString.append(buff);
		snprintf(buff, sizeof(buff), format.c_str(), std::forward<Args>(args)...);
		m_LogString.append(buff);

		m_LogString.append("\n");
	}

	//Used to log the file into a text file.
	inline void DispenseLog() {
		std::string OutputFileName = "LateralLog " + Utils::currentDateTime() + ".txt";
		std::ofstream OutputFile;
		OutputFile.open(OutputFileName);
		OutputFile << m_LogString;
		OutputFile.close();
	}

private:
	//Masterstring for the whole log;
	std::string m_LogString = "";

};

#define VulkanLog(...) Logger::GetLogger()->LogMaster("Vulkan", __VA_ARGS__);
#define VulkanFatalLog(...) Logger::GetLogger()->LogMaster("Vulkan Fatal", __VA_ARGS__);
#define WarningLog(...) Logger::GetLogger()->LogMaster("Warning", __VA_ARGS__);
#define ErrorLog(...) Logger::GetLogger()->LogMaster("Error", __VA_ARGS__);
#define LogList(...) Logger::GetLogger()->LogMasterList(__VA_ARGS__);
#define Log(...) Logger::GetLogger()->LogMaster("Log", __VA_ARGS__);
#define DispenseLog(...) Logger::GetLogger()->DispenseLog();



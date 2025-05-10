#include <iostream>
#include <fstream>
#include <ctime>
#include <chrono> 
#include <string>
#include <mutex>
#include <stdlib.h>
using namespace std;
//日志级别：调试、信息、警告、错误。
enum class LogLevel
{
	Debug,
	Info,
	Warning,
	Error
};

class  Logger
{
private:
	LogLevel m_logLevel = LogLevel::Debug;
	LogLevel m_currentLevel = LogLevel::Debug;
	ofstream m_logFile;
	static mutex m_mutex;   //互斥锁，保护日志文件的写入
private:
	Logger() {}
	Logger(const Logger&) = delete;
	Logger& operator=(const Logger&) = delete;

	//打开日志文件
	void logMessage(const std::string& msg)
	{
		string timeStr = getCurrentTime();
		cout << timeStr << " [" << getLevelstring(m_currentLevel) << "] " << msg << endl;
		if (m_logFile.is_open()) {
			m_logFile << timeStr << " [" << getLevelstring(m_currentLevel) << "] " << msg << endl;
		}
	}
	//获取当前时间
	string getCurrentTime()const
	{
		auto now = std::chrono::system_clock::now();
		auto time_tNow = std::chrono::system_clock::to_time_t(now);
		struct tm localTime;
		localtime_s(&localTime, &time_tNow);
		char buffer[80];
		strftime(buffer, sizeof(buffer), "%Y-%M-%d  %H:%M:%S", &localTime);
		return std::string(buffer);
	}
	//获取日志级别字符串
	string getLevelstring(LogLevel level)
	{
		switch (level)
		{
		case LogLevel::Debug: return "DEBUG";
		case LogLevel::Info: return "INFO";
		case LogLevel::Warning: return "WARNING";
		case LogLevel::Error: return "ERROR";
		default: return "UNKNOWN";
		}
	}
public:
	//单例模式：获取唯一的实例
	static Logger& getInstance()
	{
		static Logger instance;
		return instance;
	}
	//设置日志级别
	void setLevel(LogLevel level)
	{
		m_logLevel = level;
	}
	//设置日志输出文件
	void setLogFile(const std::string& filename)
	{
		m_logFile.open(filename, ios::app);
		if (!m_logFile.is_open())
		{
			cerr << "ebug: Failed to open log file: " << filename << endl;
		}
	}

	//写入日志，使用哨兵模式
	template <typename T>
	//处理具体日志内容
	Logger& operator<<(const T& msg)
	{
		if(m_currentLevel >= m_logLevel)
		{
			logMessage(msg);
		}
		return *this;
	}
	//设置日志级别
	Logger& operator<<(LogLevel level)
	{
		m_currentLevel = level;
		return *this;
	}
	//处理 std::endl 等流操纵符
		Logger& operator<<(std::ostream& (*manip)(std::ostream&)) {
		if (m_currentLevel >= m_logLevel) {
			// 将操纵符应用到标准输出（或文件流）
			manip(std::cout);
			if (m_logFile.is_open()) {
				manip(m_logFile);
			}
		}
		return *this;
	}
	//关闭日志文件
	~Logger()
	{
		if (m_logFile.is_open())
			m_logFile.close();
	}
};

mutex Logger::m_mutex; //初始化静态成员变量

//哨兵模式辅助写法，用于指定输出的日志级别,明确区分「设置日志级别」和「输出普通消息」
class LogSentinel
{
private:
	LogLevel m_level;
public:
	LogSentinel(LogLevel level) :m_level(level) {}
	friend Logger& operator<<(Logger& logger, const LogSentinel& sentinel);
};
Logger& operator<<(Logger& logger, const LogSentinel& sentinel)
{
	return logger << sentinel.m_level;
}

int main()
{
	Logger& logger = Logger::getInstance();
	//设置日志级别
	logger.setLevel(LogLevel::Debug);

	logger << LogSentinel(LogLevel::Debug) << "This is a debug message which will be printed out" << std::endl;
	logger << LogSentinel(LogLevel::Info) << "This is an info message" << endl;
	logger << LogSentinel(LogLevel::Warning) << "This is a warning message" << endl;
	logger << LogSentinel(LogLevel::Error) << "This is an error message" << endl;

	//设置日志输出文件
	logger.setLogFile("application.log");
	logger << LogSentinel(LogLevel::Warning) << "Logging to file" << endl;

	return 0;

}	

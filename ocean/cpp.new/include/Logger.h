#pragma once
#include <utility>

#include <iostream>
#include <fstream>
using namespace std;

namespace dann5 {
	class Logger
	{
	public:
		static Logger& Instance();

		void log_alloc(void* pObject, const char* oType, bool deallocated = false);
		void log_alloc(void* pObject, const char* oType, const string& txt, bool deallocated = false);

	protected:
	private:
		static auto_ptr<Logger> mpSingleton;

		Logger();
		~Logger();

		ofstream mLogFile;

		friend class auto_ptr<Logger>;
	};
};

#define _lc dann5::Logger::Instance().log_alloc(this, typeid(this).name())
#define _ld dann5::Logger::Instance().log_alloc(this, typeid(this).name(), true)

#define _lct(t) dann5::Logger::Instance().log_alloc(this, typeid(this).name(), t)
#define _ldt(t) dann5::Logger::Instance().log_alloc(this, typeid(this).name(), t, true)
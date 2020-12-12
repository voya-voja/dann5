#include <Logger.h>

using namespace dann5;

auto_ptr<Logger> Logger::mpSingleton;

Logger& Logger::Instance() 
{
	if (mpSingleton.get() == NULL)
		mpSingleton.reset(new Logger);
	return(*mpSingleton);
}

Logger::Logger()
{
	mLogFile.open("dann5_log.csv");
	mLogFile << "Object Type, (de)alloc, Address, Text, CountIf(C:C, C2)" << endl;

	log_alloc(this, typeid(this).name());
}

Logger::~Logger()
{
	log_alloc(this, typeid(this).name(), false);
	mLogFile.close();
}

void Logger::log_alloc(void* pObject, const char* oType, bool deallocated)
{
	mLogFile << oType << ", " << (deallocated ? "deallocate" : "allocate") << ", " 
		<< pObject << endl;
}

void Logger::log_alloc(void* pObject, const char* oType, const string& txt, bool deallocated)
{
	const char* pTxt = txt._Unchecked_begin();
	mLogFile << oType << ", " << (deallocated ? "deallocate" : "allocate") << ", " 
		<< pObject << ", " << pTxt << endl;
}

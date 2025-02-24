#include "Logger.h"

Logger::Logger (const string &logFile)
{
    logStream.open(logFile, ios::app);
    if(!logStream)
    {
        cerr << "Failed to open log file: " << logFile << endl;
    }
}

Logger::~Logger()
{
    if(logStream.is_open())
    {
        logStream.close();
    }
}

void Logger::logInfo(const string &msg)
{
    string output = "[INFO]: " + msg;
    cout << output << endl;
    if(logStream.is_open())
    {
        logStream << output << endl;
    }
}

void Logger::logError(const string &msg)
{
    string output = "[ERROR]: " + msg;
    cout << output << endl;
    if(logStream.is_open())
    {
        logStream << output << endl;
    }
}
void Logger::logSuccess(const string &msg)
{
    string output = "[SUCCESS]: " + msg;
    cout << output << endl;
    if(logStream.is_open())
    {
        logStream << output << endl;
    }
}
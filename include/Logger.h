#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <iostream>
using namespace std;

/**
 * @brief Simple logger class that logs messages to the terminal and to a log file.
 */
class Logger {
public:
    /**
     * @brief Construct a new Logger object.
     * @param logFile Name of the log file.
     */
    Logger(const string &logFile);
    
    /**
     * @brief Destroy the Logger object.
     */
    ~Logger();

    /**
     * @brief Log an informational message.
     * @param msg The message to log.
     */
    void logInfo(const string &msg);

    /**
     * @brief Log an error message.
     * @param msg The message to log.
     */
    void logError(const string &msg);

    /**
     * @brief Log a success message.
     * @param msg The message to log.
     */
    void logSuccess(const string &msg);

private:
    ofstream logStream;  //Output file stream for the log file.
};

#endif
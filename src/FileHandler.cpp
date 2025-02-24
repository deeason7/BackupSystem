#include "FileHandler.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <ctime>
#include <chrono>

namespace fs = std::filesystem;

bool FileHandler::copyFile(const std::string &source, const std::string &destination)
{
    try
    {
        fs::copy(source, destination, fs::copy_options::overwrite_existing);
        return true;
    }
    catch (const fs::filesystem_error &e)
    {
        cerr << "Error copying file from " << source << " to " << destination << ": " 
                  << e.what() << std::endl;
        return false;
    }
}

bool FileHandler::fileExists(const std::string &filePath)
{
    return fs::exists(filePath);
}

bool FileHandler::getFileModificationTime(const std::string &filePath, time_t &modTime)
{
    try
    {
        auto ftime = fs::last_write_time(filePath);
        
        // Convert filesystem clock time to system_clock time
        auto sctp = std::chrono::time_point<std::chrono::system_clock>(
            std::chrono::duration_cast<std::chrono::system_clock::duration>(
                ftime.time_since_epoch()
            )
        );

        modTime = std::chrono::system_clock::to_time_t(sctp);
        return true;
    }
    catch (const std::exception &e)
    {
        cerr << "Error getting modification time for " << filePath << ": " 
                  << e.what() << std::endl;
        return false;
    }
}

vector<string> FileHandler::listFilesInDirectory(const string &dirPath)
{
    vector<string> files;
    if (dirPath.empty() || !fs::exists(dirPath) || !fs::is_directory(dirPath))
    {
        cerr << "Directory does not exist or is not a directory: " << dirPath << endl;
        return files;
    }
    try 
    {
        for (const auto &entry : fs::directory_iterator(dirPath))
        {
            if (fs::is_regular_file(entry.status()))
            {
                files.push_back(entry.path().string());
            }
        }
    }
    catch (const fs::filesystem_error &e) 
    {
        cerr << "Filesystem error in directory_iterator for " << dirPath << ": " << e.what() << std::endl;
    }
    return files;  // return the vector, even if it's empty.
}

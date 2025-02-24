#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include<string>
#include<vector>

using namespace std;

/**
 * @brief Provides static file operations using C++17 filesystem.
 */

class FileHandler
{
    public:
    /**
     * @brief Copy a file from source to destination.
     * @param source Source file path.
     * @param destination Destination file path.
     * @return true if copy succeeds, false otherwise.
    */
    static bool copyFile(const string &source, const string &destination);
    
    /**
     * @brief Check if a file exists.
     * @param filePath The file path.
     * @return true if exists, false otherwise.
    */
    static bool fileExists(const string &fileExists);
    
    /**
     * @brief Get the modification time of a file.
     * @param filePath The file path.
     * @param modTime Output parameter for modification time.
     * @return true if successful, false otherwise.
    */
    static bool getFileModificationTime(const string &filePath, time_t &modTime);
    
    /**
     * @brief List all regular files in the given directory.
     * @param dirPath Directory path.
     * @return A vector of file paths.
    */
    static vector <string> listFilesInDirectory(const string &dirPath);
};

#endif
#include "FileManager.h"
#include "FileHandler.h"
#include <filesystem>
#include <chrono>
#include <ctime>
#include <iostream>
#include <thread>
#include <algorithm>

namespace fs = std::filesystem;

FileManager::FileManager(const string &sourceDir, const string &destDir, Logger *logger)
: sourceDirectory(sourceDir), destinationDirectory(destDir), logger(logger)
{
    logger->logInfo("Initializing FileManager with source: " + sourceDirectory + " and destination: " + destinationDirectory);   
}

FileManager::FileManager(const std::string &sourceDir, Logger *logger)
    : sourceDirectory(sourceDir), destinationDirectory("./backup"), logger(logger)
{
    logger->logInfo("Initializing FileManager with source: " + sourceDirectory + " and default destination: " + destinationDirectory);
    ensureDestinationExists();
}

// Copy constructor
FileManager::FileManager(const FileManager &other):sourceDirectory(other.sourceDirectory), destinationDirectory(other.destinationDirectory), logger(other.logger)
{
    logger -> logInfo("FileManager copy constructed.");
}

//Destructor
FileManager::~FileManager()
{
    logger->logInfo("FileManager destroyed.");
}

void FileManager::ensureDestinationExists()
{
    if(!fs::exists(destinationDirectory))
    {
        try
        {
            fs::create_directories(destinationDirectory);
            logger -> logInfo("Created destination directory: " + destinationDirectory);
        }
        catch(const fs::filesystem_error &e)
        {
            logger->logError("Failed to create destination directory: " + string(e.what()));
        }
    }
    else
    {
        logger->logInfo("Destination directory exists: " + destinationDirectory);
    }
}

vector <string> FileManager::listSourceFiles()
{
    return FileHandler::listFilesInDirectory(sourceDirectory);    
}

vector <string> FileManager::listDestinationFiles()
{
    return FileHandler::listFilesInDirectory(destinationDirectory);
}

void FileManager::performBackup()
{
    logger->logInfo("Starting full backup...");
    vector <string> filesToBackup = listSourceFiles();
    for (const auto &file : filesToBackup)
    {
        fs::path srcPath(file);
        string filename = srcPath.filename().string();
        string destPath = destinationDirectory + "/" + filename;
        if(FileHandler::copyFile(file, destPath))
        {
            logger->logSuccess("Backed up file: " +file);
        }  
        else
        {
            logger->logError("Failed to backup file: "+file);
        }
    }
    logger->logInfo("Full backup completed.");
}

vector <string> FileManager::getNewOrModifiedFiles()
{
    vector <string> newOrModified;
    vector <string> sourceFiles = listSourceFiles();
    for(const auto &srcFile : sourceFiles)
    {
        fs::path srcPath(srcFile);
        string filename =srcPath.filename().string();
        string destPath = destinationDirectory + "/" +filename;
        if(!fs::exists(destPath))
        {
            newOrModified.push_back(srcFile);
        }
        else
        {   
            time_t srcTime, destTime;
            if(FileHandler::getFileModificationTime(srcFile, srcTime) && FileHandler::getFileModificationTime(destPath, destTime))
            {
                if(srcTime > destTime)
                {
                    newOrModified.push_back(srcFile);
                }
            }
        }
    }
    return newOrModified;
}

void FileManager::incrementalBackup()
{
    logger->logInfo("Starting incremental backup...");
    vector<string> filesToBackup = getNewOrModifiedFiles();
    for (const auto &file : filesToBackup) 
    {
        fs::path srcPath(file);
        string filename = srcPath.filename().string();
        string destPath = destinationDirectory + "/" + filename;
        //Version existing file before overwriting, if it exists
        if (FileHandler::copyFile(file, destPath)) 
        {
            logger->logSuccess("Incrementally backed up file: " + file);
        } 
        else
        {
            logger->logError("Failed incremental backup for file: " + file);
        }
    }
    logger->logInfo("Incremental backup completed.");
}

void FileManager::syncFiles() {
    logger->logInfo("Starting file synchronization...");
    vector<string> sourceFiles = listSourceFiles();
    vector<string> destFiles = listDestinationFiles();

    // Sync from source to destination.
    for (const auto &srcFile : sourceFiles) 
    {
        fs::path srcPath(srcFile);
        string filename = srcPath.filename().string();
        string destPath = destinationDirectory + "/" + filename;
        if (fs::exists(destPath)) 
        {
            time_t srcTime, destTime;
            if (FileHandler::getFileModificationTime(srcFile, srcTime) && FileHandler::getFileModificationTime(destPath, destTime)) 
                {
                if (srcTime > destTime) 
                {
                    // Version the current destination file before updating.
                    versionFile(destPath);
                    if (FileHandler::copyFile(srcFile, destPath)) 
                    {
                        logger->logSuccess("Synchronized (updated) file: " + filename);
                    } 
                    else 
                    {
                        logger->logError("Failed to update file during sync: " + filename);
                    }
                }
            }
        } else 
        {
            if (FileHandler::copyFile(srcFile, destPath)) 
            {
                logger->logSuccess("Synchronized (new) file: " + filename);
            } 
            else 
            {
                logger->logError("Failed to copy new file during sync: " + filename);
            }
        }
    }
    //Sync files from destination to source if missing.
    for (const auto &destFile : destFiles)
    {
        fs::path destPath(destFile);
        string filename = destPath.filename().string();
        string srcPath = sourceDirectory + "/" + filename;
        if(!fs::exists(srcPath))
        {
            if(FileHandler::copyFile(destFile,srcPath))
            {
                logger->logSuccess("Synchronized (new) file from destination to source: " + filename);
            }
            else
            {
                logger->logError("Failed to copy new file from destination during sync: " + filename);
            }
        }
    }
    logger->logInfo("File synchronization completed.");
}

void FileManager::versionFile(const string &filePath) 
{
    logger->logInfo("Versioning file: " + filePath);
    fs::path srcPath(filePath);
    string baseName = srcPath.stem().string();
    string extension = srcPath.extension().string();

    // Generate a timestamp.
    time_t now = time(nullptr);
    char timeBuffer[20];
    if (strftime(timeBuffer, sizeof(timeBuffer), "%Y%m%d_%H%M%S", localtime(&now))) 
    {
        string versionedFileName = baseName + "_" + timeBuffer + extension;
        string destPath = destinationDirectory + "/" + versionedFileName;
        if (FileHandler::copyFile(filePath, destPath)) 
        {
            logger->logSuccess("Created versioned backup: " + destPath);
        } 
        else 
        {
            logger->logError("Failed to create versioned backup for: " + filePath);
        }
    } 
    else 
    {
        logger->logError("Failed to generate timestamp for versioning.");
    }
}

void FileManager::monitorChanges(int intervalSeconds) 
{
    logger->logInfo("Starting directory monitoring (interval: " + to_string(intervalSeconds) + " seconds)...");
    vector<string> previousFiles = listSourceFiles();

    while (true) 
    {
        this_thread::sleep_for(chrono::seconds(intervalSeconds));
        vector<string> currentFiles = listSourceFiles();
        bool changeDetected = false;

        // Check for removed files.
        for (const auto &file : previousFiles) 
        {
            if (find(currentFiles.begin(), currentFiles.end(), file) == currentFiles.end()) 
            {
                logger->logInfo("File removed from source: " + file);
                changeDetected = true;
            }
        }
        // Check for new files.
        for (const auto &file : currentFiles) 
        {
            if (find(previousFiles.begin(), previousFiles.end(), file) == previousFiles.end()) 
            {
                logger->logInfo("New file added to source: " + file);
                changeDetected = true;
            }
        }
        // If any change is detected, perform sync and incremental backup.
        if (changeDetected) 
        {
            logger->logInfo("Change detected. Initiating synchronization and incremental backup...");
            syncFiles();
            incrementalBackup();
            previousFiles = currentFiles;  
        }
    }
}

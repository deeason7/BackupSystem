#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include <vector>
#include "Logger.h"

/**
 * @brief FileManager provides operations for managing files, including backup,
 * incremental backup, synchronization, and versioning. It monitors the source
 * directory for changes and automatically synchronizes files.
 *
 * Features:
 * - Constructor overloading (default destination "backup")
 * - Copy constructor and destructor with logging
 * - Continuous directory monitoring for changes
 */

 class FileManager
 {
    public:
        /**
         * @brief Construct a new FileManager object with specified source and destination.
         * @param sourceDir Source directory.
         * @param destDir Destination directory.
         * @param logger Pointer to a Logger instance.
        */
        FileManager(const string &sourceDir, const string &destDir, Logger *logger);

        /**
         * @brief Overloaded constructor that sets the destination to "./backup" by default.
         * @param sourceDir Source directory.
         * @param logger Pointer to a Logger instance.
        */
        FileManager(const string &sourceDir, Logger *logger);

        /**
         * @brief Copy constructor.
         * @param other FileManager object to copy from.
        */
        FileManager(const FileManager &other);

        /**
         * @brief Destructor.
        */
        ~FileManager();

        /**
         * @brief Perform a full backup of all files from source to destination.
         */
        void performBackup();

        /**
         * @brief Perform an incremental backup (only new or modified files).
         */
        void incrementalBackup();

        /**
         * @brief Synchronize files between source and destination.
         */
        void syncFiles();

        /**
         * @brief Create a versioned backup of a specific file (appends a timestamp).
         * @param filePath The file to version.
         * This function is used automatically in syncFiles() and incrementalBackup()
         * when a file is updated. It archives the existing file in the destination
         * before it is overwritten.
         */
        void versionFile(const string &filePath);

        /**
         * @brief List all files in the source directory.
         * @return Vector of file paths.
         */
        vector<string> listSourceFiles();

        /**
         * @brief List all files in the destination directory.
         * @return Vector of file paths.
         */
        vector<string> listDestinationFiles();

        /**
         * @brief Monitor the source directory for changes (new, modified, or removed files).
         *        When changes are detected, automatically perform synchronization and incremental backup.
         * @param intervalSeconds The interval (in seconds) between checks.
         */
        void monitorChanges(int intervalSeconds);

    private:
        string sourceDirectory;  //Source directory
        string destinationDirectory; //Destination directory
        Logger *logger; //Logger for reporting process information

        /**
        * @brief Ensure the destination directory exists; if not, create it.
        */
        void ensureDestinationExists();

        /**
        * @brief Helper function to get new or modified files.
        * @return Vector of file paths that are new or modified.
        */
        vector <string> getNewOrModifiedFiles();
 };


#endif
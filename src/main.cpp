#include "FileManager.h"
#include "Logger.h"
#include <iostream>

int main() {
    // Create a logger that writes to "file_manager.log".
    Logger logger("file_manager.log");

    // Define source and destination directories.
    string sourceDir;
    string destDir;  
    cout << "Enter the path for file directory you want to make backup: ";
    cin >> sourceDir;
    // Alternatively, overload constructor to default to "./backup"
    cout << "Enter the path for file directory you want to save backup: ";
    cin >> destDir;

    // Create a FileManager instance with the provided directories.
    FileManager fm(sourceDir, destDir, &logger);

    fm.performBackup();
    fm.syncFiles();

    // Start monitoring the source directory for changes.
    // The program will run indefinitely (press Ctrl+C to exit).
    fm.monitorChanges(5);

    return 0;
}

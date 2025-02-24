# Backup System

A C++ backup system that monitors a source directory for changes and automatically synchronizes files to a backup directory. The system features full and incremental backups, file synchronization, and versioning (archiving older file versions before updates). It logs all operations to the terminal and a log file.

## Features

- **Full Backup:** Backs up all files from the source directory to a destination directory.
- **Incremental Backup:** Only backs up new or modified files.
- **File Synchronization:** Keeps the source and backup directories in sync.
- **File Versioning:** Creates versioned backups by appending a timestamp to the filename before overwriting.
- **Directory Monitoring:** Continuously monitors the source directory for changes and automatically triggers synchronization.
- **Logging:** Detailed logging of all operations and errors to both terminal and log file.

## Installation

1. **Clone the Repository:**
   ```bash
   git clone https://github.com/YOUR_USERNAME/REPO_NAME.git
   cd REPO_NAME

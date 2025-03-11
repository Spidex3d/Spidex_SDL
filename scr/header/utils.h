#pragma once
#include "config.h"


int countFilesInFolder(const std::string& folderPath) { // count how many file for the animation
    int fileCount = 0;

    // Iterate through the directory entries
    for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
        if (entry.is_regular_file()) {
            ++fileCount; // Increment count for each file
        }
    }

    return fileCount;
}
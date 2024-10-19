#ifndef WRITE_CONFIGURATION_H
#define WRITE_CONFIGURATION_H

#include "SYSHeader.h"

std::optional<std::filesystem::path> DetectAndWriteConfiguration();
std::optional<std::filesystem::path> LoadConfiguration();
void WriteConfiguration(const std::filesystem::path &GamePath, const char *Section, const char *Key);
#endif
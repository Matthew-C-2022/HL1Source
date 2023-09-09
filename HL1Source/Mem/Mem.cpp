#include "Mem.hpp"

std::vector<std::string> M::split(const std::string& str, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

uintptr_t M::scanpattern(const char* Library, const char* Bytes)
{
    MODULEINFO modInfo = { 0 };
    HMODULE hModule = GetModuleHandleA(Library);
    if (hModule == nullptr) {
        return 0;
    }
    GetModuleInformation(GetCurrentProcess(), hModule, &modInfo, sizeof(MODULEINFO));
    uintptr_t startAddress = (uintptr_t)modInfo.lpBaseOfDll;
    uintptr_t endAddress = startAddress + modInfo.SizeOfImage;
    std::vector<std::string> pattern = split(Bytes, ' ');
    for (uintptr_t i = startAddress; i < endAddress - pattern.size(); i++) {
        bool found = true;
        for (size_t j = 0; j < pattern.size(); j++) {
            if (pattern[j] != "??" && strtoul(pattern[j].c_str(), nullptr, 16) != *(unsigned char*)(i + j)) {
                found = false;
                break;
            }
        }
        if (found) {
            return i;
        }
    }
    printf("Pattern Not found! Lib %s, Bytes %s\n", Library, Bytes);
    return 0;
}
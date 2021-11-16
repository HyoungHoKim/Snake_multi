#ifndef STRINGUTILS_HPP
#define STRINGUTILS_HPP

namespace StringUtils
{
    string GetCommandLineArg(int inIndex);
    
    string Sprintf(const char* inFormat, ...);

    void Log(const char* inFormat);
    void Log(const char* inFormat, ...);
}

#define LOG(...) StringUtils::Log(__VA_ARGS__);

#endif
#include "sys_info.h"
#include <QSysInfo>

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
#define WINDOWS_SYS
#endif

#ifdef WINDOWS_SYS
#include<Windows.h>

bool AddPathToEnvironment(const std::wstring& addPath)
{
    wchar_t* szPath = NULL;
    DWORD dwRet = GetEnvironmentVariableW(L"path", NULL, 0);
    szPath = new wchar_t[dwRet];
    GetEnvironmentVariableW(L"path", szPath, dwRet);
    std::wstring path = addPath + L";"+szPath;
    delete[] szPath;
    return SetEnvironmentVariableW(L"path", path.c_str()) == TRUE;
}

#endif

QString sys_info::information()
{
    return QString("[%1] OS:%2%3 Kernel:%4(%5) CpuArch:%6")
            .arg(QSysInfo::machineHostName())
            .arg(QSysInfo::productType())
            .arg(QSysInfo::productVersion())
            .arg(QSysInfo::kernelType())
            .arg(QSysInfo::kernelVersion())
            .arg(QSysInfo::currentCpuArchitecture());
}

QString sys_info::version_app()
{
    return "v1.0.1";
}

QString sys_info::version_lib()
{
    return "ffmpeg:v4.0.2";
}

bool sys_info::addPath(const QString &addPath)
{
#ifdef WINDOWS_SYS
    return AddPathToEnvironment(addPath.toStdWString());
#else
    return false;
#endif
}

#include "dump_tool.h"
#include <QDir>
#include <QDebug>

#ifndef SYS_WIN
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#define SYS_WIN
#endif
#endif //SYS_WIN

#ifdef SYS_UNIX
#endif //SYS_UNIX


#ifdef SYS_WIN
#include "crashrpt_v.1.4.3/CrashRpt.h"
BOOL WINAPI CrashRptCallBack(LPVOID /*lpvState*/)
{
    qWarning("Crash Exit....dump >>>>>>>>>>> exceptions");
    return TRUE;
}
#endif

DumpTool::DumpTool(const QString &root)
{
    QDir dir(root);
    dir.mkdir("exceptions");
    if(dir.cd("exceptions"))
    {
        QByteArray abDir = dir.absolutePath().toLocal8Bit();
        dumpDir_ = abDir.toStdString();
    }
}

DumpTool::~DumpTool()
{
    if(!this->inited_)
        return;

#ifdef SYS_WIN
    crUninstall();
#endif

    this->inited_ = true;
}

bool DumpTool::init()
{
    if(dumpDir_.empty())
    {
        qWarning()<<"Not find exceptions dir";
        return false;
    }

#ifdef SYS_WIN
    CR_INSTALL_INFOA info;
    memset(&info, 0, sizeof(CR_INSTALL_INFOA));
    info.cb = sizeof(CR_INSTALL_INFOA);
    info.pszAppName = "vista_stitch";
    info.pszAppVersion = "0.1.0";
    info.dwFlags |= CR_INST_ALL_POSSIBLE_HANDLERS;
    info.dwFlags |= CR_INST_DONT_SEND_REPORT;
    info.dwFlags |= CR_INST_NO_GUI;
    info.pszErrorReportSaveDir = dumpDir_.c_str();
    info.pfnCrashCallback = &CrashRptCallBack;

    if(EXIT_SUCCESS != crInstallA(&info))
    {
        char errorMsg[512];
        memset(errorMsg, 0, 512);
        crGetLastErrorMsgA(errorMsg, 512);
        errorMsg[512-1] = 0;
        qWarning("crash rpt init failed ,reason:%s",errorMsg);
        return false;
    }
    qDebug("-----------------------crash rpt ready-------------------------");
    crAddScreenshot(CR_AS_VIRTUAL_SCREEN);
    this->inited_ = true;
    return true;
#endif
}

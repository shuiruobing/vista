#include <QCoreApplication>
#include <QDateTime>
#include <QDebug>
#include <QMutex>
#include <QDir>
#include <iostream>
#include "cmd_flags.h"
#include "sys_info.h"
#include "console_read.h"
#include "configure.h"
#include "panorama.h"
#include "spdlog/spdlog.h"
#include "dump_tool.h"

#ifndef APP_NAME
#define APP_NAME "vista_stitch"
#endif

void LogOutMessage(QtMsgType type, const QMessageLogContext& content, const QString& msg);
void InitLog(const QString& rootDir);


int main(int argc, char *argv[])
{
    QCoreApplication::setApplicationName("vista_stitch");
    QCoreApplication::setApplicationVersion("v1.0.0.0");

    G_Flag->initOptions();

    QCoreApplication a(argc, argv);

    if(!G_Flag->parse(a.arguments()))
        return -1;

    if(G_Flag->isNeedShowAndExit())
        return 0;

    if(G_Flag->showCfgExample())
    {
        qDebug()<<cfg::genExpample().c_str();
        return 0;
    }

    QString root = G_Flag->volume();
    if(root.isEmpty())
    {
        std::cout<<"Can not find data root.";
        return -2;
    }
    root.replace("\\","/");
    if(!root.endsWith("/"))
        root.append("/");

    QDir rootDir(root);
    if(!rootDir.exists())
    {
        qWarning()<<"volume: ["<<root<<"] not exist.";
        return -3;
    }

    InitLog(root);
    qDebug()<<"Log start...";
    qWarning()<<"Log start...";
    qCritical()<<"Log start...";

    DumpTool dumpTool(root);
    if(!dumpTool.init())
    {
        qCritical()<<"dump tool init failed!";
        return -4;
    }

    QString vistaFile = root + "vista_stitch.json";
    std::string param = G_Flag->parameter().toStdString();
    if(param.empty())
        param = cfg::readFromFile(vistaFile);

    if(param.empty())
    {
        qCritical()<<"Can not find vista configure anywhere.";
        return -5;
    }

    cfg::PanoNode pn;
    if(!cfg::parseConfigure(param,pn))
    {
        qCritical()<<"Can not parse vista configure.";
        return -6;
    }
    qDebug()<<sys_info::information();
    qDebug()<<param.c_str();
    cfg::saveConfigureToFile(param, vistaFile);

    Panorama* pano = new Panorama(pn);
    ConsoleRead cr;
    cr.connect(&cr,&ConsoleRead::needChangeMuxerOut, pano, &Panorama::onChangeMuxerUrl);
    cr.connect(&cr,&ConsoleRead::needQuit, qApp, &QCoreApplication::quit);
    cr.start();

    if(!pano->open())
    {
        qCritical()<<"Start vista panorama module failed!";
        return -7;
    }

    auto retCode = a.exec();
    cr.stop();
    delete pano;

    spdlog::get(APP_NAME)->flush();
    spdlog::drop_all();

    return retCode;
}


void LogOutMessage(QtMsgType type, const QMessageLogContext& content, const QString& msg)
{
    QString contextInfo = QString("File[%1] Line[%2]").arg(QString(content.file)).arg(content.line);
    QString dateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz");
    QString message = QString("%1 %2 %3").arg(dateTime).arg(contextInfo).arg(msg);
    QByteArray localMsg = message.toLocal8Bit();

    switch (type) {
    case QtDebugMsg:
        spdlog::get(APP_NAME)->debug(localMsg.constData());
        break;
    case QtWarningMsg:
        spdlog::get(APP_NAME)->warn(localMsg.constData());
        break;
    case QtCriticalMsg:
        spdlog::get(APP_NAME)->critical(localMsg.constData());
        break;
    case QtFatalMsg:
        spdlog::get(APP_NAME)->error(localMsg.constData());
        break;
    case QtInfoMsg:
        break;
        std::cout<<msg.toStdString();
    default:
        break;
    }
    spdlog::get(APP_NAME)->flush();
}

void InitLog(const QString& rootDir)
{
    QDir dir(rootDir);
    dir.mkdir(APP_NAME"_log");
    dir.cd(APP_NAME"_log");

    spdlog::set_level(spdlog::level::debug);
    //spdlog::set_async_mode(1024);
    spdlog::set_pattern("%v");

    QDateTime currentTime = QDateTime::currentDateTime();
    QString timeStr = currentTime.toString("yyyy-MM-dd-hh_mm_ss_zzz");
    std::string logPath = rootDir.toStdString()
            + std::string(APP_NAME)
            + "_log/" + std::string(APP_NAME)
            + "_" + timeStr.toStdString() + ".log";
    spdlog::rotating_logger_mt(APP_NAME, logPath, 2*1024*1024, 50);
    qInstallMessageHandler(LogOutMessage);
}

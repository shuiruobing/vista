#include "console_read.h"
#include <QDebug>
#include <iostream>

ConsoleRead::ConsoleRead(QObject *parent)
    : QObject(parent)
{

}

ConsoleRead::~ConsoleRead()
{
    this->stop();
}

bool ConsoleRead::start()
{
    running_.store(true);
    pThreadThread_ = new std::thread(&ConsoleRead::run,this);
    return pThreadThread_ != nullptr;
}

void ConsoleRead::stop()
{
    if(pThreadThread_)
    {
        pThreadThread_->detach();
        delete pThreadThread_;
        pThreadThread_ = nullptr;
    }
}

void ConsoleRead::run()
{
    std::string str;
    while(running_)
    {
        try{
            if(std::getline(std::cin,str))
                parseCommand(str);
            else
                doExit();
        } catch(const std::exception& e) {
            running_ = false;
        }
    }
}

void ConsoleRead::parseCommand(const std::string& str)
{
    QString inStr = QString::fromStdString(str);
    QStringList list = inStr.split(32); //split by space
    if(list.size() < 1 || list.size() > 2)
        return;

    QString& cmd = list[0];
    if(cmd == "q" || cmd == "quit" || cmd == "exit")
        doExit();
    else if(cmd == "cmo" || cmd == "change_muxer_output")
        doChangeMuxerOut(list[1]);
    else if(cmd == "cno" || cmd == "change_nalu_output")
        doChangeNaluOut(list[1]);
    else
        qWarning()<<"Unkown command:"<<inStr;
}

void ConsoleRead::doExit()
{
    QMetaObject::invokeMethod(this, "needQuit", Qt::QueuedConnection);
}

void ConsoleRead::doChangeMuxerOut(const QString &out)
{
    QMetaObject::invokeMethod(this, "needChangeMuxerOut",
                              Qt::QueuedConnection,
                              Q_ARG(QString, out));
}

void ConsoleRead::doChangeNaluOut(const QString &out)
{
    QMetaObject::invokeMethod(this, "needChangeNaluOut",
                              Qt::QueuedConnection,
                              Q_ARG(QString, out));
}

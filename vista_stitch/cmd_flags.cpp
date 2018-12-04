#include "cmd_flags.h"
#include <QList>
#include <QDebug>
#include <QCommandLineOption>


CmdFlags::CmdFlags()
{
    clp.setApplicationDescription("Help [input command line with system text codec]");
    clp.addHelpOption();
    clp.addVersionOption();
}

CmdFlags *CmdFlags::instance()
{
    static CmdFlags cf;
    return &cf;
}

void CmdFlags::initOptions()
{
    const static QList<QCommandLineOption> options = {
        {{"a", "all"}, "Show informations for this application"}
        //, {"stitch_version", "Show version of stitch algorithm"}
        , {{"f", "force"},"Overwrite existing files."}
        , {{"m", "volume"},"Directory needed for this vista <DataRoot>","DataRoot"}
        , {{"s", "stitch"}, "Directory the stitch configur files int<StitchDir>", "StitchDir"}
        , {{"c", "configure"}, "the application vista <Configure>", "Configure"}
        , {"cfg_example", "Show the example of the configure file"}
    };
    clp.addOptions(options);
}

bool CmdFlags::parse(const QStringList &arguments)
{
    if(clp.parse(arguments))
    {
        auto unkownArgs = clp.positionalArguments();
        if(!unkownArgs.empty())
        {
            qDebug()<<"Unkown args:"<<unkownArgs;
            return false;
        }

        qDebug()<<"Input args:"<<clp.optionNames();
        return true;
    }
    qInfo()<<"Parse args error:"<<clp.errorText();
    return false;
}

bool CmdFlags::isNeedShowAndExit()
{
    if(clp.isSet("all"))
    {
        return true;
    }
    else if(clp.isSet("version"))
    {
        clp.showVersion();
        return true;
    }
    else if(clp.isSet("help"))
    {
        clp.showHelp(0);
        return true;
    }
    return false;
}

bool CmdFlags::showCfgExample()
{
    return clp.isSet("cfg_example");
}

QString CmdFlags::volume()
{
    if(clp.isSet("volume"))
        return clp.value("volume");
    return QString();
}

QString CmdFlags::parameter()
{
    if(clp.isSet("configure"))
        return clp.value("configure");
    return QString();
}

QString CmdFlags::stitch()
{
    if(clp.isSet("stitch"))
        return clp.value("stitch");
    return QString();
}

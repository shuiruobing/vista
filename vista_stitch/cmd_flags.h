#ifndef CMD_FLAGS_H
#define CMD_FLAGS_H

#include <QCommandLineParser>
#include <QStringList>

class CmdFlags
{
    CmdFlags();

public:

    static CmdFlags* instance();

    void initOptions();

    bool parse(const QStringList &arguments);

    bool isNeedShowAndExit();

    bool showCfgExample();

    QString volume();

    QString parameter();

    QString stitch();

private:
    QCommandLineParser clp;
};

#define G_Flag CmdFlags::instance()

#endif // CMD_FLAGS_H

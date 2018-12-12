#ifndef DUMP_TOOL_H
#define DUMP_TOOL_H

#include <QString>

class DumpTool
{
public:
    explicit DumpTool(const QString& root);

    ~DumpTool();

    bool init();

private:
    bool inited_{false};

    std::string dumpDir_;
};

#endif // DUMP_TOOL_H

#ifndef SYS_INFO_H
#define SYS_INFO_H

#include <QString>

namespace sys_info {

QString information();

QString version_app();

QString version_lib();

bool addPath(const QString& addPath);

}



#endif // SYS_INFO_H

#ifndef CONSOLE_READ_H
#define CONSOLE_READ_H

#include <thread>
#include <atomic>
#include <QString>
#include <QObject>

class ConsoleRead : public QObject
{
    Q_OBJECT
public:

    ConsoleRead(QObject* parent = nullptr);

    bool start();

    void stop();

signals:

    Q_INVOKABLE void needChangeMuxerOut();

    Q_INVOKABLE void needChangeNaluOut();

    Q_INVOKABLE void needQuit();

private:
    void run();

    void parseCommand(const std::string &str);

    void doExit();

    void doChangeMuxerOut(const QString& out);

    void doChangeNaluOut(const QString &out);

private:
    std::thread* pThreadThread_{nullptr};
    std::atomic_bool running_{false};
};

#endif // NATIVE_MESSAGE_H

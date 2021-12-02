#include "core.h"

#include <QProcess>

Core::Core()
{

}

Core::~Core()
{

}

const QString &Core::getDir() const
{
    return dir;
}

void Core::setDir(const QString &newDir)
{
    dir = newDir;
}

int Core::getMinIndex() const
{
    return minIndex;
}

int Core::getMaxIndex() const
{
    return maxIndex;
}

void Core::setIndex(int newMinIndex, int newMaxIndex)
{
    minIndex = newMinIndex;
    maxIndex = newMaxIndex;
}

int Core::compile()
{
    QProcess process;
    process.setProcessChannelMode(QProcess::MergedChannels);
    process.setWorkingDirectory(dir);

    QString cmd_generator = "g++ generator.cpp -o generator.exe -O2";
    process.start(cmd_generator);

    if (!process.waitForStarted(-1)) {
        emit log("无效的编译器");
        return -1;
    }
    bool finished = process.waitForFinished(2000);
    if (!finished && process.state() == QProcess::Running) {
        emit log("无法结束编译器进程");
        return -1;
    } else if (process.exitCode()) {
        QString CEMsg = QString::fromLocal8Bit(process.readAllStandardOutput());
        if (CEMsg.length() > 51200) CEMsg = "编译错误过多";
        emit log("generator.cpp 编译错误:\n" + CEMsg);
        return -1;
    }

    QString cmd_solution = "g++ solution.cpp -o solution.exe -O2 -Wl,--stack,268435456";
    process.start(cmd_solution);

    finished = process.waitForFinished(2000);
    if (!finished && process.state() == QProcess::Running) {
        emit log("无法结束编译器进程");
        return -1;
    } else if (process.exitCode()) {
        QString CEMsg = QString::fromLocal8Bit(process.readAllStandardOutput());
        if (CEMsg.length() > 51200) CEMsg = "编译错误过多";
        emit log("solution.cpp 编译错误:\n" + CEMsg);
        return -1;
    }
    emit log("编译成功");
    return 0;
}

int Core::makeInput()
{
    QProcess process;
    QString cmd = dir + "/generator.exe";

    for (int i = minIndex; i <= maxIndex; ++i) {
        QString infile = QString("test-%1.in").arg(i, 3, 10, QLatin1Char('0'));
        process.setStandardOutputFile(dir + "/" + infile);
        process.start(cmd);

        if (!process.waitForStarted(-1)) {
            emit log(QString("数据点 %1: 生成器无法启动").arg(i));
            return -1;
        }
        bool finished = process.waitForFinished(10000);
        if (!finished && process.state() == QProcess::Running) {
            emit log(QString("数据点 %1: 生成器运行超时").arg(i));
            return -1;
        } else if (process.exitCode()) {
            emit log(QString("数据点 %1: 生成器崩溃").arg(i));
            return -1;
        }
        emit log(QString("%1 已生成").arg(infile));
    }
    return 0;
}

int Core::makeOutput()
{
    QProcess process;
    QString cmd = dir + "/solution.exe";

    for (int i = minIndex; i <= maxIndex; ++i) {
        QString infile = QString("test-%1.in").arg(i, 3, 10, QLatin1Char('0'));
        QString outfile = QString("test-%1.out").arg(i, 3, 10, QLatin1Char('0'));
        process.setStandardInputFile(dir + "/" + infile);
        process.setStandardOutputFile(dir + "/" + outfile);
        process.start(cmd);

        if (!process.waitForStarted(-1)) {
            emit log(QString("数据点 %1: 标程无法启动").arg(i));
            return -1;
        }
        bool finished = process.waitForFinished(10000);
        if (!finished && process.state() == QProcess::Running) {
            emit log(QString("数据点 %1: 标程运行超时").arg(i));
            return -1;
        } else if (process.exitCode()) {
            emit log(QString("数据点 %1: 标程崩溃").arg(i));
            return -1;
        }
        emit log(QString("%1 已生成").arg(outfile));
    }
    return 0;
}

void Core::work()
{
    int status;

    status = compile();
    if (status) {
        emit finished();
        return;
    }

    status = makeInput();
    if (status) {
        emit finished();
        return;
    }

    makeOutput();
    emit finished();
}

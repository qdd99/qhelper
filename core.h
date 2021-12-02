#ifndef CORE_H
#define CORE_H

#include <QString>
#include <QObject>

class Core : public QObject
{
    Q_OBJECT
    QString dir;
    int minIndex, maxIndex;

    int compile();
    int makeInput();
    int makeOutput();

public:
    Core();
    virtual ~Core();

    QString msg;

    const QString &getDir() const;

    int getMinIndex() const;
    int getMaxIndex() const;

public slots:
    void setDir(const QString &newDir);
    void setIndex(int newMinIndex, int newMaxIndex);
    void work();

signals:
    void finished();
    void log(const QString &msg);
};

#endif // CORE_H

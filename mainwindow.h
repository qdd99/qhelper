#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QThread>

#include "core.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void setDir(const QString& newDir);
    void setIndex(int newMinIndex, int newMaxIndex);
    void work();

private:
    Ui::MainWindow *ui;
    Core *core;
    QThread *sub;

private slots:
    void LOG(const QString& msg);
    void lockUi();
    void unlockUi();
    void on_button_dir_clicked();
    void on_button_go_clicked();
};
#endif // MAINWINDOW_H

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), core(new Core), sub(new QThread)
{
    ui->setupUi(this);

    core->moveToThread(sub);
    connect(sub, &QThread::finished, core, &QObject::deleteLater);
    sub->start();

    connect(core, &Core::log, this, &MainWindow::LOG);
    connect(core, &Core::finished, this, &MainWindow::unlockUi);

    connect(this, &MainWindow::setDir, core, &Core::setDir);
    connect(this, &MainWindow::setIndex, core, &Core::setIndex);
    connect(this, &MainWindow::work, core, &Core::work);
}

MainWindow::~MainWindow()
{
    delete ui;
    sub->quit();
    sub->wait();
}

void MainWindow::LOG(const QString &msg)
{
    ui->text_info->moveCursor(QTextCursor::End);
    ui->text_info->insertPlainText(msg + "\n");
    ui->text_info->moveCursor(QTextCursor::End);
}

void MainWindow::lockUi()
{
    ui->button_go->setEnabled(false);
    ui->button_dir->setEnabled(false);
    ui->line_dir->setEnabled(false);
    ui->spin_from->setEnabled(false);
    ui->spin_to->setEnabled(false);
}

void MainWindow::unlockUi()
{
    ui->button_go->setEnabled(true);
    ui->button_dir->setEnabled(true);
    ui->line_dir->setEnabled(true);
    ui->spin_from->setEnabled(true);
    ui->spin_to->setEnabled(true);
}

void MainWindow::on_button_dir_clicked()
{
    ui->line_dir->setText(QFileDialog::getExistingDirectory());
}

void MainWindow::on_button_go_clicked()
{
    lockUi();

    emit setDir(ui->line_dir->text());
    emit setIndex(ui->spin_from->value(), ui->spin_to->value());
    emit work();
}


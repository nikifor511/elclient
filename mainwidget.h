#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QStandardItemModel>
#include "eq_client.h"

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

private slots:
    void on_closeButton_clicked();
    void on_startButton_clicked();
    void on_getTaskButton_clicked();
    void on_sendButton_clicked();
    void on_disconnectButton_clicked();

    void log_to_ui(QString log_str);
    void tasksToTable(const QJsonArray tasks);    //TODO *tasks
    void enableCurrentTask(QJsonObject currentTask);

private:
    Ui::MainWidget *ui;
    eq_client *cli = NULL;
};

#endif // MAINWIDGET_H

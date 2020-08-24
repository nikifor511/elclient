#include "mainwidget.h"
#include "ui_mainwidget.h"

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::on_closeButton_clicked()
{
    close();
}

void MainWidget::on_startButton_clicked()
{
    if (!cli)
    {
        cli = new eq_client();
        connect(cli, &eq_client::log_to_ui, this, &MainWidget::log_to_ui);
        connect(cli, &eq_client::tasksToTable, this, &MainWidget::tasksToTable);
        connect(cli, &eq_client::enableCurrentTask, this, &MainWidget::enableCurrentTask);
    }
    if (cli->start(ui->hostEdit->text(), ui->portEdit->text().toInt()))
    {
        cli->send("#getFreeTasks");
        ui->startButton->setEnabled(false);
        ui->disconnectButton->setEnabled(true);
    } else {
        ui->startButton->setEnabled(true);
        ui->disconnectButton->setEnabled(false);
    }
}

void MainWidget::on_sendButton_clicked()
{
    if (cli != NULL)
        if (ui->messageEdit->text().length() < 1)
        {
            ui->messageEdit->setText("Type message here..");
        } else {
            cli->send(ui->messageEdit->text().trimmed());
        }
}

void MainWidget::log_to_ui(QString log_str)
{
    ui->log->append(log_str);
    ui->log->append("--------------------------------");
    int y =0;
}

void MainWidget::tasksToTable(const QList<Task*> tasks)
{
    QStandardItemModel *model = new QStandardItemModel(0, 0, this);

    foreach (const Task* task, tasks) {
        QList<QStandardItem *> items;
        items.append(new QStandardItem(QString::number(task->getID())));
        QDateTime tBeginDT = task->getTBegin();
        QString tBeginStr = tBeginDT.toString();
        items.append(new QStandardItem(tBeginStr));
        items.append(new QStandardItem(task->getTAccept().toString("yyyy-MM-dd hh:mm:ss")));
        items.append(new QStandardItem(task->getTEnd().toString("yyyy-MM-dd hh:mm:ss")));
        items.append(new QStandardItem(task->getTicket()));
        items.append(new QStandardItem(QString::number(task->getOperatorID())));
        items.append(new QStandardItem(task->getServiceName()));
        model->appendRow(items);
    }
    ui->freeTasksTableView->setModel(model);
}

void MainWidget::enableCurrentTask(QJsonObject currentTask)
{
    ui->tabWidget->setTabEnabled(1, true);
    ui->tabWidget->setCurrentIndex(1);
    //ui->tbeginLabel->setText()

}

void MainWidget::on_disconnectButton_clicked()
{
    cli->stop();
    ui->startButton->setEnabled(true);
    ui->disconnectButton->setEnabled(false);
}

void MainWidget::on_getTaskButton_clicked()
{
    QItemSelectionModel *select = ui->freeTasksTableView->selectionModel();
    //bool hasSelection = false;
    if (select != nullptr)
        if (select->hasSelection())
        {
            QModelIndexList indexes = select->selection().indexes();
            int taskID = ui->freeTasksTableView->model()->data(ui->freeTasksTableView->model()->index(indexes[0].row(),0)).toInt();
            cli->send("#iwantgettask" + QString::number(taskID));
            qDebug() << QString::number(taskID);
        }
}

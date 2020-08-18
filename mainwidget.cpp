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
    }
    if (cli->start(ui->hostEdit->text(), ui->portEdit->text().toInt()))
    {

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
}

void MainWidget::tasksToTable(const QJsonArray tasks)
{
    QStandardItemModel *model = new QStandardItemModel(0, 0, this);

    foreach (const QJsonValue &task, tasks) {
        QJsonObject taskObj = task.toObject();
        QList<QStandardItem *> items;
        foreach(const QString& key, taskObj.keys()) {
            QJsonValue value = taskObj.value(key);
            //qDebug() << "Key = " << key << ", Value = " << value.toString();
            if (value.type() == QJsonValue::Double) {
                items.append(new QStandardItem(QString::number(value.toInt())));
            } else {
                items.append(new QStandardItem(value.toString()));
            }
        }
        model->appendRow(items);
        qDebug() << "\n";
    }
    ui->currentTasksTableView->setModel(model);
}

void MainWidget::on_disconnectButton_clicked()
{
    cli->stop();
    ui->startButton->setEnabled(true);
    ui->disconnectButton->setEnabled(false);
}

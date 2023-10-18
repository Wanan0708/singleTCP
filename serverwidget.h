#ifndef SERVERWIDGET_H
#define SERVERWIDGET_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTime>
#include <QKeyEvent>

namespace Ui {
class ServerWidget;
}

class ServerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ServerWidget(QWidget *parent = nullptr);
    ~ServerWidget();

private slots:
    void on_pushButton_close_clicked();

    void on_pushButton_send_clicked();

private:
    Ui::ServerWidget *ui;

    QTcpServer *tcpServer;
    QTcpSocket *tcpSocket;

protected:
    bool eventFilter(QObject *target, QEvent *event);
};

#endif // SERVERWIDGET_H

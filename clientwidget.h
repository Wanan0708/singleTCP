#ifndef CLIENTWIDGET_H
#define CLIENTWIDGET_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QDebug>
#include <QTime>
#include <QKeyEvent>
#include <QTextBlockFormat>

QT_BEGIN_NAMESPACE
namespace Ui { class ClientWidget; }
QT_END_NAMESPACE

class ClientWidget : public QMainWindow
{
    Q_OBJECT

public:
    ClientWidget(QWidget *parent = nullptr);
    ~ClientWidget();

private slots:
    void on_pushButton_connect_clicked();

    void on_pushButton_send_clicked();

    void on_pushButton_close_clicked();

private:
    Ui::ClientWidget *ui;

    QTcpSocket *tcpSocket;

protected:
    bool eventFilter(QObject *target, QEvent *event);
};
#endif // CLIENTWIDGET_H

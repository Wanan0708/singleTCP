#include "serverwidget.h"
#include "ui_serverwidget.h"

ServerWidget::ServerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ServerWidget)
{
    ui->setupUi(this);

    tcpServer = new QTcpServer();
    tcpSocket = new QTcpSocket();

    tcpServer->listen(QHostAddress::Any, 8888);  //监听端口8888的连接

    connect(tcpServer, &QTcpServer::newConnection, [=]()
    {
        QTime curTime = QTime::currentTime();
        tcpSocket = tcpServer->nextPendingConnection();
        QString strIp = tcpSocket->peerAddress().toString();
        qDebug() << strIp;
        quint16 u16Port = tcpSocket->peerPort();
        ui->textEdit_readText->append(QString("%1 %2 %3已连接").arg(strIp).arg(u16Port).arg(curTime.toString("hh:mm:ss")));

        connect(tcpSocket, &QTcpSocket::readyRead, [=]()
        {
            QByteArray array = tcpSocket->readAll();

            ui->textEdit_readText->setFont(QFont("宋体",14));//设置字体和字体大小
            ui->textEdit_readText->setTextColor(Qt::Key_OpenUrl);//设置字体颜色
            ui->textEdit_readText->setTextBackgroundColor(Qt::yellow);//设置背景颜色

            ui->textEdit_readText->append(array);
            ui->textEdit_readText->setLayoutDirection(Qt::LeftToRight);  //收到的显示在左边
            ui->textEdit_readText->setAlignment(Qt::AlignLeft);

        });
    });

    ui->textEdit_writeText->installEventFilter(this);
    ui->textEdit_readText->setReadOnly(true);

    QTextBlockFormat blockFormat;
    blockFormat.setLineHeight(6, QTextBlockFormat::LineDistanceHeight);//设置行间距为10
    auto textCursor = ui->textEdit_readText->textCursor();
    textCursor.setBlockFormat(blockFormat);
    ui->textEdit_readText->setTextCursor(textCursor);
}

ServerWidget::~ServerWidget()
{
    if (tcpSocket != nullptr)
    {
        tcpSocket->close();
        tcpSocket = nullptr;
    }
    if (tcpServer != nullptr)
    {
        tcpServer->close();
        tcpServer = nullptr;
    }

    delete ui;
}

void ServerWidget::on_pushButton_close_clicked()
{
    if (tcpSocket != nullptr)
    {
        tcpSocket->disconnectFromHost();

        QString strIp = tcpSocket->peerAddress().toString();
        quint16 u16Port = tcpSocket->peerPort();

        ui->textEdit_readText->append(QString("已断开%1 %2的连接").arg(strIp).arg(u16Port));

        tcpSocket->close();
        tcpSocket = nullptr;
    }
    else
    {
        ui->textEdit_readText->append("未连接任何设备!");
    }
//    tcpServer->close();

}


void ServerWidget::on_pushButton_send_clicked()
{
    if (tcpSocket == nullptr)
    {
        ui->textEdit_readText->append("未连接，请稍后重试");
        return;
    }
    if (ui->textEdit_writeText->toPlainText().isEmpty())
    {
        return;
    }

    QTime curTime = QTime::currentTime();
    QString strWrite = ui->textEdit_writeText->toPlainText();
    QString strInfo = QString("%1 %2").arg(strWrite).arg(curTime.toString("hh:mm:ss"));
    tcpSocket->write(strInfo.toUtf8().data());
    bool isSend = tcpSocket->waitForBytesWritten(3000);
    if (!isSend)
    {
        ui->textEdit_readText->setText("send fail!");
    }

    ui->textEdit_readText->setFont(QFont("宋体",14));//设置字体和字体大小
    ui->textEdit_readText->setTextColor(Qt::Key_LightBulb);//设置字体颜色
    ui->textEdit_readText->setTextBackgroundColor(Qt::green);//设置背景颜色

    ui->textEdit_readText->append(strInfo);
    ui->textEdit_readText->setLayoutDirection(Qt::RightToLeft);  //自己发的显示在右边
    ui->textEdit_readText->setAlignment(Qt::AlignRight);
    QTextCursor tmpCursor = ui->textEdit_readText->textCursor();
    tmpCursor.movePosition(QTextCursor::End);
    ui->textEdit_readText->setTextCursor(tmpCursor);  //设置光标为末尾

    ui->textEdit_writeText->clear();
}

bool ServerWidget::eventFilter(QObject *target, QEvent *event)
{
    if (target == ui->textEdit_writeText)
    {
        if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent *k = static_cast<QKeyEvent *>(event);

            if (k->key() == Qt::Key_Return)
            {
                on_pushButton_send_clicked();
                return true;
            }
        }
    }
    return QWidget::eventFilter(target, event);
}



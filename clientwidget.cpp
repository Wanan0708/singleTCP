#include "clientwidget.h"
#include "ui_clientwidget.h"

ClientWidget::ClientWidget(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ClientWidget)
{
    ui->setupUi(this);

    ui->lineEdit_ipAddress->setText("127.0.0.1");
    ui->lineEdit_port->setText("8888");

    tcpSocket = new QTcpSocket();
    connect(tcpSocket, &QTcpSocket::connected, [=]()
    {
        ui->textEdit_readText->append("与服务器连接成功！");
    });
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

    ui->textEdit_writeText->installEventFilter(this);
    ui->textEdit_readText->setReadOnly(true);

    QTextBlockFormat blockFormat;
    blockFormat.setLineHeight(6, QTextBlockFormat::LineDistanceHeight);//设置行间距为10
    auto textCursor = ui->textEdit_readText->textCursor();
    textCursor.setBlockFormat(blockFormat);
    ui->textEdit_readText->setTextCursor(textCursor);
}

ClientWidget::~ClientWidget()
{
    if (tcpSocket != nullptr)
    {
        tcpSocket->close();
        tcpSocket = nullptr;
    }

    delete ui;
}


void ClientWidget::on_pushButton_connect_clicked()
{
    QString strIp = ui->lineEdit_ipAddress->text();
    quint16 u16Port = ui->lineEdit_port->text().toInt();

    if (tcpSocket == nullptr)
    {
        tcpSocket = new QTcpSocket();
    }
    tcpSocket->connectToHost(strIp, u16Port);
    bool isConnect = tcpSocket->waitForConnected(3000);
    if (!isConnect)
    {
        ui->textEdit_readText->append("The connection falied!!");
    }
}


void ClientWidget::on_pushButton_send_clicked()
{
    if (tcpSocket == nullptr)
    {
        ui->textEdit_readText->append("未连接任何设备!");
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
    bool isWrite = tcpSocket->waitForBytesWritten(3000);
    if (!isWrite)
    {
        ui->textEdit_readText->setText("send fail!");
        return;
    }
    ui->textEdit_readText->setFont(QFont("宋体",14));//设置字体和字体大小
    ui->textEdit_readText->setTextColor(Qt::black);//设置字体颜色
    ui->textEdit_readText->setTextBackgroundColor(Qt::green);//设置背景颜色

    ui->textEdit_readText->append(strInfo);
    ui->textEdit_readText->setLayoutDirection(Qt::RightToLeft); //自己发的显示在右边
    ui->textEdit_readText->setAlignment(Qt::AlignRight);
    QTextCursor tmpCursor = ui->textEdit_readText->textCursor();
    tmpCursor.movePosition(QTextCursor::End);
    ui->textEdit_readText->setTextCursor(tmpCursor);  //设置光标为末尾
    ui->textEdit_writeText->clear();
}


void ClientWidget::on_pushButton_close_clicked()
{
    if (tcpSocket == NULL)
    {
        ui->textEdit_readText->append("未连接任何设备!");
    }
    tcpSocket->disconnectFromHost();
    ui->textEdit_readText->append("与服务器断开连接!");
    tcpSocket->close();
    tcpSocket = nullptr;
}

bool ClientWidget::eventFilter(QObject *target, QEvent *event)
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


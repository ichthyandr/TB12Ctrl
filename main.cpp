#include "dialog.h"
#include <QApplication>
#include <QFile>
#include <QMutex>
#include <QString>
#include <QDateTime>
#include <QLockFile>
#include <QDir>
#include <QMessageBox>
#include <QStyleFactory>
#include <QStandardPaths>
#include <QDebug>
#include "qmidi/qmidisysexmsgs.h"

static QFile    g_logFile(QStandardPaths::locate(QStandardPaths::TempLocation, QString(), QStandardPaths::LocateDirectory)+"TB12Ctrl.log");
static QMutex   g_mutex;

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg) {

    QMutexLocker locker(&g_mutex);
    QString txt;

    QDateTime date_time = QDateTime::currentDateTime ();
    QString   stamp     = date_time.toString("hh:mm:ss.zzz");

    switch (type) {
    case QtInfoMsg:
        txt = QString("%1 <%2> INFO: [%3 %4] %5\r\n").arg(stamp).arg(context.category).arg(context.file).arg(context.line).arg(msg);
        break;
    case QtDebugMsg:
        txt = QString("%1 <%2> DEBUG: [%3 %4] %5\r\n").arg(stamp).arg(context.category).arg(context.file).arg(context.line).arg(msg);
        break;
    case QtWarningMsg:
        txt = QString("%1 <%2> WARNING: [%3 %4] %5\r\n").arg(stamp).arg(context.category).arg(context.file).arg(context.line).arg(msg);
        break;
    case QtCriticalMsg:
        txt = QString("%1 <%2> CRITICAL: [%3 %4] %5\r\n").arg(stamp).arg(context.category).arg(context.file).arg(context.line).arg(msg);
        break;
    case QtFatalMsg:
        txt = QString("%1 <%2> FATAL: [%3 %4] %5\r\n").arg(stamp).arg(context.category).arg(context.file).arg(context.line).arg(msg);
        break;
    }

    g_logFile.write(txt.toLocal8Bit());
    g_logFile.flush();
}

#if 0
#include <QStringList>
#include <QString>
#endif
int main(int argc, char *argv[]){
    g_logFile.open(QIODevice::WriteOnly | QIODevice::Append);
    qInstallMessageHandler(myMessageOutput);

    //QString tmpLocation = QStandardPaths::locate(QStandardPaths::TempLocation, QString(), QStandardPaths::LocateDirectory);
    //qDebug() << tmpLocation;

    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#if 0
    QStringList list = QStyleFactory::keys();

    foreach (const QString &str, list) {
        qDebug() << str;
    }
    14:55:35.338 <default> DEBUG: [..\TB12Ctrl\main.cpp 58] "windowsvista"
    14:55:35.339 <default> DEBUG: [..\TB12Ctrl\main.cpp 58] "Windows"
    14:55:35.339 <default> DEBUG: [..\TB12Ctrl\main.cpp 58] "Fusion"
#endif

    QApplication::setStyle(QStyleFactory::create("Fusion"));

    QApplication a(argc, argv);

    QLockFile lockFile(QDir::temp().absoluteFilePath(QString::fromUtf8("TB12Ctrl.lock")));
    /* Trying to close the Lock File, if the attempt is unsuccessful for 100 milliseconds,
        * then there is a Lock File already created by another process.
        / Therefore, we throw a warning and close the program
        * */
    if(!lockFile.tryLock(100)){
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(QString::fromUtf8("Приложение уже запущено"));
        msgBox.exec();
        g_logFile.close();
        return 1;
    }

    // init qmidimsgs module
    SSXMSGS::init();

    int retcode;
    {   // чтобы деструктор окна записался в отладку перед закрытием файла
        Dialog w;
        //w.setWindowState(w.windowState() ^ Qt::WindowFullScreen);
        //w.setWindowState(Qt::WindowMaximized);
        w.show();
        retcode = a.exec();
    }

    g_logFile.close();
    return retcode;
}

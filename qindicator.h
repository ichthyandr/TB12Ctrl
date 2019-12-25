#ifndef QINDICATOR_H
#define QINDICATOR_H

#include <QObject>
#include <QPixmap>

/// класс индикатора
class QIndicator : public QObject
{
    Q_OBJECT
public:
    /// конструктор
    explicit QIndicator(QObject *parent = nullptr);
    /// переключить в другое состояние, отдать пихмап индикатора
    QPixmap flip();
    /// отдать пихмап включенного индикатора
    QPixmap on();
    /// отдать пихмап выключенного индикатора
    QPixmap off();
private:
    /// индикатор "включено"
    bool m_on{false};

signals:

public slots:
};

#endif // QINDICATOR_H

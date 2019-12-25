#ifndef QSYSSETUPWIDGET_H
#define QSYSSETUPWIDGET_H

#include <QWidget>
#include <QVector>
#include <QLoggingCategory>
Q_DECLARE_LOGGING_CATEGORY(SYS)

/// закладка system setup
class QSysSetupWidget : public QWidget
{
    Q_OBJECT
public:
    /// конструктор
    explicit QSysSetupWidget(QWidget *parent = Q_NULLPTR);
    /// деструктор
    ~QSysSetupWidget();
    /// обновить поля при загрузке данных
    void updateData();
private:
    /// защелка
    bool m_proceed{false};
    /// вектор элементов управления
    QVector<QWidget *> m_widgets;
signals:
    /// сигнал отслеживания изменений
    ///\param number[in] - номер измененного банка
    void s_changed( int number );
public slots:
};

#endif // QSYSSETUPWIDGET_H

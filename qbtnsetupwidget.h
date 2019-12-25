#ifndef QBTNSETUPWIDGET_H
#define QBTNSETUPWIDGET_H

#include <QWidget>
#include <QLoggingCategory>
Q_DECLARE_LOGGING_CATEGORY(BSW)

/// закладка настройки кнопки
class QBtnSetupWidget : public QWidget
{
    Q_OBJECT
public:
    /// конструктор
    explicit QBtnSetupWidget(QWidget *parent = Q_NULLPTR);
    /// деструктор
    ~QBtnSetupWidget();
    /// установить номер настраиваемой кнопки
    /// \param btnNumber[in] - номер (1..12)
    /// \param curBnkNumber[in] - текущий редактируемый банк (0..)
    void setButton(int btnNumber, int curBnkNumber);
signals:
    /// сигнал "BAck"
    void s_back();
    /// сигнал отслеживания изменений
    ///\param number[in] - номер измененного банка
    void s_changed( int number );
private slots:
    /// слот отслеживания изменений
    ///\param number[in] - номер измененного банка или -1 если изменены globals
    void _changed( int number );
public slots:
private:
    /// номер кнопки (1..12)
    int m_btnNumber{1};
    /// текущий редактируемый банк (0..)
    int m_curBnkNumber{0};
    /// защелка для обработчиков
    bool m_bProceed{false};
};

#endif // QBTNSETUPWIDGET_H

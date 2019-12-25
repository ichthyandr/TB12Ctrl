#ifndef QBANK2WIDGET_H
#define QBANK2WIDGET_H

#include <QWidget>
#include <QLoggingCategory>
#include <QLabel>
#include <QSpinBox>
Q_DECLARE_LOGGING_CATEGORY(BANK2)

/// класс формы Bank To
class QBank2Widget : public QWidget
{
    Q_OBJECT
public:
    /// конструктор
    explicit QBank2Widget(QWidget *parent = nullptr);
    /// destructor
    ~QBank2Widget();
    /// get instance
    static QBank2Widget * getInstance();
    /// установить номер настраиваемой кнопки
    ///\param btnNumber[in] - номер (1..12)
    ///\param curBnkNumber[in] - текущий редактируемый банк (0..)
    void setButton(int btnNumber, int curBnkNumber);
private:
    /// вектор лейбл
    QVector<QLabel*>        m_labels;
    /// вектор спинбоксов
    QVector<QSpinBox*>      m_spinBoxes;
    /// номер кнопки (1..12)
    int m_btnNumber{1};
    /// текущий редактируемый банк (0..)
    int m_curBnkNumber{0};
    /// защелка для обработчиков
    bool m_bProceed{false};
signals:
    /// сигнал отслеживания изменений
    ///\param number[in] - номер измененного банка
    void s_changed( int number );
public slots:
};

#endif // QBANK2WIDGET_H

#ifndef QCCMWIDGET_H
#define QCCMWIDGET_H

#include <QWidget>
#include <QLoggingCategory>
#include <QLabel>
#include <QSpinBox>
#include <QCheckBox>
#include <QComboBox>
Q_DECLARE_LOGGING_CATEGORY(CCM)

/// панель Control Change Momentary
class QCcmWidget : public QWidget
{
    Q_OBJECT
public:
    /// конструктор
    explicit QCcmWidget(QWidget *parent = Q_NULLPTR);
    /// деструктор
    ~QCcmWidget();
    /// get instance
    static QCcmWidget * getInstance();
    /// установить номер настраиваемой кнопки
    /// \param btnNumber[in] - номер (1..12)
    /// \param curBnkNumber[in] - текущий редактируемый банк (0..)
    void setButton(int btnNumber, int curBnkNumber);
private:
    /// номер кнопки (1..12)
    int m_btnNumber{1};
    /// текущий редактируемый банк (0..)
    int m_curBnkNumber{0};
    /// защелка для обработчиков
    bool m_bProceed{false};
    /// вектор лейбл
    QVector<QLabel*>        m_labels;
    /// вектор спинбоксов
    QVector<QSpinBox*>      m_spinBoxes;
    /// вектор чекбоксов
    QVector<QCheckBox*>     m_checkBoxes;
    /// вектор комбо для реле
    QVector<QComboBox*>     m_relCombos;
    /// обработчик комбо реле
    void relays( int index, int shift, QComboBox * combo = Q_NULLPTR );
signals:
    /// сигнал отслеживания изменений
    ///\param number[in] - номер измененного банка
    void s_changed( int number );
public slots:
};

#endif // QCCMWIDGET_H

#ifndef QCCCWIDGET_H
#define QCCCWIDGET_H

#include <QWidget>
#include <QLoggingCategory>
#include <QLabel>
#include <QSpinBox>
#include <QComboBox>
#include <QCheckBox>
Q_DECLARE_LOGGING_CATEGORY(CCC)

/// панель Control Change Constant
class QCccWidget : public QWidget
{
    Q_OBJECT
public:
    /// конструктор
    explicit QCccWidget(QWidget *parent = nullptr);
    /// деструктор
    ~QCccWidget();
    /// get instance
    static QCccWidget * getInstance();
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
    void relays(int index, uint8_t shift, QComboBox * combo = Q_NULLPTR );
signals:
    /// сигнал отслеживания изменений
    ///\param number[in] - номер измененного банка
    void s_changed( int number );
public slots:
};

#endif // QCCCWIDGET_H

#ifndef QNRPNCWIDGET_H
#define QNRPNCWIDGET_H
#include <QWidget>
#include <QLoggingCategory>
#include <QLabel>
#include <QSpinBox>
#include <QCheckBox>
#include <QComboBox>
Q_DECLARE_LOGGING_CATEGORY(NPRNC)

/// класс формы NRPN Constant
class QNrpnCWidget : public QWidget {
    Q_OBJECT
public:
    /// конструктор
    explicit QNrpnCWidget(QWidget *parent = nullptr);
    /// деструктор
    ~QNrpnCWidget();
    /// get instance
    static QNrpnCWidget * getInstance();
    /// установить номер настраиваемой кнопки
    ///\param btnNumber[in] - номер (1..12)
    ///\param curBnkNumber[in] - текущий редактируемый банк (0..)
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

#endif // QNRPNCWIDGET_H

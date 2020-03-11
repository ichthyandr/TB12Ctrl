#ifndef QNPRNTWIDGET_H
#define QNRPNTWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QSpinBox>
#include <QCheckBox>
#include <QComboBox>
#include <QLoggingCategory>
Q_DECLARE_LOGGING_CATEGORY(NPRNT)

/// класс формы для NRPN Toggle
class QNrpnTWidget : public QWidget {
    Q_OBJECT
public:
    /// конструктор
    ///\param toggle[in] - признак формы Toggle, если true, иначе Momentary
    explicit QNrpnTWidget(bool toggle = true, QWidget *parent = nullptr);
    /// деструктор
    ~QNrpnTWidget();
    /// установить номер настраиваемой кнопки
    ///\param btnNumber[in] - номер (1..12)
    ///\param curBnkNumber[in] - текущий редактируемый банк (0..)
    void setButton(int btnNumber, int curBnkNumber);
    /// get instance
    /// \param toggle[in] - вернуть экземпляр для формы Toggle, если true, иначе для Momentary
    static QNrpnTWidget * getInstance(bool toggle = true);
private:
    /// признак Toggle если TRUE, иначе - Momentary
    bool m_bToggle{true};
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

    QComboBox *             m_vbxVID{Q_NULLPTR};
    /// обработчик комбо реле
    void relays( int index, int shift, QComboBox * combo = Q_NULLPTR );
signals:
    /// сигнал отслеживания изменений
    ///\param number[in] - номер измененного банка
    void s_changed( int number );
public slots:
};

#endif // QNRPNTWIDGET_H

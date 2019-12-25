#ifndef QCMNSETUPWIDGET_H
#define QCMNSETUPWIDGET_H

#include <QWidget>
#include <QLoggingCategory>
Q_DECLARE_LOGGING_CATEGORY(CMN)

#include <QVector>
#include <QLabel>
#include <QSpinBox>
#include <QCheckBox>
#include <QComboBox>

/// закладка Common Setup
class QCmnSetupWidget : public QWidget
{
    Q_OBJECT
public:
    /// конструктор
    explicit QCmnSetupWidget(QWidget *parent = Q_NULLPTR);
    /// деструктор
    ~QCmnSetupWidget();
    /// get instance
    static QCmnSetupWidget * getInstance();
    /// обновить данные на форме
    ///\param curBnkNumber[in] - индекс банка
    void updateData(int curBnkNumber );
private:
    /// labels vector
    QVector<QLabel *>       m_labels;
    /// spinboxes vector
    QVector<QSpinBox *>     m_spins;
    /// checkboxes vector
    QVector<QCheckBox *>    m_checks;
    /// comboboxes vector
    QVector<QComboBox *>    m_combos;
    /// защелка для обработчиков
    bool m_proceed{false};
    /// текущий редактируемый банк (0..)
    int m_curBnkNumber{0};
signals:
    /// сигнал отслеживания изменений
    ///\param number[in] - номер измененного банка
    void s_changed( int number );
};

#endif // QCMNSETUPWIDGET_H

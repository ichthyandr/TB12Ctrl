#ifndef QPRESETCHANGEWIDGET_H
#define QPRESETCHANGEWIDGET_H
#include <QWidget>
#include <QLoggingCategory>
#include <QVector>
#include <QComboBox>
#include <QCheckBox>
#include <QButtonGroup>
Q_DECLARE_LOGGING_CATEGORY(PCW)


/// preset change панель
class QPresetChangeWidget : public QWidget {
    Q_OBJECT
public:
    ///конструктор
    explicit QPresetChangeWidget(QWidget *parent = Q_NULLPTR);
    ///деструктор
    ~QPresetChangeWidget();
    /// установить номер настраиваемой кнопки
    /// \param btnNumber[in] - номер (1..12)
    /// \param curBnkNumber[in] - текущий редактируемый банк (0..)
    void setButton(int btnNumber, int curBnkNumber);
    /// get instance
    static QPresetChangeWidget * getInstance();
    /// вычисление preset number
    /// \param b_useBankSelect[in] - true or false
    /// \param number[in] - message number 0..3
    /// \param bank[in]     - bank number 0...
    /// \param button[in]   - button number 1...max
    static int presetNumber(bool b_useBankSelect, int number, int bank, int button);
private:
    /// признак SSXMSGS::UseBankSelectMess::USE_BANK_SELECT
    ///bool m_useBankSelect{false};
    /// номер кнопки (1..12)
    int m_btnNumber{1};
    /// текущий редактируемый банк (0..)
    int m_curBnkNumber{0};
    /// защелка для обработчика чекбоксов iaState
    bool m_iaStateProceed{false};
    /// вектор комбобоксов
    QVector<QComboBox*>         m_msgCombos;
    /// чекбоксы для iaState
    QButtonGroup                m_iaStateBG;
    /// комбобоксмы для реле
    QVector<QComboBox*>         m_relCombos;
    /// обработчик комбо реле
    void relays( int index, int shift, QComboBox * combo = Q_NULLPTR );
signals:
    /// сигнал отслеживания изменений
    ///\param number[in] - номер измененного банка
    void s_changed( int number );
private slots:
public slots:
};

#endif // QPRESETCHANGEWIDGET_H

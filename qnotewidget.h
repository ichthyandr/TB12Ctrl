#ifndef QNOTEWIDGET_H
#define QNOTEWIDGET_H

#include <QWidget>
#include <QLoggingCategory>
#include <QLabel>
#include <QSpinBox>
Q_DECLARE_LOGGING_CATEGORY(NOTE)

/// класс формы Note ON/OFF
class QNoteWidget : public QWidget{
    Q_OBJECT
public:
    /// конструктор
    /// \param noteON[in] - если true, то форма Note On, иначе - Note Off
    explicit QNoteWidget( bool noteON = true, QWidget *parent = nullptr);
    /// get instance
    /// \param noteON[in] - вернуть экземпляр для формы Note ON, если true, иначе Note Off
    static QNoteWidget * getInstance(bool noteON = true);
    /// установить номер настраиваемой кнопки
    ///\param btnNumber[in] - номер (1..12)
    ///\param curBnkNumber[in] - текущий редактируемый банк (0..)
    void setButton(int btnNumber, int curBnkNumber);
private:
    /// если true, то форма Note On, иначе - Note Off
    bool m_bNoteOn{true};
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

#endif // QNOTEWIDGET_H

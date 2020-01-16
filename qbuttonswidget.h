#ifndef QBUTTONSWIDGET_H
#define QBUTTONSWIDGET_H
#include <QWidget>
#include <QLoggingCategory>
Q_DECLARE_LOGGING_CATEGORY(BTN)

class QTBbutton;
/// панель кнопок
class QButtonsWidget : public QWidget
{
    Q_OBJECT
public:
    ///конструтор
    explicit QButtonsWidget(QWidget *parent = Q_NULLPTR);
    ///деструктор
    ~QButtonsWidget();
    /// get instance
    static QButtonsWidget * getInstance();
    /// update buttons
    /// \param bank[in] - bank number 0...;
    void update( int bank );
    /// update buttons
    /// \param button[in]   - button number 1...FOOT_BUTTONS_NUM;
    /// \param bank[in]     - bank number 0...;
    void update( int button, int bank );
signals:
    /// выдать сигнал, что кнопка с number нажата
    void s_clickedBtn(int number);
public slots:
private:
    /// вектор кнопок
    QVector<QTBbutton*> m_pBtns;
};

#endif // QBUTTONSWIDGET_H

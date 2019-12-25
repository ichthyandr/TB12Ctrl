#ifndef QBUTTONSWIDGET_H
#define QBUTTONSWIDGET_H
#include <QWidget>
#include <QLoggingCategory>
Q_DECLARE_LOGGING_CATEGORY(BTN)

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
signals:
    /// выдать сигнал, что кнопка с number нажата
    void s_clickedBtn(int number);
public slots:
};

#endif // QBUTTONSWIDGET_H

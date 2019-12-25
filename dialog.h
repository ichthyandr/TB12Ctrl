#ifndef DIALOG_H
#define DIALOG_H
#include <QDialog>
#include <QLoggingCategory>
Q_DECLARE_LOGGING_CATEGORY(DLG)

/// основной диалог
class Dialog : public QDialog {
    Q_OBJECT
public:
    /// конструктор
    Dialog(QWidget *parent = Q_NULLPTR);
    /// деструктор
    ~Dialog();
    /// get instance
    static Dialog * getInstance();
    /// установить текущий номер банка для редактирования
    ///\param bankNumber[in] - номер банка
    void setCurrentBankNumber( int bankNumber );
public slots:
    /// слот приема сигнала нажатия кнопки
    /// \param btnNumber[in] - номер кнопки 1..12
    void _clickedBtn( int btnNumber );
    /// слот обработки сигнала "Back" с формы настройки кнопок
    void _back();
private:
    /// текущий номер банка
    int m_currentBankNumber{0};
};

#endif // DIALOG_H

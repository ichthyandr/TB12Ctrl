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
    /// enable disable "save" menu
    /// \param enable - true for enable
    void enableSave( bool enable );
    /// enable disable "load" menu
    /// \param enable - true for enable
    void enableLoad( bool enable );
    /// enable disable "Models" menu
    /// \param enable - true for enable
    void enableModels( bool enable );
    /// обновить модель согласно загруженным данным
    void refreshModel();
public slots:
    /// слот приема сигнала нажатия кнопки
    /// \param btnNumber[in] - номер кнопки 1..12
    void _clickedBtn( int btnNumber );
    /// слот обработки сигнала "Back" с формы настройки кнопок
    void _back();
private:
    /// текущий номер банка
    int m_currentBankNumber{0};
    /// save action
    QAction * m_saveAct{Q_NULLPTR};
    /// load action
    QAction * m_loadAct{Q_NULLPTR};
    /// models menu action
    QAction * m_ctlActMenu{Q_NULLPTR};
    QVector<QAction *> m_modelsAct;
};

#endif // DIALOG_H

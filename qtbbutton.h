#ifndef QTBBUTTON_H
#define QTBBUTTON_H
#include <QPushButton>
#include <QPixmap>
#include <QPainter>
#include <QPoint>
#include <QLoggingCategory>
Q_DECLARE_LOGGING_CATEGORY(QTB)

///
class QTBbutton: public QPushButton {
    Q_OBJECT
public:
    ///конструктор
    /// \param number[in] - button number
    /// \param parent[in] - parent of the button
    explicit QTBbutton(const int number =1, QWidget *parent = Q_NULLPTR);
    /// update button view
    ///\param bank[in] - bank number;
    void update( int bank );
    /// set/unset blanked
    ///\param blanked[in] - true or false
    void setBlanked( bool blanked = true );
private:
    /// номер кнопки
    int m_btnNumber{1};
    /// make pixmap
    /// \param number[in] - button number [1..FOOT_BUTTONS_NUM]
    static QPixmap getIcon( const int number );
    /// make pixmap
    /// \param number[in]   - button number [1..FOOT_BUTTONS_NUM]
    /// \param bank[in]     - bank number 0...
    static QPixmap getIcon( const int number, const int bank );
    /// признак - кнопка рабочая или нет
    bool m_blanked{false};
};

#endif // QTBBUTTON_H

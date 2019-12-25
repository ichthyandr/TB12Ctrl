#ifndef QTBBUTTON_H
#define QTBBUTTON_H
#include <QPushButton>
#include <QPixmap>
#include <QPainter>
#include <QPoint>

///
class QTBbutton: public QPushButton {
    Q_OBJECT
public:
    ///конструктор
    /// \param number[in] - button number
    /// \param parent[in] - parent of the button
    explicit QTBbutton(const int number =1, QWidget *parent = Q_NULLPTR);
private:
    /// make pixmap
    /// \param number[in] - button number
    static QPixmap getIcon( const int number =1 );
};

#endif // QTBBUTTON_H

#include "qtbbutton.h"
#include <QPainter>
#include <QColor>
#include <QIcon>
#include <QDebug>

QTBbutton::QTBbutton(const int number, QWidget *parent):QPushButton(parent){
    setFixedSize(130,130);
    setIconSize(QSize(120,120));

    QPixmap pix = QTBbutton::getIcon( number );
    QIcon icon(pix);

    setIcon( QIcon(pix));
    //setDisabled( true );
}

QPixmap QTBbutton::getIcon( const int number ){
    QPixmap pix(120,120);
    //pix.fill(Qt::black);
    pix.fill(Qt::transparent);
    QPainter painter(&pix);
    painter.setPen( QColor(Qt::black) );
    QFont f( "Helvetica", 8 );
    painter.setFont(f);
    QFontMetrics fm(f);

    QString header = QString("Button %1").arg( number );
    painter.drawText( 0, /*fm.height()-fm.lineSpacing()*/fm.ascent() , header );
    return(pix);
}

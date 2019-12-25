#include "qindicator.h"
#include <QPainter>

QIndicator::QIndicator(QObject *parent) : QObject(parent){
}

QPixmap QIndicator::flip(){
    m_on = !m_on;
    QPixmap pix(16,16);

    if ( m_on )
        pix.fill(Qt::green);
    else
        pix.fill(Qt::darkGreen);

    QPainter painter(&pix);
    painter.setPen( QColor(Qt::darkGray) );
    painter.drawRect(0, 0, 15, 15);

    return(pix);
}


QPixmap QIndicator::on(){
    m_on = true;

    QPixmap pix(16,16);

    pix.fill(Qt::green);

    QPainter painter(&pix);
    painter.setPen( QColor(Qt::darkGray) );
    painter.drawRect(0, 0, 15, 15);

    return(pix);
}

QPixmap QIndicator::off(){
    m_on = false;

    QPixmap pix(16,16);

    pix.fill(Qt::darkGreen);

    QPainter painter(&pix);
    painter.setPen( QColor(Qt::darkGray) );
    painter.drawRect(0, 0, 15, 15);

    return(pix);
}


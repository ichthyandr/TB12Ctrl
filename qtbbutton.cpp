#include "qtbbutton.h"
#include <QPainter>
#include <QColor>
#include <QIcon>
#include <QDebug>
#include <QMap>
#include "qmidi/qmidisysexmsgs.h"
#include "qpresetchangewidget.h"

Q_LOGGING_CATEGORY( QTB, "QTB")

QTBbutton::QTBbutton(const int number, QWidget *parent):QPushButton(parent), m_btnNumber(number){

    qCDebug(QTB) << Q_FUNC_INFO << "btnNumber=" << number;

    setFixedSize(130,130);
    setIconSize(QSize(120,120));

    QPixmap pix = QTBbutton::getIcon( number );
    QIcon icon(pix);

    setIcon( QIcon(pix));
    //setDisabled( true );
}

QPixmap QTBbutton::getIcon( const int number ){
    QPixmap pix(120,120);
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

static QMap<int, QString> buttonType = {
    { SSXMSGS::ButtonType::PRESET_CHANGE,   QObject::tr("Preset Change") },
    { SSXMSGS::ButtonType::CC_TOGGLE,       QObject::tr("CC Toggle") },
    { SSXMSGS::ButtonType::CC_MOMENTARY,    QObject::tr("CC Momentary") },
    { SSXMSGS::ButtonType::CC_CONST_VAL,    QObject::tr("CC Constant Value") },
    { SSXMSGS::ButtonType::NRPN_TOGGLE,     QObject::tr("NRPN Toggle") },
    { SSXMSGS::ButtonType::NRPN_MOMENTARY,  QObject::tr("NRPN Momentary") },
    { SSXMSGS::ButtonType::NRPN_CONST_VAL,  QObject::tr("NRPN Constant Value") },
    { SSXMSGS::ButtonType::NOTE_ON,         QObject::tr("Note On") },
    { SSXMSGS::ButtonType::NOTE_OFF,        QObject::tr("Note Off") },
    { SSXMSGS::ButtonType::PRESET_UP,       QObject::tr("Preset Up") },
    { SSXMSGS::ButtonType::PRESET_DOWN,     QObject::tr("Preset Down") },
    { SSXMSGS::ButtonType::BANK_UP,         QObject::tr("Bank Up") },
    { SSXMSGS::ButtonType::BANK_DOWN,       QObject::tr("Bank Down") },
    { SSXMSGS::ButtonType::BANK_TO,         QObject::tr("Bank To") },
};

static void printRelays( int & X, int & Y, int delta, QPainter & painter, const int number, const int bank){
    X=0; QString _param("Relays:   1   2   3   4");
    Y+= delta;
    painter.drawText( X, Y, _param );
    Y+= delta;
    for ( int i=0; i<4; i++ ){
        switch (i){
            case 0: X=46; break;
            case 1: X=60; break;
            case 2: X=75; break;
            case 3: X=90; break;
        }
        uint8_t data    = SSXMSGS::g_BanksSettings[bank].buttonContext[number-1].relays;
        int     res     = (data >> (i*2)) & 0x03;
        switch ( res ){
        case RELAY_MASK_NO_ACTION:
            painter.drawText( X, Y, "N" );
            break;
        case RELAY_MASK_CLOSE:
            painter.drawText( X, Y, "C" );
            break;
        case RELAY_MASK_OPEN:
            painter.drawText( X, Y, "O" );
            break;
        case RELAY_MASK_MOMENTARY:
            painter.drawText( X, Y, "M" );
            break;
        }
    }
    X=0;
}

QPixmap QTBbutton::getIcon( const int number, const int bank ){
    QPixmap pix(120,120);
    pix.fill(Qt::transparent);
    QPainter painter(&pix);
    painter.setPen( QColor(Qt::black) );
    QFont f( "Helvetica", 8 );
    painter.setFont(f);
    QFontMetrics fm(f);

    int delta = fm.ascent()+2;

    int X = 0;
    int Y = delta;
    QString header = QString("Button %1").arg( number );
    painter.drawText( X, Y , header );

    if ( !(bank < SSXMSGS::g_BanksSettings.size())){
        qCCritical(QTB) << Q_FUNC_INFO << "g_BanksSettings is empty!";
        return(pix);
    }
    if ( !((number-1) < FOOT_BUTTONS_NUM) ){
        qCCritical(QTB) << Q_FUNC_INFO << "button number out of range! number=" << number;
        return(pix);
    }
    char _alias[BUTTON_NAME_MAX_SIZE+1];
    ::memset( _alias, 0, BUTTON_NAME_MAX_SIZE+1);
    ::memcpy( _alias, SSXMSGS::g_BanksSettings.at(bank).buttonContext->nameAlias, BUTTON_NAME_MAX_SIZE);
    QString _aliasName(_alias);
    if (_aliasName.isEmpty()) _aliasName = "Alias Name";
    _aliasName = QString("[%1]").arg(_aliasName);
    Y+= delta;
    painter.drawText( X, Y, _aliasName );

    QString _ButtonType = buttonType.value( SSXMSGS::g_BanksSettings.at(bank).buttonType[number-1] );
    Y+= delta;
    f.setBold(true);
    painter.setFont(f);
    painter.drawText( X, Y, _ButtonType );
    f.setBold(false);
    painter.setFont(f);

    QString _param("");
    switch( SSXMSGS::g_BanksSettings.at(bank).buttonType[number-1] ){
    case SSXMSGS::ButtonType::PRESET_CHANGE:
        for( int i=0; i<4; i++ ){
            QString _subparam("off");

            if (SSXMSGS::g_BanksSettings[bank].buttonContext[number-1].presetChangeContext.midiChannelNumbers[i]!= MIDI_CHANNEL_NOT_ACTIVE){
                _subparam = QString("%1").arg(QPresetChangeWidget::presetNumber( true, i, bank, number ));
            }

            if (0==(i%2)){
                X=0; Y+= delta;
            }
            else {
                X = 60;
            }
            _param = QString("m%1: %2").arg(i+1).arg(_subparam);
            painter.drawText( X, Y, _param );
        }
        printRelays(X,Y,delta,painter,number,bank);
        //IA
        {
            uint16_t state = SSXMSGS::g_BanksSettings[bank].buttonContext[number-1].presetChangeContext.iaState;
            X=0; Y+= delta;
            painter.drawText( X, Y, "IA:" );
            X=20;
            for (int i=0; i<12; i++ ){
                if((state >> i) & 0x0001)
                    painter.setBrush(Qt::green);
                else
                    painter.setBrush(Qt::gray);
                painter.drawEllipse(QPointF(X,Y-fm.ascent()/2 + 1), fm.ascent()/3, fm.ascent()/3);
                X+=(fm.ascent()/3)*2+2;
            }
            X=0;
        }
        break;
    case SSXMSGS::ButtonType::CC_TOGGLE:
    case SSXMSGS::ButtonType::CC_MOMENTARY:
    case SSXMSGS::ButtonType::CC_CONST_VAL:
        if (SSXMSGS::ButtonType::CC_TOGGLE==SSXMSGS::g_BanksSettings.at(bank).buttonType[number-1]){
            uint8_t ctl = SSXMSGS::g_BanksSettings[bank].buttonContext[number-1].commonContext.contolAndNrpnChangeContext_.ctrlLsbNumber;
            _param = QString("ctl# %1").arg( (ctl>127)?QString("off"):QString("%1").arg(ctl));
            Y+= delta;
            painter.drawText( X, Y, _param );

            uint8_t frz = SSXMSGS::g_BanksSettings[bank].buttonContext[number-1].commonContext.contolAndNrpnChangeContext_.ctrlMsbFreezeNumber;
            _param = QString("frz# %1").arg( (frz>127)?QString("off"):QString("%1").arg(frz));
            painter.drawText( X+60, Y, _param );

            Y+= delta;
            _param =
            (SSXMSGS::g_BanksSettings[bank].buttonContext[number-1].commonContext.contolAndNrpnChangeContext_.autoSendState>0)?
                        QString("Send Value"):QString("");
            painter.drawText( X, Y, _param );
        }
        else {
            uint8_t data = SSXMSGS::g_BanksSettings[bank].buttonContext[number-1].commonContext.contolAndNrpnChangeContext_.ctrlLsbNumber;
            _param = QString("Controller# %1").arg( (data>127)?QString("off"):QString("%1").arg(data));
            Y+= delta;
            painter.drawText( X, Y, _param );
            if ( SSXMSGS::ButtonType::CC_CONST_VAL==SSXMSGS::g_BanksSettings.at(bank).buttonType[number-1] ) {
                data = SSXMSGS::g_BanksSettings[bank].buttonContext[number-1].commonContext.contolAndNrpnChangeContext_.paramLsbOnValue;
                _param = QString("On Value: %1").arg( (data>127)?QString("off"):QString("%1").arg(data));
                Y+= delta;
                painter.drawText( X, Y, _param );
            }
            else
                Y+= delta;
        }
        printRelays(X,Y,delta,painter,number,bank);
        break;
    case SSXMSGS::ButtonType::NRPN_TOGGLE:
    case SSXMSGS::ButtonType::NRPN_MOMENTARY:
    case SSXMSGS::ButtonType::NRPN_CONST_VAL:
        Y+= delta;
        _param = QString("#99: %1").arg(SSXMSGS::g_BanksSettings[bank].buttonContext[number-1].commonContext.contolAndNrpnChangeContext_.ctrlMsbFreezeNumber);
        painter.drawText( X, Y, _param );
        _param = QString("#98: %1").arg(SSXMSGS::g_BanksSettings[bank].buttonContext[number-1].commonContext.contolAndNrpnChangeContext_.ctrlLsbNumber);
        painter.drawText( X +60, Y, _param );
        Y+= delta;
        if ( SSXMSGS::ButtonType::NRPN_MOMENTARY != SSXMSGS::g_BanksSettings.at(bank).buttonType[number-1] ){
            _param = (SSXMSGS::g_BanksSettings[bank].buttonContext[number-1].commonContext.contolAndNrpnChangeContext_.autoSendState>0)?
                        QString("Send Value"):QString("");
            painter.drawText( X, Y, _param );
        }
        printRelays(X,Y,delta,painter,number,bank);
        break;
    case SSXMSGS::ButtonType::NOTE_ON:
    case SSXMSGS::ButtonType::NOTE_OFF:
        Y+= delta;
        Y+= delta;
        _param = QString("Note: %1").arg(
                    SSXMSGS::g_BanksSettings[bank].buttonContext[number-1].commonContext.noteOnConstContext_.noteNumber
        );
        painter.drawText( X, Y, _param );
        Y+= delta;
        _param = QString("Velocity: %1").arg(
                    SSXMSGS::g_BanksSettings[bank].buttonContext[number-1].commonContext.noteOnConstContext_.noteVelocity
        );
        painter.drawText( X, Y, _param );
        break;
    case SSXMSGS::ButtonType::PRESET_UP:
        break;
    case SSXMSGS::ButtonType::PRESET_DOWN:
        break;
    case SSXMSGS::ButtonType::BANK_UP:
        break;
    case SSXMSGS::ButtonType::BANK_DOWN:
        break;
    case SSXMSGS::ButtonType::BANK_TO:
        _param = QString("Bank number: %1").arg(
                    SSXMSGS::g_BanksSettings.at(bank).buttonContext[number-1].commonContext.bankNumber);
        Y+= delta;
        Y+= delta;
        painter.drawText( X, Y, _param );

        break;
    default:;
    }

    return(pix);
}

void QTBbutton::update( int bank ){

    qCDebug(QTB) << Q_FUNC_INFO << "btnNumber=" << m_btnNumber << "bank=" << bank << "blanked=" << m_blanked;

    QPixmap pix;
    if (m_blanked){
        pix = QTBbutton::getIcon( m_btnNumber );
        setDisabled(true);
    }
    else {
        pix = QTBbutton::getIcon( m_btnNumber, bank );
        setEnabled(true);
    }
    QIcon icon(pix);
    setIcon( QIcon(pix));
}

void QTBbutton::setBlanked( bool blanked ){
    qCDebug(QTB) << Q_FUNC_INFO << "btnNumber=" << m_btnNumber << "blanked=" << blanked;
    m_blanked = blanked;
}

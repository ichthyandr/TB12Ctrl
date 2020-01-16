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
    X=0; QString _param("");
    for ( int i=0; i<4; i++ ){
        uint8_t data    = SSXMSGS::g_BanksSettings[bank].buttonContext[number-1].relays;
        int     res     = (data >> (i*2)) & 0x03;
        QString _state("Undef");

        switch ( res ){
        case RELAY_MASK_NO_ACTION:
            _state = "No Action";
            break;
        case RELAY_MASK_CLOSE:
            _state = "Close";
            break;
        case RELAY_MASK_OPEN:
            _state = "Open";
            break;
        case RELAY_MASK_MOMENTARY:
            _state = "Momentary";
            break;
        }
        Y+= delta;
        _param = QString("Relay %1: %2").arg(i+1).arg(_state);
        painter.drawText( X, Y, _param );
    }
}

QPixmap QTBbutton::getIcon( const int number, const int bank ){
    QPixmap pix(120,120);
    pix.fill(Qt::transparent);
    QPainter painter(&pix);
    painter.setPen( QColor(Qt::black) );
    QFont f( "Helvetica", 8 );
    painter.setFont(f);
    QFontMetrics fm(f);

    int X = 0;
    int Y = fm.ascent();
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
    QString _ButtonType = buttonType.value( SSXMSGS::g_BanksSettings.at(bank).buttonType[number-1] );
    Y+= fm.ascent();
    Y+= fm.ascent();
    painter.drawText( X, Y, _ButtonType );

    char _alias[BUTTON_NAME_MAX_SIZE+1];
    ::memset( _alias, 0, BUTTON_NAME_MAX_SIZE+1);
    ::memcpy( _alias, SSXMSGS::g_BanksSettings.at(bank).buttonContext->nameAlias, BUTTON_NAME_MAX_SIZE);
    QString _aliasName(_alias);
    if (_aliasName.isEmpty()) _aliasName = "Alias Name";
    _aliasName = QString("[%1]").arg(_aliasName);
    Y+= fm.ascent();
    painter.drawText( X, Y, _aliasName );

    QString _param("");
    switch( SSXMSGS::g_BanksSettings.at(bank).buttonType[number-1] ){
    case SSXMSGS::ButtonType::PRESET_CHANGE:
        for( int i=0; i<4; i++ ){
            QString _subparam("off");

            if (SSXMSGS::g_BanksSettings[bank].buttonContext[number-1].presetChangeContext.midiChannelNumbers[i]!= MIDI_CHANNEL_NOT_ACTIVE){
                _subparam = QString("%1").arg(QPresetChangeWidget::presetNumber( true, i, bank, number ));
            }

            if (0==(i%2)){
                X=0; Y+= fm.ascent();
            }
            else {
                X = 60;
            }
            _param = QString("m%1: %2").arg(i+1).arg(_subparam);
            painter.drawText( X, Y, _param );
        }
        printRelays(X,Y,fm.ascent(),painter,number,bank);
        break;
    case SSXMSGS::ButtonType::CC_TOGGLE:
    case SSXMSGS::ButtonType::CC_MOMENTARY:
    case SSXMSGS::ButtonType::CC_CONST_VAL:
        if (SSXMSGS::ButtonType::CC_TOGGLE==SSXMSGS::g_BanksSettings.at(bank).buttonType[number-1]){
            Y+= fm.ascent();
            _param = QString("ctl# %1");
            painter.drawText( X, Y, _param );
            _param = QString("frz# %1");
            painter.drawText( X+60, Y, _param );
            Y+= fm.ascent();
            _param = QString("Send Value");
            painter.drawText( X, Y, _param );
        }
        else {
            uint8_t data = SSXMSGS::g_BanksSettings[bank].buttonContext[number-1].commonContext.contolAndNrpnChangeContext_.ctrlLsbNumber;
            _param = QString("Controller# %1").arg( (data>127)?QString("off"):QString("%1").arg(data));
            Y+= fm.ascent();
            painter.drawText( X, Y, _param );
            Y+= fm.ascent();
        }
        printRelays(X,Y,fm.ascent(),painter,number,bank);
        break;
    case SSXMSGS::ButtonType::NRPN_TOGGLE:
    case SSXMSGS::ButtonType::NRPN_MOMENTARY:
    case SSXMSGS::ButtonType::NRPN_CONST_VAL:
        Y+= fm.ascent();
        _param = QString("#99: %1").arg(SSXMSGS::g_BanksSettings[bank].buttonContext[number-1].commonContext.contolAndNrpnChangeContext_.ctrlMsbFreezeNumber);
        painter.drawText( X, Y, _param );
        _param = QString("#98: %1").arg(SSXMSGS::g_BanksSettings[bank].buttonContext[number-1].commonContext.contolAndNrpnChangeContext_.ctrlLsbNumber);
        painter.drawText( X +60, Y, _param );
        Y+= fm.ascent();
        if ( SSXMSGS::ButtonType::NRPN_MOMENTARY != SSXMSGS::g_BanksSettings.at(bank).buttonType[number-1] ){
            _param = (SSXMSGS::g_BanksSettings[bank].buttonContext[number-1].commonContext.contolAndNrpnChangeContext_.autoSendState>0)?
                        QString("Send Value"):QString("");
            painter.drawText( X, Y, _param );
        }
        printRelays(X,Y,fm.ascent(),painter,number,bank);
        break;
    case SSXMSGS::ButtonType::NOTE_ON:
    case SSXMSGS::ButtonType::NOTE_OFF:
        Y+= fm.ascent();
        Y+= fm.ascent();
        _param = QString("Note: %1").arg(
                    SSXMSGS::g_BanksSettings[bank].buttonContext[number-1].commonContext.noteOnConstContext_.noteNumber
        );
        painter.drawText( X, Y, _param );
        Y+= fm.ascent();
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
        Y+= fm.ascent();
        Y+= fm.ascent();
        painter.drawText( X, Y, _param );
        break;
    default:;
    }


    qCDebug(QTB) << Q_FUNC_INFO << "UPDATED!" << _ButtonType;
    return(pix);
}

void QTBbutton::update( int bank ){
    QPixmap pix = QTBbutton::getIcon( m_btnNumber, bank );
    QIcon icon(pix);
    setIcon( QIcon(pix));
    //update
}

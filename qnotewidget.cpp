#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>

#include "qhomewidget.h"
#include "qnotewidget.h"
#include "qmidi/qmidisysexmsgs.h"
Q_LOGGING_CATEGORY( NOTE, "NOTE" )

static QNoteWidget *   g_noteON{Q_NULLPTR};
static QNoteWidget *   g_noteOFF{Q_NULLPTR};

QNoteWidget::QNoteWidget(bool noteON, QWidget *parent): QWidget(parent), m_bNoteOn(noteON) {
    qCDebug(NOTE) << Q_FUNC_INFO;
    if (noteON)
        g_noteON = this;
    else
        g_noteOFF= this;

    auto vMainLayout = new QVBoxLayout();
    setLayout( vMainLayout );

    auto hMainLayout = new QHBoxLayout();
    vMainLayout->addLayout( hMainLayout );
    vMainLayout->addStretch(1000);

    QString title = (noteON)?tr("Note ON"):tr("Note OFF");
    auto gbNOTE = new QGroupBox(title);

    hMainLayout->addWidget(gbNOTE);
    hMainLayout->addStretch(1000);

    auto grid = new QGridLayout();
    gbNOTE->setLayout( grid );

    auto lbl1   = new QLabel(tr("Note number"));
    auto sb1    = new QSpinBox(); sb1->setRange(0, 127); sb1->setWrapping(true);

    auto lbl2   = new QLabel(tr("Velocity"));
    auto sb2    = new QSpinBox(); sb2->setRange(0, 127); sb2->setWrapping(true);

    m_labels    << lbl1<< lbl2;
    m_spinBoxes << sb1 << sb2;

    grid->addWidget( lbl1,   0, 0, Qt::AlignRight);
    grid->addWidget( sb1,    0, 1, Qt::AlignLeft);

    grid->addWidget( lbl2,   1, 0, Qt::AlignRight);
    grid->addWidget( sb2,    1, 1, Qt::AlignLeft);

    connect( sb1, QOverload<int>::of(&QSpinBox::valueChanged), [=](int newValue){
        if (!m_bProceed) return;
        qCDebug(NOTE) << Q_FUNC_INFO << "newValue=" << newValue << "m_bProceed=" << m_bProceed;
        uint8_t data = static_cast<uint8_t>(newValue);
        SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].commonContext.noteOnConstContext_.noteNumber = data;
        emit s_changed(m_curBnkNumber);
    });
    connect( sb2, QOverload<int>::of(&QSpinBox::valueChanged), [=](int newValue){
        if (!m_bProceed) return;
        qCDebug(NOTE) << Q_FUNC_INFO << "newValue=" << newValue << "m_bProceed=" << m_bProceed;
        uint8_t data = static_cast<uint8_t>(newValue);
        SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].commonContext.noteOnConstContext_.noteVelocity = data;
        emit s_changed(m_curBnkNumber);
    });

    connect( this,  &QNoteWidget::s_changed,    QHomeWidget::getInstance(), &QHomeWidget::_changed );
}

QNoteWidget * QNoteWidget::getInstance(bool noteON){
    if (noteON) return(g_noteON);
    return(g_noteOFF);
}

void QNoteWidget::setButton(int btnNumber, int curBnkNumber){
    qCDebug(NOTE) << Q_FUNC_INFO << "btnNumber=" << btnNumber << "curBnkNumber=" << curBnkNumber;
    if ( curBnkNumber<0 ){
        qCCritical(NOTE) << Q_FUNC_INFO << "got bad bank number!";
        return;
    }
    if (!(( btnNumber>=1 ) && ( btnNumber<=FOOT_BUTTONS_NUM ))){
        qCCritical(NOTE) << Q_FUNC_INFO << "got bad button number!";
        return;
    }
    m_btnNumber     = btnNumber;
    m_curBnkNumber  = curBnkNumber;

    SSXMSGS::ButtonType buttonType = SSXMSGS::g_BanksSettings.at(m_curBnkNumber).buttonType[m_btnNumber-1];
    if ( (m_bNoteOn) && (buttonType != SSXMSGS::ButtonType::NOTE_ON)) {
        qCCritical(NOTE) << Q_FUNC_INFO << "got bad button type for Note ON!";
        return;
    }
    if ( (!m_bNoteOn) && (buttonType != SSXMSGS::ButtonType::NOTE_OFF)) {
        qCCritical(NOTE) << Q_FUNC_INFO << "got bad button type for Note OFF!";
        return;
    }

    ////// start of init ////
    m_bProceed=false;
    /// установка Note Number
    auto sb1 = m_spinBoxes.at(0);
    uint8_t value1 =
            SSXMSGS::g_BanksSettings.at(m_curBnkNumber).buttonContext[m_btnNumber-1].commonContext.noteOnConstContext_.noteNumber;
    if ( value1 > 127 ){
        value1 = 127;
        SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].commonContext.noteOnConstContext_.noteNumber = 127;
    }
    sb1->setValue(value1);
    /// установка Note Velocity
    auto sb2 = m_spinBoxes.at(1);
    uint8_t value2 =
            SSXMSGS::g_BanksSettings.at(m_curBnkNumber).buttonContext[m_btnNumber-1].commonContext.noteOnConstContext_.noteVelocity;
    if ( value2 > 127 ){
        value2 = 127;
        SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].commonContext.noteOnConstContext_.noteVelocity = 127;
    }
    sb2->setValue(value2);
    ////// end of init /////
    m_bProceed=true;
}


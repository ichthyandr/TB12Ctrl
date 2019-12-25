#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>

#include "qhomewidget.h"
#include "qbank2widget.h"
#include "qmidi/qmidisysexmsgs.h"
Q_LOGGING_CATEGORY( BANK2, "BANK2" )

static QBank2Widget *   g_bank2{Q_NULLPTR};

QBank2Widget::QBank2Widget(QWidget *parent) : QWidget(parent){
    qCDebug(BANK2) << Q_FUNC_INFO;

    g_bank2 = this;

    auto vMainLayout = new QVBoxLayout();
    setLayout( vMainLayout );

    auto hMainLayout = new QHBoxLayout();
    vMainLayout->addLayout( hMainLayout );
    vMainLayout->addStretch(1000);

    auto gbBank2 = new QGroupBox(tr("Bank To"));

    hMainLayout->addWidget(gbBank2);
    hMainLayout->addStretch(1000);

    auto hb = new QHBoxLayout();
    gbBank2->setLayout(hb);

    auto lbl1   = new QLabel(tr("Bank number"));
    auto sb1    = new QSpinBox(); sb1->setRange(0, 127); sb1->setWrapping(true);

    hb->addWidget( lbl1 );
    hb->addWidget( sb1 );

    m_labels    << lbl1;
    m_spinBoxes << sb1;

    connect( sb1, QOverload<int>::of(&QSpinBox::valueChanged), [=](int newValue){
        if (!m_bProceed) return;
        qCDebug(BANK2) << Q_FUNC_INFO << "newValue=" << newValue << "m_bProceed=" << m_bProceed;
        uint8_t data = static_cast<uint8_t>(newValue);
        SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].commonContext.bankNumber = data;
        emit s_changed(m_curBnkNumber);
    });

    connect( this,  &QBank2Widget::s_changed,   QHomeWidget::getInstance(), &QHomeWidget::_changed );
}

QBank2Widget::~QBank2Widget(){
    qCDebug(BANK2) << Q_FUNC_INFO;
}

QBank2Widget * QBank2Widget::getInstance(){
    return(g_bank2);
}

void QBank2Widget::setButton(int btnNumber, int curBnkNumber){
    qCDebug(BANK2) << Q_FUNC_INFO << "btnNumber=" << btnNumber << "curBnkNumber=" << curBnkNumber;
    if ( curBnkNumber<0 ){
        qCCritical(BANK2) << Q_FUNC_INFO << "got bad bank number!";
        return;
    }
    if (!(( btnNumber>=1 ) && ( btnNumber<=FOOT_BUTTONS_NUM ))){
        qCCritical(BANK2) << Q_FUNC_INFO << "got bad button number!";
        return;
    }
    m_btnNumber     = btnNumber;
    m_curBnkNumber  = curBnkNumber;

    SSXMSGS::ButtonType buttonType = SSXMSGS::g_BanksSettings.at(m_curBnkNumber).buttonType[m_btnNumber-1];
    if (  (buttonType != SSXMSGS::ButtonType::BANK_TO)) {
        qCCritical(BANK2) << Q_FUNC_INFO << "got bad button type!";
        return;
    }

    ////// start of init ////
    m_bProceed=false;
    /// установка Bank Number
    auto sb1 = m_spinBoxes.at(0);
    uint8_t value1 =
            SSXMSGS::g_BanksSettings.at(m_curBnkNumber).buttonContext[m_btnNumber-1].commonContext.bankNumber;
    if ( value1 > 127 ){
        value1 = 127;
        SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].commonContext.bankNumber = 127;
    }
    sb1->setValue(value1);
    ////// end of init /////
    m_bProceed=true;
}

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>

#include "qhomewidget.h"
#include "qnrpntwidget.h"
#include "qmidi/qmidisysexmsgs.h"

Q_LOGGING_CATEGORY( NPRNT, "NPRNT" )

static QNrpnTWidget *   g_NRPNt{Q_NULLPTR};
static QNrpnTWidget *   g_NRPNm{Q_NULLPTR};

QNrpnTWidget::QNrpnTWidget(bool toggle, QWidget *parent) : QWidget(parent), m_bToggle(toggle){
    qCDebug(NPRNT) << Q_FUNC_INFO;

    if (m_bToggle)
        g_NRPNt = this;
    else
        g_NRPNm = this;

    auto vMainLayout = new QVBoxLayout();
    setLayout( vMainLayout );

    auto hMainLayout = new QHBoxLayout();
    vMainLayout->addLayout( hMainLayout );
    vMainLayout->addStretch(1000);

    auto grid = new QGridLayout();
    hMainLayout->addLayout( grid );
    hMainLayout->addStretch(1000);

    QString title = (m_bToggle)?tr("NRPN Toggle"):tr("NRPN Momentary");

    auto gbNPRNt    = new QGroupBox(title);
    auto gbRS       = new QGroupBox(tr("Relay States"));

    grid->addWidget( gbNPRNt, 0, 0, Qt::AlignLeft );
    grid->addWidget( gbRS,  0, 1, Qt::AlignLeft );

    // NRPN Toggle
    auto vbxNPRNt  = new QVBoxLayout();
    auto gridNPRNt = new QGridLayout();

    gbNPRNt->setLayout(vbxNPRNt);
    vbxNPRNt->addLayout(gridNPRNt);
    vbxNPRNt->addStretch(1000);

    auto lbl1   = new QLabel(tr("CC#99"));
    auto sb1    = new QSpinBox(); sb1->setRange(0, 127); sb1->setWrapping(true);

    auto lbl2   = new QLabel(tr("CC#98"));
    auto sb2    = new QSpinBox(); sb2->setRange(0, 127); sb2->setWrapping(true);

    auto lbl3   = new QLabel(tr("CC#6 ON value"));
    auto sb3    = new QSpinBox(); sb3->setRange(0, 127); sb3->setWrapping(true);

    auto lbl4   = new QLabel(tr("CC#38 ON value"));
    auto sb4    = new QSpinBox(); sb4->setRange(0, 127); sb4->setWrapping(true);

    auto lbl5   = new QLabel(tr("CC#6 OFF value"));
    auto sb5    = new QSpinBox(); sb5->setRange(0, 127); sb5->setWrapping(true);

    auto lbl6   = new QLabel(tr("CC#38 OFF value"));
    auto sb6    = new QSpinBox(); sb6->setRange(0, 127); sb6->setWrapping(true);

    auto chb1   = new QCheckBox(tr("Send value when preset changed"));

    m_labels    << lbl1<< lbl2<< lbl3<< lbl4<< lbl5<< lbl6;
    m_spinBoxes << sb1 << sb2 << sb3 << sb4 << sb5 << sb6;
    m_checkBoxes<< chb1;

    gridNPRNt->addWidget( lbl1,   0, 0, Qt::AlignRight);
    gridNPRNt->addWidget( sb1,    0, 1, Qt::AlignLeft);

    gridNPRNt->addWidget( lbl2,   1, 0, Qt::AlignRight);
    gridNPRNt->addWidget( sb2,    1, 1, Qt::AlignLeft);

    gridNPRNt->addWidget( lbl3,   2, 0, Qt::AlignRight);
    gridNPRNt->addWidget( sb3,    2, 1, Qt::AlignLeft);

    gridNPRNt->addWidget( lbl4,   3, 0, Qt::AlignRight);
    gridNPRNt->addWidget( sb4,    3, 1, Qt::AlignLeft);

    gridNPRNt->addWidget( lbl5,   4, 0, Qt::AlignRight);
    gridNPRNt->addWidget( sb5,    4, 1, Qt::AlignLeft);

    gridNPRNt->addWidget( lbl6,   5, 0, Qt::AlignRight);
    gridNPRNt->addWidget( sb6,    5, 1, Qt::AlignLeft);

    if (m_bToggle)
        gridNPRNt->addWidget( chb1,   6, 0, 1, 2, Qt::AlignLeft);

    connect( sb1, QOverload<int>::of(&QSpinBox::valueChanged), [=](int newValue){
        if (!m_bProceed) return;
        qCDebug(NPRNT) << Q_FUNC_INFO << "newValue=" << newValue << "m_bProceed=" << m_bProceed;
        uint8_t data = static_cast<uint8_t>(newValue);
        SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].commonContext.contolAndNrpnChangeContext_.ctrlMsbFreezeNumber = data;
        emit s_changed(m_curBnkNumber);
    });
    connect( sb2, QOverload<int>::of(&QSpinBox::valueChanged), [=](int newValue){
        if (!m_bProceed) return;
        qCDebug(NPRNT) << Q_FUNC_INFO << "newValue=" << newValue << "m_bProceed=" << m_bProceed;
        uint8_t data = static_cast<uint8_t>(newValue);
        SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].commonContext.contolAndNrpnChangeContext_.ctrlLsbNumber = data;
        emit s_changed(m_curBnkNumber);
    });
    connect( sb3, QOverload<int>::of(&QSpinBox::valueChanged), [=](int newValue){
        if (!m_bProceed) return;
        qCDebug(NPRNT) << Q_FUNC_INFO << "newValue=" << newValue << "m_bProceed=" << m_bProceed;
        uint8_t data = static_cast<uint8_t>(newValue);
        SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].commonContext.contolAndNrpnChangeContext_.paramMsbOnValue = data;
        emit s_changed(m_curBnkNumber);
    });
    connect( sb4, QOverload<int>::of(&QSpinBox::valueChanged), [=](int newValue){
        if (!m_bProceed) return;
        qCDebug(NPRNT) << Q_FUNC_INFO << "newValue=" << newValue << "m_bProceed=" << m_bProceed;
        uint8_t data = static_cast<uint8_t>(newValue);
        SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].commonContext.contolAndNrpnChangeContext_.paramLsbOnValue = data;
        emit s_changed(m_curBnkNumber);
    });
    connect( sb5, QOverload<int>::of(&QSpinBox::valueChanged), [=](int newValue){
        if (!m_bProceed) return;
        qCDebug(NPRNT) << Q_FUNC_INFO << "newValue=" << newValue << "m_bProceed=" << m_bProceed;
        uint8_t data = static_cast<uint8_t>(newValue);
        SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].commonContext.contolAndNrpnChangeContext_.paramMsbOffValue = data;
        emit s_changed(m_curBnkNumber);
    });
    connect( sb6, QOverload<int>::of(&QSpinBox::valueChanged), [=](int newValue){
        if (!m_bProceed) return;
        qCDebug(NPRNT) << Q_FUNC_INFO << "newValue=" << newValue << "m_bProceed=" << m_bProceed;
        uint8_t data = static_cast<uint8_t>(newValue);
        SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].commonContext.contolAndNrpnChangeContext_.paramLsbOffValue = data;
        emit s_changed(m_curBnkNumber);
    });
    if (m_bToggle){
        connect( chb1,QOverload<int>::of(&QCheckBox::stateChanged), [=](int state){
            if (!m_bProceed) return;
            qCDebug(NPRNT) << Q_FUNC_INFO << "state=" << state << "m_bProceed=" << m_bProceed;
            SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].commonContext.contolAndNrpnChangeContext_.autoSendState =
                    (Qt::Checked == state)?1:0;
            emit s_changed(m_curBnkNumber);
        });
    }

    // Relay States
    auto vbxRS  = new QVBoxLayout();
    auto gridRS = new QGridLayout();
    gbRS->setLayout(vbxRS);
    vbxRS->addLayout(gridRS);
    vbxRS->addStretch(1000);

    auto rlbl1 = new QLabel(tr("Relay 1"));
    auto rlbl2 = new QLabel(tr("Relay 2"));
    auto rlbl3 = new QLabel(tr("Relay 3"));
    auto rlbl4 = new QLabel(tr("Relay 4"));

    auto rlcbx1 = new QComboBox();
    auto rlcbx2 = new QComboBox();
    auto rlcbx3 = new QComboBox();
    auto rlcbx4 = new QComboBox();

    gridRS->addWidget(rlbl1, 0, 0 );
    gridRS->addWidget(rlbl2, 1, 0 );
    gridRS->addWidget(rlbl3, 2, 0 );
    gridRS->addWidget(rlbl4, 3, 0 );

    gridRS->addWidget(rlcbx1, 0, 1 );
    gridRS->addWidget(rlcbx2, 1, 1 );
    gridRS->addWidget(rlcbx3, 2, 1 );
    gridRS->addWidget(rlcbx4, 3, 1 );

    rlcbx1->addItem(tr("No Action"),    QVariant(RELAY_MASK_NO_ACTION));
    rlcbx1->addItem(tr("Close"),        QVariant(RELAY_MASK_CLOSE));
    rlcbx1->addItem(tr("Open"),         QVariant(RELAY_MASK_OPEN));
    rlcbx1->addItem(tr("Momentary"),    QVariant(RELAY_MASK_MOMENTARY));

    rlcbx2->addItem(tr("No Action"),    QVariant(RELAY_MASK_NO_ACTION));
    rlcbx2->addItem(tr("Close"),        QVariant(RELAY_MASK_CLOSE));
    rlcbx2->addItem(tr("Open"),         QVariant(RELAY_MASK_OPEN));
    rlcbx2->addItem(tr("Momentary"),    QVariant(RELAY_MASK_MOMENTARY));

    rlcbx3->addItem(tr("No Action"),    QVariant(RELAY_MASK_NO_ACTION));
    rlcbx3->addItem(tr("Close"),        QVariant(RELAY_MASK_CLOSE));
    rlcbx3->addItem(tr("Open"),         QVariant(RELAY_MASK_OPEN));
    rlcbx3->addItem(tr("Momentary"),    QVariant(RELAY_MASK_MOMENTARY));

    rlcbx4->addItem(tr("No Action"),    QVariant(RELAY_MASK_NO_ACTION));
    rlcbx4->addItem(tr("Close"),        QVariant(RELAY_MASK_CLOSE));
    rlcbx4->addItem(tr("Open"),         QVariant(RELAY_MASK_OPEN));
    rlcbx4->addItem(tr("Momentary"),    QVariant(RELAY_MASK_MOMENTARY));

    m_relCombos << rlcbx1 << rlcbx2 << rlcbx3 << rlcbx4;

    connect(rlcbx1, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index){
        relays(index, 0 , rlcbx1);
    });
    connect(rlcbx2, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index){
        relays(index, 2 , rlcbx2);
    });
    connect(rlcbx3, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index){
        relays(index, 4 , rlcbx3);
    });
    connect(rlcbx4, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index){
        relays(index, 6 , rlcbx4);
    });

    connect( this, &QNrpnTWidget::s_changed,    QHomeWidget::getInstance(), &QHomeWidget::_changed );
}

void QNrpnTWidget::relays( int index, int shift, QComboBox * combo ){
    qCDebug(NPRNT) << Q_FUNC_INFO << "index=" << index << "shift=" << shift << "m_bProceed=" << m_bProceed << "combo=" << hex << combo;
    if (!m_bProceed) return;
    if (Q_NULLPTR == combo)return;

    uint8_t mask = static_cast<uint8_t>(0x03 << shift);
    mask = ~mask;
    uint8_t dataALL = SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].relays;
    dataALL &= mask;

    QVariant var = combo->itemData( index);
    bool ok;
    int data = var.toInt(&ok);
    if (!ok){
        qCCritical(NPRNT) << Q_FUNC_INFO << "can't convert to int!";
        return;
    }
    uint8_t _data = static_cast<uint8_t>(data);
    _data <<= shift;
    uint8_t res  = _data | dataALL;
    SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].relays = res;

    emit s_changed(m_curBnkNumber);
}

QNrpnTWidget::~QNrpnTWidget(){
    qCDebug(NPRNT) << Q_FUNC_INFO;
}

QNrpnTWidget * QNrpnTWidget::getInstance( bool toggle ){
    if (toggle) return(g_NRPNt);
    return(g_NRPNm);
}

void QNrpnTWidget::setButton(int btnNumber, int curBnkNumber){
    qCDebug(NPRNT) << Q_FUNC_INFO << "btnNumber=" << btnNumber << "curBnkNumber=" << curBnkNumber;
    if ( curBnkNumber<0 ){
        qCCritical(NPRNT) << Q_FUNC_INFO << "got bad bank number!";
        return;
    }
    if (!(( btnNumber>=1 ) && ( btnNumber<=FOOT_BUTTONS_NUM ))){
        qCCritical(NPRNT) << Q_FUNC_INFO << "got bad button number!";
        return;
    }
    m_btnNumber     = btnNumber;
    m_curBnkNumber  = curBnkNumber;

    SSXMSGS::ButtonType buttonType = SSXMSGS::g_BanksSettings.at(m_curBnkNumber).buttonType[m_btnNumber-1];
    if ( m_bToggle && ( buttonType != SSXMSGS::ButtonType::NRPN_TOGGLE) ){
        qCCritical(NPRNT) << Q_FUNC_INFO << "got bad button type for NRPN_TOGGLE!";
        return;
    }
    if ((!m_bToggle) && (buttonType != SSXMSGS::ButtonType::NRPN_MOMENTARY)){
        qCCritical(NPRNT) << Q_FUNC_INFO << "got bad button type for NRPN_MOMENTARY!";
        return;
    }

    ////// start of init ////
    m_bProceed=false;
    /// установка CC#99
    auto sb1 = m_spinBoxes.at(0);
    uint8_t value1 =
            SSXMSGS::g_BanksSettings.at(m_curBnkNumber).buttonContext[m_btnNumber-1].commonContext.contolAndNrpnChangeContext_.ctrlMsbFreezeNumber;
    if ( value1 > 127 ){
        value1 = 127;
        SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].commonContext.contolAndNrpnChangeContext_.ctrlMsbFreezeNumber = 127;
    }
    sb1->setValue(value1);
    /// установка CC#98
    auto sb2 = m_spinBoxes.at(1);
    uint8_t value2 =
            SSXMSGS::g_BanksSettings.at(m_curBnkNumber).buttonContext[m_btnNumber-1].commonContext.contolAndNrpnChangeContext_.ctrlLsbNumber;
    if ( value2 > 127 ){
        value2 = 127;
        SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].commonContext.contolAndNrpnChangeContext_.ctrlLsbNumber = 127;
    }
    sb2->setValue(value2);
    /// установка CC#6 ON value
    auto sb3 = m_spinBoxes.at(2);
    uint8_t value3 =
            SSXMSGS::g_BanksSettings.at(m_curBnkNumber).buttonContext[m_btnNumber-1].commonContext.contolAndNrpnChangeContext_.paramMsbOnValue;
    if ( value3 > 127 ){
        value3 = 127;
        SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].commonContext.contolAndNrpnChangeContext_.paramMsbOnValue = 127;
    }
    sb3->setValue(value3);
    /// установка CC#38 ON value
    auto sb4 = m_spinBoxes.at(3);
    uint8_t value4 =
            SSXMSGS::g_BanksSettings.at(m_curBnkNumber).buttonContext[m_btnNumber-1].commonContext.contolAndNrpnChangeContext_.paramLsbOnValue;
    if ( value4 > 127 ){
        value4 = 127;
        SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].commonContext.contolAndNrpnChangeContext_.paramLsbOnValue = 127;
    }
    sb4->setValue(value4);
    /// установка CC#6 OFF value
    auto sb5 = m_spinBoxes.at(4);
    uint8_t value5 =
            SSXMSGS::g_BanksSettings.at(m_curBnkNumber).buttonContext[m_btnNumber-1].commonContext.contolAndNrpnChangeContext_.paramMsbOffValue;
    if ( value5 > 127 ){
        value5 = 127;
        SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].commonContext.contolAndNrpnChangeContext_.paramMsbOffValue = 127;
    }
    sb5->setValue(value5);
    /// установка CC#38 OFF value
    auto sb6 = m_spinBoxes.at(5);
    uint8_t value6 =
            SSXMSGS::g_BanksSettings.at(m_curBnkNumber).buttonContext[m_btnNumber-1].commonContext.contolAndNrpnChangeContext_.paramLsbOffValue;
    if ( value6 > 127 ){
        value6 = 127;
        SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].commonContext.contolAndNrpnChangeContext_.paramLsbOffValue = 127;
    }
    sb6->setValue(value6);

    if (m_bToggle){
        /// set "Send value when preset changed"
        auto chb1 = m_checkBoxes.at(0);
        uint8_t autoSendState   =
                SSXMSGS::g_BanksSettings.at(m_curBnkNumber).buttonContext[m_btnNumber-1].commonContext.contolAndNrpnChangeContext_.autoSendState;
        chb1->setChecked( 0!= autoSendState );
    }
    //set up relays
    int i = 0;
    for( auto combo: m_relCombos ){
        uint8_t data    = SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].relays;
        int     res     = (data >> i) & 0x03;
        int index = combo->findData(QVariant(res));
        combo->setCurrentIndex(index);
        i+=2;
    }
    ////// end of init /////
    m_bProceed=true;

    qCDebug(NPRNT) << Q_FUNC_INFO << "m_bProceed=" << m_bProceed;
}


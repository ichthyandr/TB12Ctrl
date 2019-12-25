#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>

#include "qhomewidget.h"
#include "qnrpncwidget.h"
#include "qmidi/qmidisysexmsgs.h"
Q_LOGGING_CATEGORY( NPRNC, "NPRNC" )

static QNrpnCWidget *   g_NRPNc{Q_NULLPTR};

QNrpnCWidget::QNrpnCWidget(QWidget *parent) : QWidget(parent){
    qCDebug(NPRNC) << Q_FUNC_INFO;
    g_NRPNc = this;

    auto vMainLayout = new QVBoxLayout();
    setLayout( vMainLayout );

    auto hMainLayout = new QHBoxLayout();
    vMainLayout->addLayout( hMainLayout );
    vMainLayout->addStretch(1000);

    auto grid = new QGridLayout();
    hMainLayout->addLayout( grid );
    hMainLayout->addStretch(1000);

    auto gbNPRNc    = new QGroupBox(tr("NRPN Constant"));
    auto gbRS       = new QGroupBox(tr("Relay States"));

    grid->addWidget( gbNPRNc,   0, 0, Qt::AlignLeft );
    grid->addWidget( gbRS,      0, 1, Qt::AlignLeft );

    // NRPN Constant
    auto vbxNPRNc  = new QVBoxLayout();
    auto gridNPRNc = new QGridLayout();

    gbNPRNc->setLayout(vbxNPRNc);
    vbxNPRNc->addLayout(gridNPRNc);
    vbxNPRNc->addStretch(1000);

    auto lbl1   = new QLabel(tr("CC#99"));
    auto sb1    = new QSpinBox(); sb1->setRange(0, 127); sb1->setWrapping(true);

    auto lbl2   = new QLabel(tr("CC#98"));
    auto sb2    = new QSpinBox(); sb2->setRange(0, 127); sb2->setWrapping(true);

    auto lbl3   = new QLabel(tr("CC#6 value"));
    auto sb3    = new QSpinBox(); sb3->setRange(0, 127); sb3->setWrapping(true);

    auto lbl4   = new QLabel(tr("CC#38 value"));
    auto sb4    = new QSpinBox(); sb4->setRange(0, 127); sb4->setWrapping(true);

    auto chb1   = new QCheckBox(tr("Send value when preset changed"));

    m_labels    << lbl1<< lbl2<< lbl3<< lbl4;
    m_spinBoxes << sb1 << sb2 << sb3 << sb4;
    m_checkBoxes<< chb1;

    gridNPRNc->addWidget( lbl1,   0, 0, Qt::AlignRight);
    gridNPRNc->addWidget( sb1,    0, 1, Qt::AlignLeft);

    gridNPRNc->addWidget( lbl2,   1, 0, Qt::AlignRight);
    gridNPRNc->addWidget( sb2,    1, 1, Qt::AlignLeft);

    gridNPRNc->addWidget( lbl3,   2, 0, Qt::AlignRight);
    gridNPRNc->addWidget( sb3,    2, 1, Qt::AlignLeft);

    gridNPRNc->addWidget( lbl4,   3, 0, Qt::AlignRight);
    gridNPRNc->addWidget( sb4,    3, 1, Qt::AlignLeft);

    gridNPRNc->addWidget( chb1,   6, 0, 1, 2, Qt::AlignLeft);

    connect( sb1, QOverload<int>::of(&QSpinBox::valueChanged), [=](int newValue){
        if (!m_bProceed) return;
        qCDebug(NPRNC) << Q_FUNC_INFO << "newValue=" << newValue << "m_bProceed=" << m_bProceed;
        uint8_t data = static_cast<uint8_t>(newValue);
        SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].commonContext.contolAndNrpnChangeContext_.ctrlMsbFreezeNumber = data;
        emit s_changed(m_curBnkNumber);
    });
    connect( sb2, QOverload<int>::of(&QSpinBox::valueChanged), [=](int newValue){
        if (!m_bProceed) return;
        qCDebug(NPRNC) << Q_FUNC_INFO << "newValue=" << newValue << "m_bProceed=" << m_bProceed;
        uint8_t data = static_cast<uint8_t>(newValue);
        SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].commonContext.contolAndNrpnChangeContext_.ctrlLsbNumber = data;
        emit s_changed(m_curBnkNumber);
    });
    connect( sb3, QOverload<int>::of(&QSpinBox::valueChanged), [=](int newValue){
        if (!m_bProceed) return;
        qCDebug(NPRNC) << Q_FUNC_INFO << "newValue=" << newValue << "m_bProceed=" << m_bProceed;
        uint8_t data = static_cast<uint8_t>(newValue);
        SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].commonContext.contolAndNrpnChangeContext_.paramMsbOnValue = data;
        emit s_changed(m_curBnkNumber);
    });
    connect( sb4, QOverload<int>::of(&QSpinBox::valueChanged), [=](int newValue){
        if (!m_bProceed) return;
        qCDebug(NPRNC) << Q_FUNC_INFO << "newValue=" << newValue << "m_bProceed=" << m_bProceed;
        uint8_t data = static_cast<uint8_t>(newValue);
        SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].commonContext.contolAndNrpnChangeContext_.paramLsbOnValue = data;
        emit s_changed(m_curBnkNumber);
    });
    connect( chb1,QOverload<int>::of(&QCheckBox::stateChanged), [=](int state){
        if (!m_bProceed) return;
        qCDebug(NPRNC) << Q_FUNC_INFO << "state=" << state << "m_bProceed=" << m_bProceed;
        SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].commonContext.contolAndNrpnChangeContext_.autoSendState =
                (Qt::Checked == state)?1:0;
        emit s_changed(m_curBnkNumber);
    });

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

    connect( this,  &QNrpnCWidget::s_changed,   QHomeWidget::getInstance(), &QHomeWidget::_changed );
}

void QNrpnCWidget::relays( int index, int shift, QComboBox * combo ){
    qCDebug(NPRNC) << Q_FUNC_INFO << "index=" << index << "shift=" << shift << "m_bProceed=" << m_bProceed << "combo=" << hex << combo;
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
        qCCritical(NPRNC) << Q_FUNC_INFO << "can't convert to int!";
        return;
    }
    uint8_t _data = static_cast<uint8_t>(data);
    _data <<= shift;
    uint8_t res  = _data | dataALL;
    SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].relays = res;

    emit s_changed(m_curBnkNumber);
}

QNrpnCWidget::~QNrpnCWidget(){
    qCDebug(NPRNC) << Q_FUNC_INFO;
}

QNrpnCWidget * QNrpnCWidget::getInstance(){
    return(g_NRPNc);
}

void QNrpnCWidget::setButton(int btnNumber, int curBnkNumber){
    qCDebug(NPRNC) << Q_FUNC_INFO << "btnNumber=" << btnNumber << "curBnkNumber=" << curBnkNumber;
    if ( curBnkNumber<0 ){
        qCCritical(NPRNC) << Q_FUNC_INFO << "got bad bank number!";
        return;
    }
    if (!(( btnNumber>=1 ) && ( btnNumber<=FOOT_BUTTONS_NUM ))){
        qCCritical(NPRNC) << Q_FUNC_INFO << "got bad button number!";
        return;
    }
    m_btnNumber     = btnNumber;
    m_curBnkNumber  = curBnkNumber;

    SSXMSGS::ButtonType buttonType = SSXMSGS::g_BanksSettings.at(m_curBnkNumber).buttonType[m_btnNumber-1];
    if ( buttonType != SSXMSGS::ButtonType::NRPN_CONST_VAL) {
        qCCritical(NPRNC) << Q_FUNC_INFO << "got bad button type!";
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
    /// установка CC#6 value
    auto sb3 = m_spinBoxes.at(2);
    uint8_t value3 =
            SSXMSGS::g_BanksSettings.at(m_curBnkNumber).buttonContext[m_btnNumber-1].commonContext.contolAndNrpnChangeContext_.paramMsbOnValue;
    if ( value3 > 127 ){
        value3 = 127;
        SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].commonContext.contolAndNrpnChangeContext_.paramMsbOnValue = 127;
    }
    sb3->setValue(value3);
    /// установка CC#38 value
    auto sb4 = m_spinBoxes.at(3);
    uint8_t value4 =
            SSXMSGS::g_BanksSettings.at(m_curBnkNumber).buttonContext[m_btnNumber-1].commonContext.contolAndNrpnChangeContext_.paramLsbOnValue;
    if ( value4 > 127 ){
        value4 = 127;
        SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].commonContext.contolAndNrpnChangeContext_.paramLsbOnValue = 127;
    }
    sb4->setValue(value4);

    /// set "Autosend"
    auto chb1 = m_checkBoxes.at(0);
    uint8_t autoSendState   =
            SSXMSGS::g_BanksSettings.at(m_curBnkNumber).buttonContext[m_btnNumber-1].commonContext.contolAndNrpnChangeContext_.autoSendState;
    chb1->setChecked( 0!= autoSendState );

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
}


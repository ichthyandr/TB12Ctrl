#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QGroupBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QString>
#include <QCheckBox>

#include "qhomewidget.h"
#include "qcccwidget.h"
#include "qmidi/qmidisysexmsgs.h"
Q_LOGGING_CATEGORY( CCC, "CCC" )

static QCccWidget * g_ccc{Q_NULLPTR};

QCccWidget::QCccWidget(QWidget *parent) : QWidget(parent){
    qCDebug(CCC) << Q_FUNC_INFO;

    g_ccc = this;

    auto vMainLayout = new QVBoxLayout();
    setLayout( vMainLayout );

    auto hMainLayout = new QHBoxLayout();
    vMainLayout->addLayout( hMainLayout );
    vMainLayout->addStretch(1000);

    auto grid = new QGridLayout();
    hMainLayout->addLayout( grid );
    hMainLayout->addStretch(1000);

    auto gbCCC  = new QGroupBox(tr("Control Change Constant"));
    auto gbRS   = new QGroupBox(tr("Relay States"));

    grid->addWidget( gbCCC, 0, 0, Qt::AlignLeft );
    grid->addWidget( gbRS,  0, 1, Qt::AlignLeft );

    // Control Change Constant
    auto vbxCCC  = new QVBoxLayout();
    auto gridCCC = new QGridLayout();

    gbCCC->setLayout(vbxCCC);
    vbxCCC->addLayout(gridCCC);
    vbxCCC->addStretch(1000);

    auto lbl1   = new QLabel(tr("Controller #"));
    auto sb1    = new QSpinBox(); sb1->setRange(0, 127); sb1->setWrapping(true);
    auto chb1   = new QCheckBox(tr("Off"));

    auto lbl2   = new QLabel(tr("On value"));
    auto sb2    = new QSpinBox(); sb2->setRange(0, 127); sb2->setWrapping(true);

    m_labels    <<lbl1 <<lbl2;
    m_spinBoxes << sb1 << sb2;
    m_checkBoxes<< chb1;

    gridCCC->addWidget( lbl1,   0, 0, Qt::AlignRight);
    gridCCC->addWidget( sb1,    0, 1, Qt::AlignLeft);
    gridCCC->addWidget( chb1,   0, 2, Qt::AlignLeft);

    gridCCC->addWidget( lbl2,   1, 0, Qt::AlignRight);
    gridCCC->addWidget( sb2,    1, 1, Qt::AlignLeft);

    connect( sb1, QOverload<int>::of(&QSpinBox::valueChanged), [=](int newValue){
        qCDebug(CCC) << Q_FUNC_INFO << "newValue=" << newValue << "m_bProceed=" << m_bProceed;
        if (!m_bProceed) return;
        qCDebug(CCC) << Q_FUNC_INFO << "newValue=" << newValue << "m_bProceed=" << m_bProceed;
        uint8_t data = static_cast<uint8_t>(newValue);
        SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].commonContext.contolAndNrpnChangeContext_.ctrlLsbNumber = data;
        emit s_changed( m_curBnkNumber );
    });
    connect( sb2, QOverload<int>::of(&QSpinBox::valueChanged), [=](int newValue){
        if (!m_bProceed) return;
        qCDebug(CCC) << Q_FUNC_INFO << "newValue=" << newValue << "m_bProceed=" << m_bProceed;
        uint8_t data = static_cast<uint8_t>(newValue);
        SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].commonContext.contolAndNrpnChangeContext_.paramLsbOnValue = data;
        emit s_changed( m_curBnkNumber );
    });

    connect( chb1,QOverload<int>::of(&QCheckBox::stateChanged), [=](int state){
        if (!m_bProceed) return;
        qCDebug(CCC) << Q_FUNC_INFO << "state=" << state << "m_bProceed=" << m_bProceed;

        m_bProceed = false;
        if ( Qt::Unchecked == state ){
            lbl1->setEnabled(true);
            sb1->setEnabled(true);
            SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].commonContext.contolAndNrpnChangeContext_.ctrlLsbNumber = 0;
            sb1->setValue( 0 );
        }
        else if ( Qt::Checked == state ){
            lbl1->setDisabled(true);
            sb1->setDisabled(true);
            sb1->setValue( 0 );
            SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].commonContext.contolAndNrpnChangeContext_.ctrlLsbNumber = 128;
        }
        emit s_changed( m_curBnkNumber );
        m_bProceed = true;
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

    connect( this,  &QCccWidget::s_changed, QHomeWidget::getInstance(), &QHomeWidget::_changed );
}

QCccWidget::~QCccWidget(){
    qCDebug(CCC) << Q_FUNC_INFO;
}

QCccWidget * QCccWidget::getInstance(){
    return(g_ccc);
}

void QCccWidget::setButton(int btnNumber, int curBnkNumber){
    qCDebug(CCC) << Q_FUNC_INFO << "btnNumber=" << btnNumber << "curBnkNumber=" << curBnkNumber;

    if ( curBnkNumber<0 ){
        qCCritical(CCC) << Q_FUNC_INFO << "got bad bank number!";
        return;
    }
    if (!(( btnNumber>=1 ) && ( btnNumber<=FOOT_BUTTONS_NUM ))){
        qCCritical(CCC) << Q_FUNC_INFO << "got bad button number!";
        return;
    }

    m_btnNumber     = btnNumber;
    m_curBnkNumber  = curBnkNumber;

    SSXMSGS::ButtonType buttonType = SSXMSGS::g_BanksSettings.at(m_curBnkNumber).buttonType[m_btnNumber-1];
    if ( buttonType != SSXMSGS::ButtonType::CC_CONST_VAL ){
        qCCritical(CCC) << Q_FUNC_INFO << "got bad button type!";
        return;
    }
    ////// start of init ////
    m_bProceed=false;
    /// установка Controller #
    auto lbl1               = m_labels.at(0);
    auto sb1                = m_spinBoxes.at(0);
    auto chb1               = m_checkBoxes.at(0);
    uint8_t ctrlLsbNumber   =
            SSXMSGS::g_BanksSettings.at(m_curBnkNumber).buttonContext[m_btnNumber-1].commonContext.contolAndNrpnChangeContext_.ctrlLsbNumber;
    if ( 128 <= ctrlLsbNumber ){
        chb1->setChecked(true);
        lbl1->setDisabled(true);
        sb1->setValue(0);
        sb1->setDisabled(true);
        SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].commonContext.contolAndNrpnChangeContext_.ctrlLsbNumber = 128;
    }
    else {
        chb1->setChecked(false);
        lbl1->setEnabled(true);
        sb1->setValue(ctrlLsbNumber);
        sb1->setEnabled(true);
    }
    /// установка ON VALUE
    auto sb2        = m_spinBoxes.at(1);
    uint8_t onValue =
            SSXMSGS::g_BanksSettings.at(m_curBnkNumber).buttonContext[m_btnNumber-1].commonContext.contolAndNrpnChangeContext_.paramLsbOnValue;
    if ( onValue > 127 ){
        onValue = 127;
        SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].commonContext.contolAndNrpnChangeContext_.paramLsbOnValue = 127;
    }
    sb2->setValue(onValue);

    //set up relays
    int i = 0;
    for( auto combo: m_relCombos ){
        uint8_t data    = SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].relays;
        int     res     = (data >> i) & 0x03;
        //qCDebug(CCC) << Q_FUNC_INFO << "i=" << i << hex << "data=" << data << "res=" << res;
        int index = combo->findData(QVariant(res));
        combo->setCurrentIndex(index);
        i+=2;
    }

    ////// end of init /////
    m_bProceed=true;
}

void QCccWidget::relays( int index, uint8_t shift, QComboBox * combo ){
    qCDebug(CCC) << Q_FUNC_INFO << "index=" << index << "shift=" << shift << "m_iaStateProceed=" << m_bProceed << "combo=" << hex << combo;
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
        qCCritical(CCC) << Q_FUNC_INFO << "can't convert to int!";
        return;
    }
    uint8_t _data = static_cast<uint8_t>(data);
    _data <<= shift;
    uint8_t res  = _data | dataALL;
    SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].relays = res;

    emit s_changed( m_curBnkNumber );
}


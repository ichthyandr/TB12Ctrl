#include <QVBoxLayout>
#include "qcmnsetupwidget.h"
Q_LOGGING_CATEGORY( CMN,"CMN")

#include <QComboBox>
#include <QLabel>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QSpinBox>
#include <QCheckBox>
#include "qmidi/qmidisysexmsgs.h"

static QCmnSetupWidget * g_cmnsw{Q_NULLPTR};

QCmnSetupWidget::QCmnSetupWidget(QWidget *parent) : QWidget(parent){
    qCDebug(CMN) << Q_FUNC_INFO;

    g_cmnsw = this;

    auto mainLayout = new QVBoxLayout();
    setLayout(mainLayout);

    auto grid = new QGridLayout();
    mainLayout->addLayout(grid);

    mainLayout->addStretch(1000);

    auto gb1 = new QGroupBox(tr("CC# for current bank"));
    auto gb2 = new QGroupBox(tr("Behavior (common for all banks)"));
    auto gb3 = new QGroupBox(tr("Bank select action"));

    grid->addWidget( gb1, 0, 0);
    grid->addWidget( gb2, 0, 1);
    grid->addWidget( gb3, 1, 0);

    //CC# for current bank
    auto vb1    = new QVBoxLayout();
    auto grid1  = new QGridLayout();

    gb1->setLayout( vb1 );
    vb1->addLayout( grid1 );
    vb1->addStretch(1000);

    auto lbl11  = new QLabel(tr("Exp. pedal 1 CC#"));
    auto sb11   = new QSpinBox(); sb11->setRange(0, 127); sb11->setWrapping(true);
    connect(sb11, QOverload<int>::of(&QSpinBox::valueChanged),[=](int value){
        if ( m_proceed ){
            SSXMSGS::g_BanksSettings[m_curBnkNumber].pedalsCc[0] = static_cast<uint8_t>(value);
            emit s_changed(m_curBnkNumber);
        }
    });

    auto lbl12  = new QLabel(tr("Exp. pedal 2 CC#"));
    auto sb12   = new QSpinBox(); sb12->setRange(0, 127); sb12->setWrapping(true);
    connect(sb12, QOverload<int>::of(&QSpinBox::valueChanged),[=](int value){
        if ( m_proceed ){
            SSXMSGS::g_BanksSettings[m_curBnkNumber].pedalsCc[1] = static_cast<uint8_t>(value);
            emit s_changed(m_curBnkNumber);
        }
    });

    auto lbl13  = new QLabel(tr("Onboard exp. pedal CC#"));
    auto sb13   = new QSpinBox(); sb13->setRange(0, 127); sb13->setWrapping(true);
    connect(sb13, QOverload<int>::of(&QSpinBox::valueChanged),[=](int value){
        if ( m_proceed ){
            SSXMSGS::g_BanksSettings[m_curBnkNumber].pedalsCc[2] = static_cast<uint8_t>(value);
            emit s_changed(m_curBnkNumber);
        }
    });

    auto lbl14  = new QLabel(tr("Onboard exp. pedal alt. CC#"));
    auto sb14   = new QSpinBox(); sb14->setRange(0, 127); sb14->setWrapping(true);
    auto chb14  = new QCheckBox(tr("Off"));
    connect(sb14, QOverload<int>::of(&QSpinBox::valueChanged),[=](int value){
        if ( m_proceed ){
            SSXMSGS::g_BanksSettings[m_curBnkNumber].pedalsCc[3] = static_cast<uint8_t>(value);
            emit s_changed(m_curBnkNumber);
        }
    });
    connect(chb14, QOverload<int>::of(&QCheckBox::stateChanged), [=](int state){
        qCDebug(CMN) << Q_FUNC_INFO << "state=" << state << "m_proceed=" << m_proceed;
        if ( m_proceed ){
            m_proceed = false;
            if ( Qt::Unchecked == state ){
                lbl14->setEnabled(true);
                sb14->setEnabled(true);
                SSXMSGS::g_BanksSettings[m_curBnkNumber].pedalsCc[3] = 0;
                sb14->setValue( 0 );
            }
            else if ( Qt::Checked == state ){
                lbl14->setDisabled(true);
                sb14->setDisabled(true);
                sb14->setValue( 0 );
                SSXMSGS::g_BanksSettings[m_curBnkNumber].pedalsCc[3] = 128;
            }
            emit s_changed(m_curBnkNumber);
            m_proceed = true;
        }
    });

    auto lbl15  = new QLabel(tr("Tuner CC#"));
    auto sb15   = new QSpinBox(); sb15->setRange(0, 127); sb15->setWrapping(true);
    auto chb15  = new QCheckBox(tr("Off"));
    connect(sb15, QOverload<int>::of(&QSpinBox::valueChanged),[=](int value){
        if ( m_proceed ){
            SSXMSGS::g_BanksSettings[m_curBnkNumber].tunerCc = static_cast<uint8_t>(value);
            emit s_changed(m_curBnkNumber);
        }
    });
    connect(chb15, QOverload<int>::of(&QCheckBox::stateChanged), [=](int state){
        qCDebug(CMN) << Q_FUNC_INFO << "state=" << state << "m_proceed=" << m_proceed;
        if ( m_proceed ){
            m_proceed = false;
            if ( Qt::Unchecked == state ){
                lbl15->setEnabled(true);
                sb15->setEnabled(true);
                SSXMSGS::g_BanksSettings[m_curBnkNumber].tunerCc = 0;
                sb15->setValue( 0 );
            }
            else if ( Qt::Checked == state ){
                lbl15->setDisabled(true);
                sb15->setDisabled(true);
                sb15->setValue( 0 );
                SSXMSGS::g_BanksSettings[m_curBnkNumber].tunerCc = 128;
            }
            emit s_changed(m_curBnkNumber);
            m_proceed = true;
        }
    });

    auto lbl16  = new QLabel(tr("Tap CC#"));
    auto sb16   = new QSpinBox(); sb16->setRange(0, 127); sb16->setWrapping(true);
    auto chb16  = new QCheckBox(tr("Off"));
    connect(sb16, QOverload<int>::of(&QSpinBox::valueChanged),[=](int value){
        if ( m_proceed ){
            SSXMSGS::g_BanksSettings[m_curBnkNumber].tapCc = static_cast<uint8_t>(value);
            emit s_changed(m_curBnkNumber);
        }
    });
    connect(chb16, QOverload<int>::of(&QCheckBox::stateChanged), [=](int state){
        qCDebug(CMN) << Q_FUNC_INFO << "state=" << state << "m_proceed=" << m_proceed;
        if ( m_proceed ){
            m_proceed = false;
            if ( Qt::Unchecked == state ){
                lbl16->setEnabled(true);
                sb16->setEnabled(true);
                SSXMSGS::g_BanksSettings[m_curBnkNumber].tapCc = 0;
                sb16->setValue( 0 );
            }
            else if ( Qt::Checked == state ){
                lbl16->setDisabled(true);
                sb16->setDisabled(true);
                sb16->setValue( 0 );
                SSXMSGS::g_BanksSettings[m_curBnkNumber].tapCc = 128;
            }
            emit s_changed(m_curBnkNumber);
            m_proceed = true;
        }
    });


    m_labels << lbl11 << lbl12 << lbl13 << lbl14 << lbl15 << lbl16;
    m_spins  << sb11  << sb12  << sb13  << sb14  << sb15  << sb16;
    m_checks << chb14 << chb15 << chb16;

    grid1->addWidget( lbl11,    0, 0, Qt::AlignRight);
    grid1->addWidget( sb11,     0, 1, Qt::AlignLeft);

    grid1->addWidget( lbl12,    1, 0, Qt::AlignRight);
    grid1->addWidget( sb12,     1, 1, Qt::AlignLeft);

    grid1->addWidget( lbl13,    2, 0, Qt::AlignRight);
    grid1->addWidget( sb13,     2, 1, Qt::AlignLeft);

    grid1->addWidget( lbl14,    3, 0, Qt::AlignRight);
    grid1->addWidget( sb14,     3, 1, Qt::AlignLeft);
    grid1->addWidget( chb14,    3, 2, Qt::AlignLeft);

    grid1->addWidget( lbl15,    4, 0, Qt::AlignRight);
    grid1->addWidget( sb15,     4, 1, Qt::AlignLeft);
    grid1->addWidget( chb15,    4, 2, Qt::AlignLeft);

    grid1->addWidget( lbl16,    5, 0, Qt::AlignRight);
    grid1->addWidget( sb16,     5, 1, Qt::AlignLeft);
    grid1->addWidget( chb16,    5, 2, Qt::AlignLeft);

    //Behavior (common for all banks)
    auto vb2    = new QVBoxLayout();
    auto grid2  = new QGridLayout();

    gb2->setLayout( vb2 );
    vb2->addLayout( grid2 );
    vb2->addStretch(1000);

    auto lbl21  = new QLabel(tr("Exp. pedal 1 type"));
    auto cbx21  = new QComboBox();
    cbx21->addItem(tr("Lineal"),            QVariant(SSXMSGS::ExpPedalType::LINEAL));
    cbx21->addItem(tr("Logarithmic"),       QVariant(SSXMSGS::ExpPedalType::LOGARITHMIC));
    cbx21->addItem(tr("Back Logarithmic"),  QVariant(SSXMSGS::ExpPedalType::BACK_LOGARITHMIC));
    cbx21->setMinimumContentsLength(13);
    connect(cbx21, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index){
        if (m_proceed){
            QVariant var = cbx21->itemData(index);
            bool ok;
            int data = var.toInt(&ok);
            if ( ok ){
                SSXMSGS::g_GlobalSettings.expPtype[0] = static_cast<SSXMSGS::ExpPedalType>(data);
                emit s_changed(-1);
            }
            else
                qCCritical(CMN) << Q_FUNC_INFO << "can't convert data for 'Exp. pedal 1 type'";
        }
    });

    auto lbl22  = new QLabel(tr("Exp. pedal 2 type"));
    auto cbx22  = new QComboBox();
    cbx22->addItem(tr("Lineal"),            QVariant(SSXMSGS::ExpPedalType::LINEAL));
    cbx22->addItem(tr("Logarithmic"),       QVariant(SSXMSGS::ExpPedalType::LOGARITHMIC));
    cbx22->addItem(tr("Back Logarithmic"),  QVariant(SSXMSGS::ExpPedalType::BACK_LOGARITHMIC));
    cbx22->setMinimumContentsLength(13);
    connect(cbx22, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index){
        if (m_proceed){
            QVariant var = cbx22->itemData(index);
            bool ok;
            int data = var.toInt(&ok);
            if ( ok ){
                SSXMSGS::g_GlobalSettings.expPtype[1] = static_cast<SSXMSGS::ExpPedalType>(data);
                emit s_changed(-1);
            }
            else
                qCCritical(CMN) << Q_FUNC_INFO << "can't convert data for 'Exp. pedal 2 type'";
        }
    });

    auto lbl23  = new QLabel(tr("Onboard pedal LEDs view"));
    auto cbx23  = new QComboBox();
    cbx23->addItem(tr("Mode Button"),       QVariant(SSXMSGS::PedalLedView::PEDAL_LED_MODE_BUTTON));
    cbx23->addItem(tr("Mode Tuner"),        QVariant(SSXMSGS::PedalLedView::PEDAL_LED_MODE_TUNER));
    cbx23->addItem(tr("Mode Dual"),         QVariant(SSXMSGS::PedalLedView::PEDAL_LED_MODE_DUAL));
    cbx23->setMinimumContentsLength(13);
    connect(cbx23, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index){
        if (m_proceed){
            QVariant var = cbx23->itemData(index);
            bool ok;
            int data = var.toInt(&ok);
            if ( ok ){
                SSXMSGS::g_GlobalSettings.pedalLedView = static_cast<SSXMSGS::PedalLedView>(data);
                emit s_changed(-1);
            }
            else
                qCCritical(CMN) << Q_FUNC_INFO << "can't convert data for 'Onboard pedal LEDs view'";
        }
    });

    auto lbl24  = new QLabel(tr("Onboard pedal tuner type"));
    auto cbx24  = new QComboBox();
    cbx24->addItem(tr("Scheme1"),           QVariant(SSXMSGS::PedalTunerScheme::Scheme1));
    cbx24->addItem(tr("Scheme2"),           QVariant(SSXMSGS::PedalTunerScheme::Scheme2));
    cbx24->setMinimumContentsLength(13);
    connect(cbx24, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index){
        if (m_proceed){
            QVariant var = cbx24->itemData(index);
            bool ok;
            int data = var.toInt(&ok);
            if ( ok ){
                SSXMSGS::g_GlobalSettings.pedalTunerScheme = static_cast<SSXMSGS::PedalTunerScheme>(data);
                emit s_changed(-1);
            }
            else
                qCCritical(CMN) << Q_FUNC_INFO << "can't convert data for 'Onboard pedal tuner type'";
        }
    });
    // для модели без педали запрещены
    lbl24->setDisabled( SSXMSGS::g_Model < 100 );
    cbx24->setDisabled( SSXMSGS::g_Model < 100 );

    auto lbl25  = new QLabel(tr("Tap message type"));
    auto cbx25  = new QComboBox();
    cbx25->addItem(tr("Tap Toggle"),        QVariant(SSXMSGS::TapType::TAP_TOGGLE));
    cbx25->addItem(tr("Tap Momentrary"),    QVariant(SSXMSGS::TapType::TAP_MOMETRARY));
    cbx25->addItem(tr("Tap Const Value"),   QVariant(SSXMSGS::TapType::TAP_CONST_VALUE));
    cbx25->setMinimumContentsLength(13);
    connect(cbx25, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index){
        if (m_proceed){
            QVariant var = cbx25->itemData(index);
            bool ok;
            int data = var.toInt(&ok);
            if ( ok ){
                SSXMSGS::g_GlobalSettings.tapType = static_cast<SSXMSGS::TapType>(data);
                emit s_changed(-1);
            }
            else
                qCCritical(CMN) << Q_FUNC_INFO << "can't convert data for 'Onboard pedal tuner type'";
        }
    });

    auto lbl26  = new QLabel(tr("Tap display type"));
    auto cbx26  = new QComboBox();
    cbx26->addItem(tr("Not Show Tap"),      QVariant(SSXMSGS::TapDisplayType::NOT_SHOW_TAP));
    cbx26->addItem(tr("On Screen"),         QVariant(SSXMSGS::TapDisplayType::ON_SCREEN));
    cbx26->addItem(tr("On Active Preset"),  QVariant(SSXMSGS::TapDisplayType::ON_ACTIVE_PRESET));
    cbx26->addItem(tr("LED1"),              QVariant(SSXMSGS::TapDisplayType::LED1));
    cbx26->addItem(tr("LED2"),              QVariant(SSXMSGS::TapDisplayType::LED2));
    cbx26->addItem(tr("LED3"),              QVariant(SSXMSGS::TapDisplayType::LED3));
    cbx26->addItem(tr("LED4"),              QVariant(SSXMSGS::TapDisplayType::LED4));
    cbx26->addItem(tr("LED5"),              QVariant(SSXMSGS::TapDisplayType::LED5));
    cbx26->addItem(tr("LED6"),              QVariant(SSXMSGS::TapDisplayType::LED6));
    cbx26->addItem(tr("LED7"),              QVariant(SSXMSGS::TapDisplayType::LED7));
    cbx26->addItem(tr("LED8"),              QVariant(SSXMSGS::TapDisplayType::LED8));
    cbx26->addItem(tr("LED9"),              QVariant(SSXMSGS::TapDisplayType::LED9));
    cbx26->addItem(tr("LED10"),             QVariant(SSXMSGS::TapDisplayType::LED10));
    cbx26->addItem(tr("LED11"),             QVariant(SSXMSGS::TapDisplayType::LED11));
    cbx26->addItem(tr("LED12"),             QVariant(SSXMSGS::TapDisplayType::LED12));
    cbx26->setMinimumContentsLength(13);
    connect(cbx26, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index){
        if (m_proceed){
            QVariant var = cbx26->itemData(index);
            bool ok;
            int data = var.toInt(&ok);
            if ( ok ){
                SSXMSGS::g_GlobalSettings.tapDisplayType = static_cast<SSXMSGS::TapDisplayType>(data);
                emit s_changed(-1);
            }
            else
                qCCritical(CMN) << Q_FUNC_INFO << "can't convert data for 'Tap display type'";
        }
    });

    m_labels << lbl21 << lbl22 << lbl23 << lbl24 << lbl25 << lbl26;
    m_combos << cbx21 << cbx22 << cbx23 << cbx24 << cbx25 << cbx26;

    grid2->addWidget( lbl21,    0, 0, Qt::AlignRight);
    grid2->addWidget( cbx21,    0, 1, Qt::AlignLeft);

    grid2->addWidget( lbl22,    1, 0, Qt::AlignRight);
    grid2->addWidget( cbx22,    1, 1, Qt::AlignLeft);

    grid2->addWidget( lbl23,    2, 0, Qt::AlignRight);
    grid2->addWidget( cbx23,    2, 1, Qt::AlignLeft);

    grid2->addWidget( lbl24,    3, 0, Qt::AlignRight);
    grid2->addWidget( cbx24,    3, 1, Qt::AlignLeft);

    grid2->addWidget( lbl25,    4, 0, Qt::AlignRight);
    grid2->addWidget( cbx25,    4, 1, Qt::AlignLeft);

    grid2->addWidget( lbl26,    5, 0, Qt::AlignRight);
    grid2->addWidget( cbx26,    5, 1, Qt::AlignLeft);

    //Bank select action
    auto vb3    = new QVBoxLayout();
    auto grid3  = new QGridLayout();

    gb3->setLayout( vb3 );
    vb3->addLayout( grid3 );
    vb3->addStretch(1000);

    auto lbl30 = new QLabel(" ");

    auto chb31  = new QCheckBox(tr("Send PC"));
    auto lbl31  = new QLabel(tr("Preset #"));
    auto sb31   = new QSpinBox(); sb31->setRange(1, 128); sb31->setWrapping(true);

    connect(chb31, QOverload<int>::of(&QCheckBox::stateChanged), [=](int state){
        qCDebug(CMN) << Q_FUNC_INFO << "state=" << state << "m_proceed=" << m_proceed;
        if ( m_proceed ){
            m_proceed = false;
            if ( Qt::Unchecked == state ){
                SSXMSGS::g_BanksSettings[m_curBnkNumber].selectBankAction = 0;
            }
            else if ( Qt::Checked == state ){
                SSXMSGS::g_BanksSettings[m_curBnkNumber].selectBankAction = 1;
            }
            emit s_changed(m_curBnkNumber);
            m_proceed = true;
        }
    });
    connect(sb31, QOverload<int>::of(&QSpinBox::valueChanged),[=](int value){
        if ( m_proceed ){
            SSXMSGS::g_BanksSettings[m_curBnkNumber].selectBankActionProgNum = static_cast<uint8_t>(value-1);
            emit s_changed(m_curBnkNumber);
        }
    });


    grid3->addWidget( lbl30,    0, 0, Qt::AlignRight);
    grid3->addWidget( lbl30,    0, 1, Qt::AlignRight);

    grid3->addWidget( lbl31,    0, 2, Qt::AlignRight);
    grid3->addWidget( sb31,     0, 3, Qt::AlignLeft);
    grid3->addWidget( chb31,    0, 4, Qt::AlignLeft);
/*
    m_spins  << sb11  << sb12  << sb13  << sb14  << sb15  << sb16;
    m_checks << chb14 << chb15 << chb16;
*/
    m_spins     << sb31;
    m_checks    << chb31;

    updateData( 0 ); // set up first bank by default
}

QCmnSetupWidget::~QCmnSetupWidget(){
    qCDebug(CMN) << Q_FUNC_INFO;
}

QCmnSetupWidget * QCmnSetupWidget::getInstance(){
    return(g_cmnsw);
}

void QCmnSetupWidget::updateData( int curBnkNumber ){
    qCDebug(CMN) << Q_FUNC_INFO << "curBnkNumber=" << curBnkNumber;
    if ( curBnkNumber < 0 ) {
        qCDebug(CMN) << Q_FUNC_INFO << "bad bank number!";
    }
    m_proceed = false;

    m_curBnkNumber = curBnkNumber;

    //Exp. pedal 1 CC#
    // sb11
    // BankSettings.pedalsCc[0];
    auto sb11 = m_spins.at(0);
    uint8_t data = SSXMSGS::g_BanksSettings.at(m_curBnkNumber).pedalsCc[0];
    if ( data > 127 ) {
        data = 0;
        SSXMSGS::g_BanksSettings[m_curBnkNumber].pedalsCc[0] = 0;
    }
    sb11->setValue(data);

    //Exp. pedal 2 CC#
    // sb12
    // BankSettings.pedalsCc[1];
    auto sb12 = m_spins.at(1);
    data = SSXMSGS::g_BanksSettings.at(m_curBnkNumber).pedalsCc[1];
    if ( data > 127 ) {
        data = 0;
        SSXMSGS::g_BanksSettings[m_curBnkNumber].pedalsCc[1] = 0;
    }
    sb12->setValue(data);

    //Onboard exp. pedal CC#
    //lb13 sb13
    //BankSettings.pedalsCc[2];
    auto lb13 = m_labels.at(2);
    auto sb13 = m_spins.at(2);
    data = SSXMSGS::g_BanksSettings.at(m_curBnkNumber).pedalsCc[2];
    if ( data > 127 ) {
        data = 0;
        SSXMSGS::g_BanksSettings[m_curBnkNumber].pedalsCc[2] = 0;
        //emit s_changed(m_curBnkNumber);
    }
    sb13->setValue(data);
    // для моделей без педали запрещены
    lb13->setDisabled( SSXMSGS::g_Model < 100 );
    sb13->setDisabled( SSXMSGS::g_Model < 100 );

    // lbl14  = new QLabel(tr("Onboard exp. pedal alt. CC#"));
    // sb14 chb14
    // BankSettings.pedalsCc[3];
    auto lbl14  = m_labels.at(3);
    auto sb14   = m_spins.at(3);
    auto chb14  = m_checks.at(0);

    // для моделей без педали запрещены
    lbl14->setDisabled( SSXMSGS::g_Model < 100 );
    sb14->setDisabled( SSXMSGS::g_Model < 100 );
    chb14->setDisabled( SSXMSGS::g_Model < 100 );

    data = SSXMSGS::g_BanksSettings.at(m_curBnkNumber).pedalsCc[3];
    if ( data > 127 ){
        SSXMSGS::g_BanksSettings[m_curBnkNumber].pedalsCc[3] = 128;
        sb14->setValue(0);
        if ( SSXMSGS::g_Model > 100 ) {
            lbl14->setDisabled(true);
            sb14->setDisabled(true);
        }
        chb14->setCheckState(Qt::Checked);
    }
    else{
        sb14->setValue(data);
        if ( SSXMSGS::g_Model > 100 ){
            lbl14->setEnabled(true);
            sb14->setEnabled(true);
        }
        chb14->setCheckState(Qt::Unchecked);
    }


    // auto lbl15  = new QLabel(tr("Tuner CC#"));
    // sb15 chb15
    // BankSettings.tunerCc
    auto lbl15  = m_labels.at(4);
    auto sb15   = m_spins.at(4);
    auto chb15  = m_checks.at(1);
    data = SSXMSGS::g_BanksSettings.at(m_curBnkNumber).tunerCc;
    if ( data > 127 ){
        SSXMSGS::g_BanksSettings[m_curBnkNumber].tunerCc = 128;
        lbl15->setDisabled(true);
        sb15->setValue(0); sb15->setDisabled(true);
        chb15->setCheckState(Qt::Checked);
    }
    else{
        lbl15->setEnabled(true);
        sb15->setValue(data); sb15->setEnabled(true);
        chb15->setCheckState(Qt::Unchecked);
    }

    // auto lbl16  = new QLabel(tr("Tap CC#"));
    // sb16  chb16
    // BankSettings.tapCc
    auto lbl16  = m_labels.at(5);
    auto sb16   = m_spins.at(5);
    auto chb16  = m_checks.at(2);
    data = SSXMSGS::g_BanksSettings.at(m_curBnkNumber).tapCc;
    if ( data > 127 ){
        SSXMSGS::g_BanksSettings[m_curBnkNumber].tapCc = 128;
        lbl16->setDisabled(true);
        sb16->setValue(0); sb16->setDisabled(true);
        chb16->setCheckState(Qt::Checked);
    }
    else{
        lbl16->setEnabled(true);
        sb16->setValue(data); sb16->setEnabled(true);
        chb16->setCheckState(Qt::Unchecked);
    }

    // Exp. pedal 1 type;
    // cbx21
    // GlobalSettings.expPtype[0]
    auto cbx21 = m_combos.at(0);
    data = SSXMSGS::g_GlobalSettings.expPtype[0];
    int id = cbx21->findData(QVariant(data));
    if ( id < 0 ){
        cbx21->setCurrentIndex(0);
        QVariant var = cbx21->itemData(0);
        int data;
        bool ok;
        data = var.toInt(&ok);
        if ( ok ){
            SSXMSGS::g_GlobalSettings.expPtype[0] = static_cast<SSXMSGS::ExpPedalType>(data);
        }
        else
            qCCritical(CMN) << Q_FUNC_INFO << "can't convert data for 'Exp. pedal 1 type'";
    }
    else
        cbx21->setCurrentIndex(id);

    // Exp. pedal 2 type
    // cbx22
    // GlobalSettings.expPtype[1]
    auto cbx22 = m_combos.at(1);
    data = SSXMSGS::g_GlobalSettings.expPtype[1];
    id = cbx22->findData(QVariant(data));
    if ( id < 0 ){
        cbx22->setCurrentIndex(0);
        QVariant var = cbx22->itemData(0);
        int data;
        bool ok;
        data = var.toInt(&ok);
        if ( ok ){
            SSXMSGS::g_GlobalSettings.expPtype[1] = static_cast<SSXMSGS::ExpPedalType>(data);
        }
        else
            qCCritical(CMN) << Q_FUNC_INFO << "can't convert data for 'Exp. pedal 2 type'";
    }
    else
        cbx22->setCurrentIndex(id);

    // Onboard pedal LEDs view
    // cbx23
    // GlobalSettings.pedalLedView
    auto lbl23 = m_labels.at(8);
    auto cbx23 = m_combos.at(2);
    data = SSXMSGS::g_GlobalSettings.pedalLedView;
    id = cbx23->findData(QVariant(data));
    if ( id < 0 ){
        cbx23->setCurrentIndex(0);
        QVariant var = cbx23->itemData(0);
        int data;
        bool ok;
        data = var.toInt(&ok);
        if ( ok ){
            SSXMSGS::g_GlobalSettings.pedalLedView = static_cast<SSXMSGS::PedalLedView>(data);
            //emit s_changed(-1);
        }
        else
            qCCritical(CMN) << Q_FUNC_INFO << "can't convert data for 'Onboard pedal LEDs view'";
    }
    else
        cbx23->setCurrentIndex(id);

    // для модели без педали запрещены
    lbl23->setDisabled( SSXMSGS::g_Model < 100 );
    cbx23->setDisabled( SSXMSGS::g_Model < 100 );

    // Onboard pedal tuner type
    // cbx24
    // SSXMSGS::PedalTunerScheme
    auto lbl24 = m_labels.at(9);
    auto cbx24 = m_combos.at(3);
    data = SSXMSGS::g_GlobalSettings.pedalTunerScheme;
    id = cbx24->findData(QVariant(data));
    if ( id < 0 ){
        cbx24->setCurrentIndex(0);
        QVariant var = cbx24->itemData(0);
        int data;
        bool ok;
        data = var.toInt(&ok);
        if ( ok ){
            SSXMSGS::g_GlobalSettings.pedalTunerScheme = static_cast<SSXMSGS::PedalTunerScheme>(data);
            //emit s_changed(-1);
        }
        else
            qCCritical(CMN) << Q_FUNC_INFO << "can't convert data for 'Onboard pedal tuner type'";
    }
    else
        cbx24->setCurrentIndex(id);
    // для модели без педали запрещены
    lbl24->setDisabled( SSXMSGS::g_Model < 100 );
    cbx24->setDisabled( SSXMSGS::g_Model < 100 );

    // Tap message type
    // cbx25
    // SSXMSGS::TapType
    auto cbx25 = m_combos.at(4);
    data = SSXMSGS::g_GlobalSettings.tapType;
    id = cbx25->findData(QVariant(data));
    if ( id < 0 ){
        cbx25->setCurrentIndex(0);
        QVariant var = cbx25->itemData(0);
        int data;
        bool ok;
        data = var.toInt(&ok);
        if ( ok ){
            SSXMSGS::g_GlobalSettings.tapType = static_cast<SSXMSGS::TapType>(data);
            //emit s_changed(-1);
        }
        else
            qCCritical(CMN) << Q_FUNC_INFO << "can't convert data for 'Tap message type'";
    }
    else
        cbx25->setCurrentIndex(id);

    // Tap display type
    // cbx26
    // SSXMSGS::TapDisplayType
    auto cbx26 = m_combos.at(5);
    data = SSXMSGS::g_GlobalSettings.tapDisplayType;
    id = cbx26->findData(QVariant(data));
    if ( id < 0 ){
        cbx26->setCurrentIndex(0);
        QVariant var = cbx26->itemData(0);
        int data;
        bool ok;
        data = var.toInt(&ok);
        if ( ok ){
            SSXMSGS::g_GlobalSettings.tapDisplayType = static_cast<SSXMSGS::TapDisplayType>(data);
            //emit s_changed(-1);
        }
        else
            qCCritical(CMN) << Q_FUNC_INFO << "can't convert data for 'Tap display type'";
    }
    else
        cbx26->setCurrentIndex(id);

    // auto chb31  = new QCheckBox(tr("Send PC"));
    // auto lbl31  = new QLabel(tr("Preset #"));
    // auto sb31   = new QSpinBox(); sb31->setRange(1, 128); sb31->setWrapping(true);

    auto chb31  = m_checks.at(3);
    auto sb31   = m_spins.at(6);

    if ( SSXMSGS::g_BanksSettings[m_curBnkNumber].selectBankAction > 0){
        chb31->setChecked(true);
        SSXMSGS::g_BanksSettings[m_curBnkNumber].selectBankAction = 1;
    }
    else {
        chb31->setChecked(false);
    }

    if ( SSXMSGS::g_BanksSettings[m_curBnkNumber].selectBankActionProgNum > 127)
        SSXMSGS::g_BanksSettings[m_curBnkNumber].selectBankActionProgNum = 127;
    sb31->setValue( SSXMSGS::g_BanksSettings[m_curBnkNumber].selectBankActionProgNum + 1 );

    m_proceed = true;
}

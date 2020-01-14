#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QGridLayout>
#include <QLabel>
#include <QSpinBox>
#include <QComboBox>
#include "qsyssetupwidget.h"
#include "qmidi/qmidisysexmsgs.h"
Q_LOGGING_CATEGORY( SYS,"SYS")

static QSysSetupWidget * g_ssw{Q_NULLPTR};

QSysSetupWidget::QSysSetupWidget(QWidget *parent) : QWidget(parent){
    qCDebug(SYS) << Q_FUNC_INFO;

    g_ssw = this;

    auto mainLayout = new QVBoxLayout();
    setLayout( mainLayout );

    auto hbox   = new QHBoxLayout();
    mainLayout->addLayout(hbox);
    mainLayout->addStretch(1000);

    auto gbox   = new QGroupBox(tr("Settings"));
    hbox->addWidget(gbox);

    auto grid   = new QGridLayout();
    gbox->setLayout(grid);

    // GlobalSettings.midiChanNum 0..15 (screen 1..16)
    auto lbl01  = new QLabel(tr("Primary MIDI channel"));
    auto sbx01  = new QSpinBox(); sbx01->setRange( 1, 16 ); sbx01->setWrapping(true);
    connect(sbx01, QOverload<int>::of(&QSpinBox::valueChanged),[=](int value){
        if ( m_proceed ){
            SSXMSGS::g_GlobalSettings.midiChanNum = static_cast<uint8_t>(value-1);
            emit s_changed(-1);
        }
    });

    // GlobalSettings.useBankSelectMess(enum)
    auto lbl02  = new QLabel(tr("Program Change Mode"));
    auto cbx02  = new QComboBox(); cbx02->setMinimumContentsLength(20);
    cbx02->addItem(tr("Use Bank Select"),       QVariant(SSXMSGS::UseBankSelectMess::USE_BANK_SELECT));
    cbx02->addItem(tr("Dont Use Bank Select"),  QVariant(SSXMSGS::UseBankSelectMess::DONT_USE_BANK_SELECT));
    connect(cbx02, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index){
        if (m_proceed){
            QVariant var = cbx02->itemData(index);
            bool ok;
            int data = var.toInt(&ok);
            if ( ok ){
                SSXMSGS::g_GlobalSettings.useBankSelectMess = static_cast<SSXMSGS::UseBankSelectMess>(data);
                emit s_changed(-1);
            }
            else
                qCCritical(SYS) << Q_FUNC_INFO << "can't convert data for 'Program Change Mode'";
        }
    });

    // GlobalSettings.bankSelectMessType(enum)
    auto lbl03  = new QLabel(tr("Program Change Mode"));
    auto cbx03  = new QComboBox(); cbx03->setMinimumContentsLength(20);
    cbx03->addItem(tr("MSB"),                   QVariant(SSXMSGS::BankSelectMessType::MSB));
    cbx03->addItem(tr("LSB"),                   QVariant(SSXMSGS::BankSelectMessType::LSB));
    cbx03->addItem(tr("MSB and LSB"),           QVariant(SSXMSGS::BankSelectMessType::MSB_AND_LSB));
    connect(cbx03, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index){
        if (m_proceed){
            QVariant var = cbx03->itemData(index);
            bool ok;
            int data = var.toInt(&ok);
            if ( ok ){
                SSXMSGS::g_GlobalSettings.bankSelectMessType = static_cast<SSXMSGS::BankSelectMessType>(data);
                emit s_changed(-1);
            }
            else
                qCCritical(SYS) << Q_FUNC_INFO << "can't convert data for 'Program Change Mode'";
        }
    });

    // GlobalSettings.bnkSwOnBoard(enum)
    auto lbl04  = new QLabel(tr("BS-2 pedal"));
    auto cbx04  = new QComboBox(); cbx04->setMinimumContentsLength(20);
    cbx04->addItem(tr("Not used"),              QVariant(SSXMSGS::ExternalBs2Pedal::NO_EXT_PEDAL));
    cbx04->addItem(tr("Switch banks"),          QVariant(SSXMSGS::ExternalBs2Pedal::EXT_PEDAL_BANK_SWITCH));
    cbx04->addItem(tr("Switch presets"),        QVariant(SSXMSGS::ExternalBs2Pedal::EXT_PEDAL_PRESET_SWITCH));
    connect(cbx04, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index){
        if (m_proceed){
            QVariant var = cbx04->itemData(index);
            bool ok;
            int data = var.toInt(&ok);
            if ( ok ){
                SSXMSGS::g_GlobalSettings.bnkSwOnBoard = static_cast<SSXMSGS::ExternalBs2Pedal>(data);
                emit s_changed(-1);
            }
            else
                qCCritical(SYS) << Q_FUNC_INFO << "can't convert data for 'BS-2 pedal'";
        }
    });

    // GlobalSettings.maxBankNumber 0..127 (screen 1..128)
    auto lbl05  = new QLabel(tr("Limit bank number: up & down"));
    auto sbx05  = new QSpinBox(); sbx05->setRange( 1, 128 ); sbx05->setWrapping(true);
    connect(sbx05, QOverload<int>::of(&QSpinBox::valueChanged),[=](int value){
        if ( m_proceed ){
            SSXMSGS::g_GlobalSettings.maxBankNumber = static_cast<uint8_t>(value-1);
        }
    });

    // GlobalSettings.Show_pr_name(enum)
    auto lbl06  = new QLabel(tr("Name display"));
    auto cbx06  = new QComboBox(); cbx06->setMinimumContentsLength(20);
    cbx06->addItem(tr("Only Preset"),           QVariant(SSXMSGS::ShowPresetBank::ONLY_PRESET));
    cbx06->addItem(tr("Only Bank"),             QVariant(SSXMSGS::ShowPresetBank::ONLY_BANK));
    cbx06->addItem(tr("Preset and Bank"),       QVariant(SSXMSGS::ShowPresetBank::PRESET_AND_BANK));
    connect(cbx06, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index){
        if (m_proceed){
            QVariant var = cbx06->itemData(index);
            bool ok;
            int data = var.toInt(&ok);
            if ( ok ){
                SSXMSGS::g_GlobalSettings.Show_pr_name = static_cast<SSXMSGS::ShowPresetBank>(data);
                emit s_changed(-1);
            }
            else
                qCCritical(SYS) << Q_FUNC_INFO << "can't convert data for 'Name display'";
        }
    });

    // GlobalSettings.targetDevice(enum)
    auto lbl07  = new QLabel(tr("Target device"));
    auto cbx07  = new QComboBox(); cbx07->setMinimumContentsLength(20);
    cbx07->addItem(tr("Auto"),                  QVariant(SSXMSGS::TargetDevice::TARGET_DEVICE_AUTO));
    cbx07->addItem(tr("Off"),                   QVariant(SSXMSGS::TargetDevice::TARGET_DEVICE_OFF));
    cbx07->addItem(tr("Axe ST"),                QVariant(SSXMSGS::TargetDevice::TARGET_DEVICE_AXE_ST));
    cbx07->addItem(tr("Axe Ultra"),             QVariant(SSXMSGS::TargetDevice::TARGET_DEVICE_AXE_ULTRA));
    cbx07->addItem(tr("Ax8"),                   QVariant(SSXMSGS::TargetDevice::TARGET_DEVICE_AX8));
    cbx07->addItem(tr("Axe II"),                QVariant(SSXMSGS::TargetDevice::TARGET_DEVICE_AXEII));
    cbx07->addItem(tr("Axe II XL"),             QVariant(SSXMSGS::TargetDevice::TARGET_DEVICE_AXEII_XL));
    cbx07->addItem(tr("Axe II XL Plus"),        QVariant(SSXMSGS::TargetDevice::TARGET_DEVICE_AXEII_XL_PLUS));
    cbx07->addItem(tr("Axe III"),               QVariant(SSXMSGS::TargetDevice::TARGET_DEVICE_AXEIII));
    cbx07->addItem(tr("Kpa"),                   QVariant(SSXMSGS::TargetDevice::TARGET_DEVICE_KPA));
    cbx07->addItem(tr("G-Major"),               QVariant(SSXMSGS::TargetDevice::TARGET_DEVICE_G_MAJOR));
    connect(cbx07, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index){
        if (m_proceed){
            QVariant var = cbx07->itemData(index);
            bool ok;
            int data = var.toInt(&ok);
            if ( ok ){
                SSXMSGS::g_GlobalSettings.targetDevice = static_cast<SSXMSGS::TargetDevice>(data);
                emit s_changed(-1);
            }
            else
                qCCritical(SYS) << Q_FUNC_INFO << "can't convert data for 'Target device'";
        }
    });

    // GlobalSettings.usbBaudrate(enum)
    auto lbl08  = new QLabel(tr("USB baudrate"));
    auto cbx08  = new QComboBox(); cbx08->setMinimumContentsLength(20);
    cbx08->addItem(tr("MIDI Baud"),             QVariant(SSXMSGS::UsbBaudrate::MIDI_BAUD));
    cbx08->addItem(tr("Custom Baud 57600"),     QVariant(SSXMSGS::UsbBaudrate::CUSTOM_BAUD_57600));
    connect(cbx08, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index){
        if (m_proceed){
            QVariant var = cbx08->itemData(index);
            bool ok;
            int data = var.toInt(&ok);
            if ( ok ){
                SSXMSGS::g_GlobalSettings.usbBaudrate = static_cast<SSXMSGS::UsbBaudrate>(data);
                emit s_changed(-1);
            }
            else
                qCCritical(SYS) << Q_FUNC_INFO << "can't convert data for 'USB baudrate'";
        }
    });

    // GlobalSettings.inputThrough[0](enum)
    auto lbl09  = new QLabel(tr("Pass MIDI IN to"));
    auto cbx09  = new QComboBox(); cbx09->setMinimumContentsLength(20);
    cbx09->addItem(tr("IN to None"),            QVariant(SSXMSGS::InputThrough::IN_TO_NONE));
    cbx09->addItem(tr("IN to MIDI OUT"),        QVariant(SSXMSGS::InputThrough::IN_TO_MIDI_OUT));
    cbx09->addItem(tr("IN to USB OUT"),         QVariant(SSXMSGS::InputThrough::IN_TO_USB_OUT));
    cbx09->addItem(tr("IN to MIDI and USB OUT"),QVariant(SSXMSGS::InputThrough::IN_TO_MIDI_AND_USB_OUT));
    connect(cbx09, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index){
        if (m_proceed){
            QVariant var = cbx09->itemData(index);
            bool ok;
            int data = var.toInt(&ok);
            if ( ok ){
                SSXMSGS::g_GlobalSettings.inputThrough[0] = static_cast<SSXMSGS::InputThrough>(data);
                emit s_changed(-1);
            }
            else
                qCCritical(SYS) << Q_FUNC_INFO << "can't convert data for 'Pass MIDI IN to'";
        }
    });

    // GlobalSettings.inputThrough[1](enum)
    auto lbl10  = new QLabel(tr("Pass USB IN to"));
    auto cbx10  = new QComboBox(); cbx10->setMinimumContentsLength(20);
    cbx10->addItem(tr("IN to None"),            QVariant(SSXMSGS::InputThrough::IN_TO_NONE));
    cbx10->addItem(tr("IN to MIDI OUT"),        QVariant(SSXMSGS::InputThrough::IN_TO_MIDI_OUT));
    cbx10->addItem(tr("IN to USB OUT"),         QVariant(SSXMSGS::InputThrough::IN_TO_USB_OUT));
    cbx10->addItem(tr("IN to MIDI and USB OUT"),QVariant(SSXMSGS::InputThrough::IN_TO_MIDI_AND_USB_OUT));
    connect(cbx10, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index){
        if (m_proceed){
            QVariant var = cbx10->itemData(index);
            bool ok;
            int data = var.toInt(&ok);
            if ( ok ){
                SSXMSGS::g_GlobalSettings.inputThrough[1] = static_cast<SSXMSGS::InputThrough>(data);
                emit s_changed(-1);
            }
            else
                qCCritical(SYS) << Q_FUNC_INFO << "can't convert data for 'Pass USB IN to'";
        }
    });

    grid->addWidget( lbl01,     0, 0, Qt::AlignRight);
    grid->addWidget( sbx01,     0, 1, Qt::AlignLeft);

    grid->addWidget( lbl02,     1, 0, Qt::AlignRight);
    grid->addWidget( cbx02,     1, 1, Qt::AlignLeft);

    grid->addWidget( lbl03,     2, 0, Qt::AlignRight);
    grid->addWidget( cbx03,     2, 1, Qt::AlignLeft);

    grid->addWidget( lbl04,     3, 0, Qt::AlignRight);
    grid->addWidget( cbx04,     3, 1, Qt::AlignLeft);

    grid->addWidget( lbl05,     4, 0, Qt::AlignRight);
    grid->addWidget( sbx05,     4, 1, Qt::AlignLeft);
    //-----------------------------------------------
    grid->addWidget( lbl06,     5, 0, Qt::AlignRight);
    grid->addWidget( cbx06,     5, 1, Qt::AlignLeft);

    grid->addWidget( lbl07,     6, 0, Qt::AlignRight);
    grid->addWidget( cbx07,     6, 1, Qt::AlignLeft);

    grid->addWidget( lbl08,     7, 0, Qt::AlignRight);
    grid->addWidget( cbx08,     7, 1, Qt::AlignLeft);

    grid->addWidget( lbl09,     8, 0, Qt::AlignRight);
    grid->addWidget( cbx09,     8, 1, Qt::AlignLeft);

    grid->addWidget( lbl10,     9, 0, Qt::AlignRight);
    grid->addWidget( cbx10,     9, 1, Qt::AlignLeft);

    m_widgets << sbx01 << cbx02 << cbx03 << cbx04 << sbx05 << cbx06 << cbx07 << cbx08 << cbx09 << cbx10;

    updateData();
}

QSysSetupWidget::~QSysSetupWidget(){
    qCDebug(SYS) << Q_FUNC_INFO;
}

QSysSetupWidget * QSysSetupWidget::getInstance(){
    return (g_ssw);
}

void QSysSetupWidget::updateData(){
    qCDebug(SYS) << Q_FUNC_INFO;

    m_proceed = false;

    // GlobalSettings.midiChanNum 0..15 (screen 1..16)
    auto sbx01 = static_cast<QSpinBox*>(m_widgets.at(0));
    uint8_t data = SSXMSGS::g_GlobalSettings.midiChanNum;
    if ( data > 15 ){
        data = 0;
        SSXMSGS::g_GlobalSettings.midiChanNum = 0;
    }
    sbx01->setValue(data+1);

    // GlobalSettings.useBankSelectMess(enum)
    auto cbx02 = static_cast<QComboBox*>(m_widgets.at(1));
    data = SSXMSGS::g_GlobalSettings.useBankSelectMess;
    int index = cbx02->findData(QVariant(data));
    if ( index < 0 )
        cbx02->setCurrentIndex(0);
    else
        cbx02->setCurrentIndex(index);

    // GlobalSettings.bankSelectMessType(enum)
    auto cbx03 = static_cast<QComboBox*>(m_widgets.at(2));
    data = SSXMSGS::g_GlobalSettings.bankSelectMessType;
    index = cbx03->findData(QVariant(data));
    if ( index < 0 )
        cbx03->setCurrentIndex(0);
    else
        cbx03->setCurrentIndex(index);

    // GlobalSettings.bnkSwOnBoard(enum)
    auto cbx04 = static_cast<QComboBox*>(m_widgets.at(3));
    data = SSXMSGS::g_GlobalSettings.bnkSwOnBoard;
    index = cbx04->findData(QVariant(data));
    if ( index < 0 )
        cbx04->setCurrentIndex(0);
    else
        cbx04->setCurrentIndex(index);

    // GlobalSettings.maxBankNumber 0..127 (screen 1..128)
    auto sbx05 = static_cast<QSpinBox*>(m_widgets.at(4));
    data = SSXMSGS::g_GlobalSettings.maxBankNumber;
    if ( data > 127 ){
        data = 0;
        SSXMSGS::g_GlobalSettings.maxBankNumber = 0;
    }
    sbx05->setValue(data+1);

    // GlobalSettings.Show_pr_name(enum)
    auto cbx06 = static_cast<QComboBox*>(m_widgets.at(5));
    data = SSXMSGS::g_GlobalSettings.Show_pr_name;
    index = cbx06->findData(QVariant(data));
    if ( index < 0 )
        cbx06->setCurrentIndex(0);
    else
        cbx06->setCurrentIndex(index);

    // GlobalSettings.targetDevice(enum)
    auto cbx07 = static_cast<QComboBox*>(m_widgets.at(6));
    data = SSXMSGS::g_GlobalSettings.targetDevice;
    index = cbx07->findData(QVariant(data));
    if ( index < 0 )
        cbx07->setCurrentIndex(0);
    else
        cbx07->setCurrentIndex(index);

    // GlobalSettings.usbBaudrate(enum)
    auto cbx08 = static_cast<QComboBox*>(m_widgets.at(7));
    data = SSXMSGS::g_GlobalSettings.usbBaudrate;
    index = cbx08->findData(QVariant(data));
    if ( index < 0 )
        cbx08->setCurrentIndex(0);
    else
        cbx08->setCurrentIndex(index);

    // GlobalSettings.inputThrough[0](enum)
    auto cbx09 = static_cast<QComboBox*>(m_widgets.at(8));
    data = SSXMSGS::g_GlobalSettings.inputThrough[0];
    index = cbx09->findData(QVariant(data));
    if ( index < 0 )
        cbx09->setCurrentIndex(0);
    else
        cbx09->setCurrentIndex(index);

    // GlobalSettings.inputThrough[1](enum)
    auto cbx10 = static_cast<QComboBox*>(m_widgets.at(9));
    data = SSXMSGS::g_GlobalSettings.inputThrough[1];
    index = cbx10->findData(QVariant(data));
    if ( index < 0 )
        cbx10->setCurrentIndex(0);
    else
        cbx10->setCurrentIndex(index);

    m_proceed = true;
}

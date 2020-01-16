#include <QVBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QComboBox>
#include <QStackedLayout>
#include <QPushButton>
#include <QVariant>

#include "qhomewidget.h"
#include "qbtnsetupwidget.h"
#include "qbuttonswidget.h"

#include "qpresetchangewidget.h"
#include "qcctwidget.h"
#include "qcccwidget.h"
#include "qccmwidget.h"
#include "qnrpntwidget.h"
#include "qnrpncwidget.h"
#include "qnotewidget.h"
#include "qbank2widget.h"
#include "qmidi/qmidisysexmsgs.h"

Q_LOGGING_CATEGORY( BSW, "BSW" )

// object names
static const QString _sl            = QString("BswStackedLayout");
static const QString _bsGroupBox    = QString("BswGroupBox");
static const QString _typeCombo     = QString("BswComboBox");

QBtnSetupWidget::QBtnSetupWidget(QWidget *parent) : QWidget(parent){
    qCDebug(BSW) << Q_FUNC_INFO;

    auto mainLayout = new QVBoxLayout();
    setLayout( mainLayout );

    auto bsGroupBox = new QGroupBox(tr("Button 1 settings"));
    bsGroupBox->setObjectName( _bsGroupBox );
    mainLayout->addWidget(bsGroupBox);

    auto VBoxLayout = new QVBoxLayout();
    bsGroupBox->setLayout(VBoxLayout);

    // layout for button type
    auto hb1 = new QHBoxLayout();
    VBoxLayout->addLayout(hb1);

    auto typeLabel = new QLabel(tr("Type:"));
    auto typeCombo = new QComboBox();
    typeCombo->setObjectName(_typeCombo);

    hb1->addWidget(typeLabel, 0, Qt::AlignLeft);
    hb1->addWidget(typeCombo, 0, Qt::AlignLeft);
    hb1->addStretch(1000);

    typeCombo->addItem(tr("Preset Change"),                 QVariant(SSXMSGS::ButtonType::PRESET_CHANGE));
    typeCombo->addItem(tr("Control Change Toggle"),         QVariant(SSXMSGS::ButtonType::CC_TOGGLE));
    typeCombo->addItem(tr("Control Change Momentary"),      QVariant(SSXMSGS::ButtonType::CC_MOMENTARY));
    typeCombo->addItem(tr("Control Change Constant Value"), QVariant(SSXMSGS::ButtonType::CC_CONST_VAL));
    typeCombo->addItem(tr("NRPN Toggle"),                   QVariant(SSXMSGS::ButtonType::NRPN_TOGGLE));
    typeCombo->addItem(tr("NRPN Momentary"),                QVariant(SSXMSGS::ButtonType::NRPN_MOMENTARY));
    typeCombo->addItem(tr("NRPN Constant Value"),           QVariant(SSXMSGS::ButtonType::NRPN_CONST_VAL));
    typeCombo->addItem(tr("Note On"),                       QVariant(SSXMSGS::ButtonType::NOTE_ON));
    typeCombo->addItem(tr("Note Off"),                      QVariant(SSXMSGS::ButtonType::NOTE_OFF));
    typeCombo->addItem(tr("Preset Up"),                     QVariant(SSXMSGS::ButtonType::PRESET_UP));
    typeCombo->addItem(tr("Preset Down"),                   QVariant(SSXMSGS::ButtonType::PRESET_DOWN));
    typeCombo->addItem(tr("Bank Up"),                       QVariant(SSXMSGS::ButtonType::BANK_UP));
    typeCombo->addItem(tr("Bank Down"),                     QVariant(SSXMSGS::ButtonType::BANK_DOWN));
    typeCombo->addItem(tr("Bank To"),                       QVariant(SSXMSGS::ButtonType::BANK_TO));

    // layout for button settings forms
    auto sl = new QStackedLayout();
    sl->setObjectName(_sl);

    VBoxLayout->addLayout(sl);
    VBoxLayout->addStretch(2000);

    auto pcw        = new QPresetChangeWidget();
    auto cct        = new QCctWidget();
    auto ccm        = new QCcmWidget();
    auto ccc        = new QCccWidget();
    auto nrpnt      = new QNrpnTWidget();
    auto nrpnm      = new QNrpnTWidget( false );
    auto nrpnc      = new QNrpnCWidget();
    auto noteOn     = new QNoteWidget();
    auto noteOff    = new QNoteWidget( false );
    auto blank1     = new QWidget();
    auto blank2     = new QWidget();
    auto blank3     = new QWidget();
    auto blank4     = new QWidget();
    auto bank2      = new QBank2Widget();

    sl->addWidget( pcw );
    sl->addWidget( cct );
    sl->addWidget( ccm );
    sl->addWidget( ccc );
    sl->addWidget( nrpnt );
    sl->addWidget( nrpnm );
    sl->addWidget( nrpnc );
    sl->addWidget( noteOn );
    sl->addWidget( noteOff );
    sl->addWidget( blank1 );
    sl->addWidget( blank2 );
    sl->addWidget( blank3 );
    sl->addWidget( blank4 );
    sl->addWidget( bank2 );

    connect( pcw, &QPresetChangeWidget::s_changed, this , &QBtnSetupWidget::_changed);

    // layout for button controls
    auto hb2 = new QHBoxLayout();
    VBoxLayout->addLayout(hb2);

    auto backBtn = new QPushButton(tr("<< Back"));
    hb2->addWidget(backBtn,0,Qt::AlignLeft);

    connect(typeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index){
        qCDebug(BSW) << Q_FUNC_INFO << "index=" << index;

        sl->setCurrentIndex(index);

        QVariant variant = typeCombo->itemData( index );
        uint8_t buttonType = variant.value<uint8_t>();
        qCDebug(BSW) << Q_FUNC_INFO << "data=" << buttonType;
        SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonType[m_btnNumber-1] = static_cast<SSXMSGS::ButtonType>(buttonType);
        if (m_bProceed) emit s_changed(m_curBnkNumber);

        if ( buttonType == SSXMSGS::ButtonType::PRESET_CHANGE ){
            pcw->setButton( m_btnNumber, m_curBnkNumber);
        }
        else if (buttonType == SSXMSGS::ButtonType::CC_TOGGLE){
            cct->setButton( m_btnNumber, m_curBnkNumber);
        }
        else if (buttonType == SSXMSGS::ButtonType::CC_MOMENTARY){
            ccm->setButton( m_btnNumber, m_curBnkNumber);
        }
        else if (buttonType == SSXMSGS::ButtonType::CC_CONST_VAL){
            ccc->setButton( m_btnNumber, m_curBnkNumber);
        }
        else if (buttonType == SSXMSGS::ButtonType::NRPN_TOGGLE){
            nrpnt->setButton( m_btnNumber, m_curBnkNumber);
        }
        else if (buttonType == SSXMSGS::ButtonType::NRPN_MOMENTARY){
            nrpnm->setButton( m_btnNumber, m_curBnkNumber);
        }
        else if (buttonType == SSXMSGS::ButtonType::NRPN_CONST_VAL){
            nrpnc->setButton( m_btnNumber, m_curBnkNumber);
        }
        else if (buttonType == SSXMSGS::ButtonType::NOTE_ON){
            noteOn->setButton( m_btnNumber, m_curBnkNumber);
        }
        else if (buttonType == SSXMSGS::ButtonType::NOTE_OFF){
            noteOff->setButton( m_btnNumber, m_curBnkNumber);
        }
        else if (buttonType == SSXMSGS::ButtonType::BANK_TO){
            bank2->setButton( m_btnNumber, m_curBnkNumber);
        }

        //QButtonsWidget::getInstance()->update( m_btnNumber, m_curBnkNumber );
        QButtonsWidget::getInstance()->update( m_curBnkNumber );

    });

    connect(backBtn, &QPushButton::clicked,[=](){
        QButtonsWidget::getInstance()->update( m_curBnkNumber );
        emit s_back();
    });
    connect(this, &QBtnSetupWidget::s_changed, QHomeWidget::getInstance(), &QHomeWidget::_changed);
}

QBtnSetupWidget::~QBtnSetupWidget(){
    qCDebug(BSW) << Q_FUNC_INFO;
}

void QBtnSetupWidget::_changed( int number ){
    qCDebug(BSW) << Q_FUNC_INFO << "number=" << number;
}

void QBtnSetupWidget::setButton(int btnNumber, int curBnkNumber){
    qCDebug(BSW) << Q_FUNC_INFO << "btnNumber=" << btnNumber << "curBnkNumber=" << curBnkNumber;
    if ( curBnkNumber<0 ){
        qCCritical(BSW) << Q_FUNC_INFO << "got bad bank number!";
        return;
    }
    if (!(( btnNumber>=1 ) && ( btnNumber<=FOOT_BUTTONS_NUM ))){
        qCCritical(BSW) << Q_FUNC_INFO << "got bad button number!";
        return;
    }
    m_bProceed = false;

    m_curBnkNumber  = curBnkNumber;
    m_btnNumber     = btnNumber;

    auto bsGroupBox = findChild<QGroupBox*>(_bsGroupBox);
    auto typeCombo  = findChild<QComboBox*>(_typeCombo);

    if ( Q_NULLPTR == bsGroupBox ){
        qCCritical(BSW) << Q_FUNC_INFO << "can't find group box!!";
        return;
    }
    if ( Q_NULLPTR == typeCombo ){
        qCCritical(BSW) << Q_FUNC_INFO << "can't find combo box!!";
        return;
    }
    QString title = QString(tr("Button %1 settings")).arg(m_btnNumber);
    bsGroupBox->setTitle( title );
    // !!! set up Button Type !!!
    SSXMSGS::ButtonType buttonType = SSXMSGS::g_BanksSettings.at(m_curBnkNumber).buttonType[m_btnNumber-1];
    int index = typeCombo->findData(QVariant(buttonType));

    qCDebug(BSW) << Q_FUNC_INFO << "combo index=" << index;

    typeCombo->setCurrentIndex(-1);
    typeCombo->setCurrentIndex(index);

    m_bProceed = true;
}

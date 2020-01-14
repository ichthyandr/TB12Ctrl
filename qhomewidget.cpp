#include <QVBoxLayout>
#include <QTabWidget>
#include <QLabel>
#include <QComboBox>
#include <QGuiApplication>
#include <QCoreApplication>
#include <QGroupBox>
#include <QPushButton>
#include <QRegExp>
#include <QRegExpValidator>
#include "qhomewidget.h"
#include "qbuttonswidget.h"
#include "qsyssetupwidget.h"
#include "qcmnsetupwidget.h"
#include "qmidi/qmidisysexmsgs.h"
#include "dialog.h"
Q_LOGGING_CATEGORY( HOM, "HOM" )

// object names
static const QString _btnsWidget    = QString("HomQButtonsWidget");
static const QString _bankCombo     = QString("bankCombo");
static const QString _tabWidget     = QString("tabWidget");
static const QString _sendBankBtn   = QString("sendBankBtn");
static const QString _inPortCmb     = QString("inPortCmb");
static const QString _outPortCmb    = QString("outPortCmb");
static const QString _sendBtn       = QString("sendBtn");
static const QString _onBtn         = QString("onBtn");

static QHomeWidget * g_hw{Q_NULLPTR};

QHomeWidget::QHomeWidget(QWidget *parent) : QWidget(parent){
    qCDebug(HOM) << Q_FUNC_INFO;

    g_hw = this;

    auto mainLayout = new QVBoxLayout();
    setLayout( mainLayout );

    auto tabWidget = new QTabWidget();
    tabWidget->setObjectName(_tabWidget);

    auto btnsWidget     = new QButtonsWidget();
    auto sysSetupWidget = new QSysSetupWidget();
    auto cmnSetupWidget = new QCmnSetupWidget();

    connect( sysSetupWidget,    &QSysSetupWidget::s_changed,    this, &QHomeWidget::_changed );
    connect( cmnSetupWidget,    &QCmnSetupWidget::s_changed,    this, &QHomeWidget::_changed );

    btnsWidget->setObjectName(_btnsWidget);

    tabWidget->addTab( btnsWidget,      tr("Buttons"));
    tabWidget->addTab( sysSetupWidget,  tr("System setup"));
    tabWidget->addTab( cmnSetupWidget,  tr("Common Settings"));

    auto grid = new QGridLayout();
    mainLayout->addLayout(grid);

    auto banksGrBx  = new QGroupBox(tr("Select bank"));
    auto midiGrBx   = new QGroupBox(tr("MIDI ports"));

    grid->addWidget(banksGrBx, 0, 0);
    grid->addWidget(midiGrBx,  0, 1);

    /// banks
    auto bankLayout = new QGridLayout();
    banksGrBx->setLayout( bankLayout );

    auto bankLabel  = new QLabel(tr("Name:"));
    auto bankCombo  = new QComboBox();
    bankCombo->setMinimumContentsLength(/*27*/18);
    bankCombo->setEditable(true);
    bankCombo->setInsertPolicy(QComboBox::InsertAtCurrent);
    QRegExp rx("[a-zA-Z0-9\\s\\-_]{1,13}");
    auto validator = new QRegExpValidator(rx, this);
    bankCombo->setValidator(validator);

    auto sendBankBtn= new QPushButton(tr("Send Bank"));
    sendBankBtn->setObjectName(_sendBankBtn);
    sendBankBtn->setFocusPolicy(Qt::NoFocus);
    sendBankBtn->setDisabled(true);

    bankLayout->addWidget(bankLabel,    0, 0, Qt::AlignLeft );
    bankLayout->addWidget(bankCombo,    0, 1, Qt::AlignLeft );
    bankLayout->addWidget(sendBankBtn,  1, 1, Qt::AlignRight );

    bankCombo->setObjectName(_bankCombo);

    connect( bankCombo, QOverload<const QString &>::of(&QComboBox::editTextChanged), [=](const QString &text){
        int index = bankCombo->currentIndex();
        if (index >= 0){

            QString itemText= bankCombo->itemText( index );
            if ( text != itemText ){
                bankCombo->setItemText( index, text );
                m_cmdSet << (-1) << index;
                updTitle();
            }

            qCDebug(HOM) << Q_FUNC_INFO << "index=" << index << "text=" << text << "set=" << m_cmdSet;
            ::memset( SSXMSGS::g_BanksSettings[index].BankName, 0, BANK_NAME_NMAX_SIZE);
            ::memcpy_s( SSXMSGS::g_BanksSettings[index].BankName, BANK_NAME_NMAX_SIZE, text.toStdString().c_str(), text.toStdString().length());
        }
    });

    connect( bankCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index){
        qCDebug(HOM) << Q_FUNC_INFO << "index=" << index << "set=" << m_cmdSet;
        if (index >= 0){
            updTitle();
            Dialog::getInstance()->setCurrentBankNumber(index);

            cmnSetupWidget->updateData( index );
        }
    });

    /// midi ports
    auto grid_ = new QGridLayout();
    midiGrBx->setLayout(grid_);

    QStringList inPorts     = QIN::Controller::getPorts();
    QStringList outPorts    = QOUT::Controller::getPorts();

    auto inPortCmb          = new QComboBox();
    auto outPortCmb         = new QComboBox();

    inPortCmb->setMinimumContentsLength(31);
    outPortCmb->setMinimumContentsLength(31);

    auto inLabel            = new QLabel(tr("IN:"));
    auto outLabel           = new QLabel(tr("OUT:"));

    auto onBtn              = new QPushButton();
    onBtn->setIcon(QIcon(":/images/off_16.png"));
    onBtn->setToolTip(tr("Connect"));

    auto sendBtn            = new QPushButton(tr("Send All"));
    auto rxLabel            = new QLabel(tr("Rx"));
    auto txLabel            = new QLabel(tr("Tx"));

    inPortCmb->setObjectName(_inPortCmb);
    outPortCmb->setObjectName(_outPortCmb);
    sendBtn->setObjectName(_sendBtn);
    onBtn->setObjectName(_onBtn);

    rxLabel->setPixmap( m_rxIndicator.off() );
    txLabel->setPixmap( m_rxIndicator.off() );

    rxLabel->setFixedWidth(16);
    txLabel->setFixedWidth(16);

    grid_->addWidget( inLabel,   0, 0, Qt::AlignRight);
    grid_->addWidget( outLabel,  1, 0, Qt::AlignRight);
    grid_->addWidget( inPortCmb, 0, 1, Qt::AlignLeft);
    grid_->addWidget( outPortCmb,1, 1, Qt::AlignLeft);
    grid_->addWidget( rxLabel,   0, 2, Qt::AlignLeft);
    grid_->addWidget( txLabel,   1, 2, Qt::AlignLeft);
    grid_->addWidget( onBtn,     0, 3, Qt::AlignLeft);
    grid_->addWidget( sendBtn,   1, 3, Qt::AlignLeft);

    grid_->setColumnStretch(3, 100);

    inPortCmb->addItems(inPorts);
    outPortCmb->addItems(outPorts);

    sendBtn->setDisabled(true);

    if ( inPorts.isEmpty() || outPorts.isEmpty() ){
        onBtn->setDisabled( true );
        inPortCmb->setDisabled(true);
        outPortCmb->setDisabled(true);
    }

    m_inPortIndex     = inPortCmb->currentIndex();
    m_outPortIndex    = outPortCmb->currentIndex();

    connect( inPortCmb,     QOverload<int>::of(&QComboBox::currentIndexChanged),[=](int index){ m_inPortIndex = index; });
    connect( outPortCmb,    QOverload<int>::of(&QComboBox::currentIndexChanged),[=](int index){ m_outPortIndex = index;});
    connect( onBtn,         QOverload<bool>::of(&QPushButton::clicked),         [=](bool checked){
        Q_UNUSED(checked);
        if (m_Connected){
            m_Connected = false;
            onBtn->setIcon(QIcon(":/images/off_16.png"));
            onBtn->setToolTip(tr("Connect"));
            m_qmidiin.off();
            m_qmidiout.off();

            sendBtn->setDisabled(true);
            sendBankBtn->setDisabled(true);

            qCDebug(CMN) << Q_FUNC_INFO << "QMidiIn & QMidiOut off!!!";
        }
        else {
            if ( QIN::WORK == m_qmidiin.on( static_cast<unsigned int>(m_inPortIndex))
                 &&
                 QOUT::WORK== m_qmidiout.on( static_cast<unsigned int>(m_outPortIndex))
                 ){
                m_Connected = true;
                onBtn->setIcon(QIcon(":/images/on_16.png"));
                onBtn->setToolTip(tr("Disconnect"));
                qCDebug(CMN) << Q_FUNC_INFO << "QMidiIn & QMidiOut runned!!!";

                sendBankBtn->setEnabled(m_dataLoaded);
                sendBtn->setEnabled(!m_cmdSet.isEmpty());

            }
            else
                qCCritical(CMN) << Q_FUNC_INFO << "QMidiIn & QMidiOut not runned!!!";
        }

        if ( m_dataLoaded != bankCombo->isEnabled() )       bankCombo->setEnabled(m_dataLoaded);
        if ( m_dataLoaded != tabWidget->isEnabled() )       tabWidget->setEnabled(m_dataLoaded);
        if ((!m_Connected)!= inPortCmb->isEnabled())        inPortCmb->setDisabled(m_Connected);
        if ((!m_Connected)!= outPortCmb->isEnabled())       outPortCmb->setDisabled(m_Connected);
        if ( !onBtn->isEnabled() )                          onBtn->setEnabled(true);

    });
    connect( sendBtn,       QOverload<bool>::of(&QPushButton::clicked),         [=](bool checked){
        Q_UNUSED(checked);
        QList<int> sequence = m_cmdSet.toList();
        m_qmidiout.sendBySequence(sequence);
        m_cmdSet.clear();
        updTitle();
    });

    connect( sendBankBtn,   QOverload<bool>::of(&QPushButton::clicked),         [=](bool checked){
        Q_UNUSED(checked);
        int number = bankCombo->currentIndex();
        QList<int> sequence;
        sequence << (-1) << number;
        for(auto cmd: sequence){
            m_cmdSet.remove(cmd);
        }
        m_qmidiout.sendBySequence(sequence);
        updTitle();
    });

    connect(&m_qmidiin,     &QIN::Controller::s_ack,        &m_qmidiout,    &QOUT::Controller::_ack);
    connect(&m_qmidiout,    &QOUT::Controller::s_enableACK, &m_qmidiin,     &QIN::Controller::_enableACK);
    connect(&m_qmidiin,     &QIN::Controller::s_msg, [=](){
        qCDebug(HOM) << Q_FUNC_INFO << "&QIN::Controller::s_msg m_transmission=" << m_transmission;
        rxLabel->setPixmap(m_rxIndicator.flip());
        if ( !m_transmission ){
            m_transmission  = true;
            m_dataLoaded    = false;

            bankCombo->setDisabled(true);
            tabWidget->setDisabled(true);
            sendBankBtn->setDisabled(true);
            inPortCmb->setDisabled(true);
            outPortCmb->setDisabled(true);
            sendBtn->setDisabled(true);
            onBtn->setDisabled(true);

            Dialog::getInstance()->enableLoad( false );
            Dialog::getInstance()->enableSave( false );
        }
    });
    connect(&m_qmidiin,     &QIN::Controller::s_ack, [=](){
        qCDebug(HOM) << Q_FUNC_INFO << "&QIN::Controller::s_ack m_transmission=" << m_transmission;
        if (m_transmission) rxLabel->setPixmap(m_rxIndicator.flip());
    });
    connect(&m_qmidiin,     &QIN::Controller::s_eot, [=](){
        qCDebug(HOM) << Q_FUNC_INFO << "&QIN::Controller::s_eot m_transmission=" << m_transmission;
        if ( m_transmission ){

            _loadNames();

            m_transmission  = false;
            m_dataLoaded    = true;

            if ( m_dataLoaded != bankCombo->isEnabled() )       bankCombo->setEnabled(m_dataLoaded);
            if ( m_dataLoaded != tabWidget->isEnabled() )       tabWidget->setEnabled(m_dataLoaded);
            if ( m_dataLoaded != sendBankBtn->isEnabled())      sendBankBtn->setEnabled(m_dataLoaded);
            if ((!m_Connected)!= inPortCmb->isEnabled())        inPortCmb->setDisabled(m_Connected);
            if ((!m_Connected)!= outPortCmb->isEnabled())       outPortCmb->setDisabled(m_Connected);
            if ( !onBtn->isEnabled() )                          onBtn->setEnabled(true);

            sendBtn->setEnabled(!m_cmdSet.isEmpty());

            rxLabel->setPixmap(m_rxIndicator.off());
            txLabel->setPixmap(m_txIndicator.off());

            sysSetupWidget->updateData();
            Dialog::getInstance()->enableLoad( true );
            Dialog::getInstance()->enableSave( true );
        }
    });
    connect(&m_qmidiout,    &QOUT::Controller::s_sent,[=](){
        qCDebug(HOM) << Q_FUNC_INFO << "&QOUT::Controller::s_sent m_transmission=" << m_transmission;
        txLabel->setPixmap(m_txIndicator.flip());
        if ( !m_transmission ){
            m_transmission = true;

            bankCombo->setDisabled(true);
            tabWidget->setDisabled(true);
            sendBankBtn->setDisabled(true);
            inPortCmb->setDisabled(true);
            outPortCmb->setDisabled(true);
            sendBtn->setDisabled(true);
            onBtn->setDisabled(true);

            Dialog::getInstance()->enableLoad( false );
            Dialog::getInstance()->enableSave( false );
        }
    });
    connect(&m_qmidiout,    &QOUT::Controller::s_end,[=](){
        qCDebug(HOM) << Q_FUNC_INFO << "&QOUT::Controller::s_end m_transmission=" << m_transmission;
        if ( m_transmission ){
            m_transmission  = false;
            if ( m_dataLoaded != bankCombo->isEnabled() )       bankCombo->setEnabled(m_dataLoaded);
            if ( m_dataLoaded != tabWidget->isEnabled() )       tabWidget->setEnabled(m_dataLoaded);
            if ( m_dataLoaded != sendBankBtn->isEnabled())      sendBankBtn->setEnabled(m_dataLoaded);
            if ((!m_Connected)!= inPortCmb->isEnabled())        inPortCmb->setDisabled(m_Connected);
            if ((!m_Connected)!= outPortCmb->isEnabled())       outPortCmb->setDisabled(m_Connected);
            if ( !onBtn->isEnabled() )                          onBtn->setEnabled(true);

            sendBtn->setEnabled(!m_cmdSet.isEmpty());

            rxLabel->setPixmap(m_rxIndicator.off());
            txLabel->setPixmap(m_txIndicator.off());

            Dialog::getInstance()->enableLoad( true );
            Dialog::getInstance()->enableSave( true );
        }
    });


    //////////////////////////////////////////////////////////////////

    //connect( cmnSetupWidget, &QCmnSetupWidget::s_dataLoaded, this, &QHomeWidget::_loadNames );

    mainLayout->addWidget(tabWidget);
    _loadNames();

    bankCombo->setDisabled(true);
    tabWidget->setDisabled(true);
    sendBankBtn->setDisabled(true);
    inPortCmb->setDisabled(false);
    outPortCmb->setDisabled(false);
    sendBtn->setDisabled(true);
    onBtn->setDisabled(false);
}

void QHomeWidget::updTitle(){
    qCDebug(HOM) << Q_FUNC_INFO;
    auto bankCombo  = findChild<QComboBox*>(_bankCombo);
    auto sendBtn    = findChild<QPushButton*>(_sendBtn);

    if (Q_NULLPTR == bankCombo){
        qCCritical(HOM) << Q_FUNC_INFO << "can't find combo!";
        return;
    }
    if (Q_NULLPTR == sendBtn){
        qCCritical(HOM) << Q_FUNC_INFO << "can't find sendBtn!";
        return;
    }

    int index = bankCombo->currentIndex();
    if (index >= 0){
        QString text= bankCombo->itemText( index );

        QString appName =
        (m_cmdSet.isEmpty())?
                    QString("%1 - %2").arg(QString("TB12Ctrl")).arg(text)
                  :
                    QString("%1 - %2 [Config changed]").arg(QString("TB12Ctrl")).arg(text);

        QGuiApplication::setApplicationDisplayName(appName);
        QCoreApplication::setApplicationName(appName);
        Dialog::getInstance()->setWindowTitle( QCoreApplication::applicationName());
    }
    else {
        qCCritical(HOM) << Q_FUNC_INFO << "combo is empty!";
    }
    if (m_Connected) sendBtn->setEnabled(!m_cmdSet.isEmpty());
}

void QHomeWidget::_changed( int number ){
    qCDebug(HOM) << Q_FUNC_INFO << "number=" << number;
    m_cmdSet << number;
    updTitle();
}

void QHomeWidget::_loadNames(){
    qCDebug(HOM) << Q_FUNC_INFO;
    auto bankCombo = findChild<QComboBox*>(_bankCombo);
    if ( Q_NULLPTR != bankCombo ){
        bankCombo->clear();
        QStringList bankNames;
        for( auto bankSettings: SSXMSGS::g_BanksSettings ){
            bankNames << QString(bankSettings.BankName);
        }
        bankCombo->addItems( bankNames );
        bankCombo->setCurrentIndex(0);
    }
    else {
        qCCritical(HOM) << Q_FUNC_INFO << "can't find combo!";
    }
}

QHomeWidget::~QHomeWidget(){
    qCDebug(HOM) << Q_FUNC_INFO;
    m_qmidiin.off();
    m_qmidiout.off();
}

QHomeWidget * QHomeWidget::getInstance(){
    return(g_hw);
}

///QString err;
///SSXMSGS::load("TB12Ctrl.json", err );

bool QHomeWidget::loadFile( QString fileName, QString & err ){
    qCDebug(HOM) << Q_FUNC_INFO << "fileName=" << fileName;

    auto bankCombo = findChild<QComboBox*>(_bankCombo);
    if ( Q_NULLPTR == bankCombo ){
        err = "can't find combo";
        qCCritical(HOM) << Q_FUNC_INFO << err;
        return( false );
    }
    auto tabWidget = findChild<QTabWidget*>(_tabWidget);
    if ( Q_NULLPTR == tabWidget ){
        err = "can't find tabwidget";
        qCCritical(HOM) << Q_FUNC_INFO << err;
        return( false );
    }
    auto sendBankBtn = findChild<QPushButton*>(_sendBankBtn);
    if ( Q_NULLPTR == sendBankBtn ){
        err = "can't find send bank button";
        qCCritical(HOM) << Q_FUNC_INFO << err;
        return( false );
    }
    auto inPortCmb  = findChild<QComboBox*>(_inPortCmb);
    if ( Q_NULLPTR == inPortCmb ){
        err = "can't find in port combo";
        qCCritical(HOM) << Q_FUNC_INFO << err;
        return( false );
    }
    auto outPortCmb = findChild<QComboBox*>(_outPortCmb);
    if ( Q_NULLPTR == outPortCmb ){
        err = "can't find out port combo";
        qCCritical(HOM) << Q_FUNC_INFO << err;
        return( false );
    }
    auto onBtn = findChild<QPushButton*>(_onBtn);
    if ( Q_NULLPTR == onBtn ){
        err = "can't find on button";
        qCCritical(HOM) << Q_FUNC_INFO << err;
        return( false );
    }
    auto sendBtn = findChild<QPushButton*>(_sendBtn);
    if ( Q_NULLPTR == sendBtn ){
        err = "can't find send button";
        qCCritical(HOM) << Q_FUNC_INFO << err;
        return( false );
    }


    bool res = SSXMSGS::load( fileName, err );

    if ( res ){

        m_cmdSet << -1;
        int numBank = 0;
        for( auto bank: SSXMSGS::g_BanksSettings ){
            m_cmdSet << numBank;
            numBank++;
        }

        _loadNames();
        m_dataLoaded    = true;

        bankCombo->setEnabled(true);
        tabWidget->setEnabled(true);
        sendBankBtn->setEnabled(m_Connected);
        inPortCmb->setDisabled(m_Connected);
        outPortCmb->setDisabled(m_Connected);
        if ( !onBtn->isEnabled() ) onBtn->setEnabled(true);
        sendBtn->setEnabled(m_Connected);

        QSysSetupWidget::getInstance()->updateData();
        updTitle();

        Dialog::getInstance()->enableSave( true );
    }
    else {
        m_cmdSet.clear();
        SSXMSGS::init();
        _loadNames();

        bankCombo->setDisabled(true);
        tabWidget->setDisabled(true);
        sendBankBtn->setEnabled(m_Connected);
        inPortCmb->setDisabled(m_Connected);
        outPortCmb->setDisabled(m_Connected);
        if ( !onBtn->isEnabled() ) onBtn->setEnabled(true);
        sendBtn->setEnabled(m_Connected);

        Dialog::getInstance()->enableSave( false );
    }

    return (res);
}

bool QHomeWidget::saveFile( QString fileName, QString & err ){
    qCDebug(HOM) << Q_FUNC_INFO << "fileName=" << fileName;
    bool res = SSXMSGS::save( fileName, err );
    return( res );
}


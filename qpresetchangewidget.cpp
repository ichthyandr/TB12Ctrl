#include "qpresetchangewidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QStringList>
#include <QSpinBox>

#include "qmidi/qmidisysexmsgs.h"
#include "qhomewidget.h"

Q_LOGGING_CATEGORY( PCW,"PCW")

static QPresetChangeWidget * g_preset{Q_NULLPTR};

QPresetChangeWidget::QPresetChangeWidget( QWidget *parent ) : QWidget( parent ){
    qCDebug(PCW) << Q_FUNC_INFO;

    g_preset = this;

    //m_useBankSelect = ( SSXMSGS::g_GlobalSettings.useBankSelectMess == SSXMSGS::UseBankSelectMess::USE_BANK_SELECT);
    //m_useBankSelect = true;

    auto mainLayout = new QHBoxLayout();
    setLayout( mainLayout );

    auto grid = new QGridLayout();
    mainLayout->addLayout( grid );
    mainLayout->addStretch(1000);

    auto gb1 = new QGroupBox(tr("Message 1"));
    auto gb2 = new QGroupBox(tr("Message 2"));
    auto gb3 = new QGroupBox(tr("Message 3"));
    auto gb4 = new QGroupBox(tr("Message 4"));

    grid->addWidget( gb1, 0, 0, Qt::AlignLeft);
    grid->addWidget( gb2, 0, 1, Qt::AlignLeft);
    grid->addWidget( gb3, 1, 0, Qt::AlignLeft);
    grid->addWidget( gb4, 1, 1, Qt::AlignLeft);

    ////////////////////////////////////////////////
    /// message 1
    auto grid1  = new QGridLayout();
    gb1->setLayout(grid1);
    /* program number */
    auto lbl14  = new QLabel(tr("Program Number"));
    auto sb14   = new QSpinBox(); sb14->setRange(1, 128); sb14->setWrapping(true);
    /* bank number */
    auto lbl13  = new QLabel(tr("Bank Number"));
    auto sb13   = new QSpinBox(); sb13->setRange(1, 8); sb13->setWrapping(true);
    /* preset number */
    auto lbl11  = new QLabel(tr("Preset Number"));
    auto le1    = new QLineEdit(); le1->setFixedWidth( 36 ); le1->setReadOnly(true);
    /*MIDI channel*/
    auto lbl12  = new QLabel(tr("MIDI Channel"));
    auto combx1 = new QComboBox();

    grid1->addWidget(lbl14, 0, 0, Qt::AlignRight);
    grid1->addWidget(sb14,  0, 1, Qt::AlignLeft);

    grid1->addWidget(lbl13, 1, 0, Qt::AlignRight);
    grid1->addWidget(sb13,  1, 1, Qt::AlignLeft);

    grid1->addWidget(lbl11, 2, 0, Qt::AlignRight);
    grid1->addWidget(le1,   2, 1, Qt::AlignLeft);

    grid1->addWidget(lbl12, 3, 0, Qt::AlignRight);
    grid1->addWidget(combx1,3, 1, Qt::AlignLeft);

    combx1->addItem(tr("Not Active"), QVariant(MIDI_CHANNEL_NOT_ACTIVE));
    //combx1->setItemData( 0, QColor( Qt::red ), Qt::BackgroundRole );
    combx1->addItem(tr("Primary"), QVariant(MIDI_CHANNEL_PRIMARY));
    for ( int i=0; i<=MAX_MIDI_CHANNEL_NUMBER; i++) {
        QString str = QString("%1").arg(i+1);
        combx1->addItem( str, QVariant(i) );
    }

    connect( combx1, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index){
        qCDebug(PCW) << Q_FUNC_INFO << "index=" << index;

        if ( index < 0 ) return;

        QVariant data = combx1->itemData(index);
        bool ok;
        int midiChannel = data.toInt(&ok);
        if ( !ok ){
            qCCritical(PCW) << Q_FUNC_INFO << "can't convert data!";
        }
        else {
            SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].presetChangeContext.midiChannelNumbers[0] =
                    static_cast<uint8_t>(midiChannel);
            if (m_iaStateProceed) emit s_changed(m_curBnkNumber);
            bool notActive = (midiChannel == MIDI_CHANNEL_NOT_ACTIVE );

            lbl11->setDisabled(notActive);
            le1->setDisabled(notActive);

            int prNum = presetNumber(( SSXMSGS::g_GlobalSettings.useBankSelectMess == SSXMSGS::UseBankSelectMess::USE_BANK_SELECT),0,m_curBnkNumber,m_btnNumber);
            QString preset = (!notActive)?(QString("%1").arg(prNum)):(QString(""));
            le1->setText(preset);

            lbl14->setDisabled( notActive );
            sb14->setDisabled( notActive );
            if (!notActive)
                sb14->setValue(SSXMSGS::g_BanksSettings.at(m_curBnkNumber).buttonContext[m_btnNumber-1].presetChangeContext.programsNumbers[0] + 1);

            lbl13->setDisabled( notActive || (!( SSXMSGS::g_GlobalSettings.useBankSelectMess == SSXMSGS::UseBankSelectMess::USE_BANK_SELECT)));
            sb13->setDisabled( notActive || (!( SSXMSGS::g_GlobalSettings.useBankSelectMess == SSXMSGS::UseBankSelectMess::USE_BANK_SELECT)));
            if (!(notActive || (!( SSXMSGS::g_GlobalSettings.useBankSelectMess == SSXMSGS::UseBankSelectMess::USE_BANK_SELECT))))
                sb13->setValue(SSXMSGS::g_BanksSettings.at(m_curBnkNumber).buttonContext[m_btnNumber-1].presetChangeContext.banksNumbers[0]+1);
        }
    });
    connect( sb14, QOverload<int>::of(&QSpinBox::valueChanged), [=](int newValue){
        qCDebug(PCW) << Q_FUNC_INFO << "newValue=" << newValue;
        uint8_t data = static_cast<uint8_t>(newValue);
        SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].presetChangeContext.programsNumbers[0] = data-1;
        int prNum = presetNumber(( SSXMSGS::g_GlobalSettings.useBankSelectMess == SSXMSGS::UseBankSelectMess::USE_BANK_SELECT),0,m_curBnkNumber,m_btnNumber);
        QString preset = QString("%1").arg(prNum);
        le1->setText(preset);
        if (m_iaStateProceed) emit s_changed(m_curBnkNumber);
    });
    connect( sb13, QOverload<int>::of(&QSpinBox::valueChanged), [=](int newValue){
        qCDebug(PCW) << Q_FUNC_INFO << "newValue=" << newValue;
        uint8_t data = static_cast<uint8_t>(newValue);
        SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].presetChangeContext.banksNumbers[0] = data-1;
        int prNum = presetNumber(( SSXMSGS::g_GlobalSettings.useBankSelectMess == SSXMSGS::UseBankSelectMess::USE_BANK_SELECT),0,m_curBnkNumber,m_btnNumber);
        QString preset = QString("%1").arg(prNum);
        le1->setText(preset);
        if (m_iaStateProceed) emit s_changed(m_curBnkNumber);
    });

    ////////////////////////////////////////////////
    /// message 2
    auto grid2  = new QGridLayout();
    gb2->setLayout(grid2);
    /* program number */
    auto lbl24  = new QLabel(tr("Program Number"));
    auto sb24   = new QSpinBox(); sb24->setRange(1, 128); sb24->setWrapping(true);
    /* bank number */
    auto lbl23  = new QLabel(tr("Bank Number"));
    auto sb23   = new QSpinBox(); sb23->setRange(1, 8); sb23->setWrapping(true);
    /* preset number */
    auto lbl21  = new QLabel(tr("Preset Number"));
    auto le2    = new QLineEdit(); le2->setFixedWidth( 36 ); le2->setReadOnly(true);
    /*MIDI channel*/
    auto lbl22  = new QLabel(tr("MIDI Channel"));
    auto combx2 = new QComboBox();

    grid2->addWidget(lbl24, 0, 0, Qt::AlignRight);
    grid2->addWidget(sb24,  0, 1, Qt::AlignLeft);

    grid2->addWidget(lbl23, 1, 0, Qt::AlignRight);
    grid2->addWidget(sb23,  1, 1, Qt::AlignLeft);

    grid2->addWidget(lbl21, 2, 0, Qt::AlignRight);
    grid2->addWidget(le2,   2, 1, Qt::AlignLeft);

    grid2->addWidget(lbl22, 3, 0, Qt::AlignRight);
    grid2->addWidget(combx2,3, 1, Qt::AlignLeft);

    combx2->addItem(tr("Not Active"), QVariant(MIDI_CHANNEL_NOT_ACTIVE));
    combx2->addItem(tr("Primary"), QVariant(MIDI_CHANNEL_PRIMARY));
    for ( int i=0; i<=MAX_MIDI_CHANNEL_NUMBER; i++) {
        QString str = QString("%1").arg(i+1);
        combx2->addItem( str, QVariant(i) );
    }

    connect( combx2, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index){
        qCDebug(PCW) << Q_FUNC_INFO << "index=" << index;

        if ( index < 0 ) return;

        QVariant data = combx2->itemData(index);
        bool ok;
        int midiChannel = data.toInt(&ok);
        if ( !ok ){
            qCCritical(PCW) << Q_FUNC_INFO << "can't convert data!";
        }
        else {
            SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].presetChangeContext.midiChannelNumbers[1] =
                    static_cast<uint8_t>(midiChannel);
            if (m_iaStateProceed) emit s_changed(m_curBnkNumber);
            bool notActive = (midiChannel == MIDI_CHANNEL_NOT_ACTIVE );

            lbl21->setDisabled(notActive);
            le2->setDisabled(notActive);

            int prNum = presetNumber(( SSXMSGS::g_GlobalSettings.useBankSelectMess == SSXMSGS::UseBankSelectMess::USE_BANK_SELECT),1,m_curBnkNumber,m_btnNumber);
            QString preset = (!notActive)?(QString("%1").arg(prNum)):(QString(""));
            le2->setText(preset);

            lbl24->setDisabled( notActive );
            sb24->setDisabled( notActive );
            if (!notActive)
                sb24->setValue(SSXMSGS::g_BanksSettings.at(m_curBnkNumber).buttonContext[m_btnNumber-1].presetChangeContext.programsNumbers[1] + 1);

            lbl23->setDisabled( notActive || (!( SSXMSGS::g_GlobalSettings.useBankSelectMess == SSXMSGS::UseBankSelectMess::USE_BANK_SELECT)));
            sb23->setDisabled( notActive || (!( SSXMSGS::g_GlobalSettings.useBankSelectMess == SSXMSGS::UseBankSelectMess::USE_BANK_SELECT)));
            if (!(notActive || (!( SSXMSGS::g_GlobalSettings.useBankSelectMess == SSXMSGS::UseBankSelectMess::USE_BANK_SELECT))))
                sb23->setValue(SSXMSGS::g_BanksSettings.at(m_curBnkNumber).buttonContext[m_btnNumber-1].presetChangeContext.banksNumbers[1]+1);
        }
    });
    connect( sb24, QOverload<int>::of(&QSpinBox::valueChanged), [=](int newValue){
        qCDebug(PCW) << Q_FUNC_INFO << "newValue=" << newValue;
        uint8_t data = static_cast<uint8_t>(newValue);
        SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].presetChangeContext.programsNumbers[1] = data-1;
        int prNum = presetNumber(( SSXMSGS::g_GlobalSettings.useBankSelectMess == SSXMSGS::UseBankSelectMess::USE_BANK_SELECT),1,m_curBnkNumber,m_btnNumber);
        QString preset = QString("%1").arg(prNum);
        le2->setText(preset);
        if (m_iaStateProceed) emit s_changed(m_curBnkNumber);
    });
    connect( sb23, QOverload<int>::of(&QSpinBox::valueChanged), [=](int newValue){
        qCDebug(PCW) << Q_FUNC_INFO << "newValue=" << newValue;
        uint8_t data = static_cast<uint8_t>(newValue);
        SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].presetChangeContext.banksNumbers[1] = data-1;
        int prNum = presetNumber(( SSXMSGS::g_GlobalSettings.useBankSelectMess == SSXMSGS::UseBankSelectMess::USE_BANK_SELECT),1,m_curBnkNumber,m_btnNumber);
        QString preset = QString("%1").arg(prNum);
        le2->setText(preset);
        if (m_iaStateProceed) emit s_changed(m_curBnkNumber);
    });

    ////////////////////////////////////////////////
    /// message 3
    auto grid3  = new QGridLayout();
    gb3->setLayout(grid3);
    /* program number */
    auto lbl34  = new QLabel(tr("Program Number"));
    auto sb34   = new QSpinBox(); sb34->setRange(1, 128); sb34->setWrapping(true);
    /* bank number */
    auto lbl33  = new QLabel(tr("Bank Number"));
    auto sb33   = new QSpinBox(); sb33->setRange(1, 8); sb33->setWrapping(true);
    /* preset number */
    auto lbl31  = new QLabel(tr("Preset Number"));
    auto le3    = new QLineEdit(); le3->setFixedWidth( 36 ); le3->setReadOnly(true);
    grid3->addWidget(lbl31, 0, 0, Qt::AlignRight);
    grid3->addWidget(le3,   0, 1, Qt::AlignLeft);
    /*MIDI channel*/
    auto lbl32  = new QLabel(tr("MIDI Channel"));
    auto combx3 = new QComboBox();

    grid3->addWidget(lbl34, 0, 0, Qt::AlignRight);
    grid3->addWidget(sb34,  0, 1, Qt::AlignLeft);

    grid3->addWidget(lbl33, 1, 0, Qt::AlignRight);
    grid3->addWidget(sb33,  1, 1, Qt::AlignLeft);

    grid3->addWidget(lbl31, 2, 0, Qt::AlignRight);
    grid3->addWidget(le3,   2, 1, Qt::AlignLeft);

    grid3->addWidget(lbl32, 3, 0, Qt::AlignRight);
    grid3->addWidget(combx3,3, 1, Qt::AlignLeft);

    combx3->addItem(tr("Not Active"), QVariant(MIDI_CHANNEL_NOT_ACTIVE));
    combx3->addItem(tr("Primary"), QVariant(MIDI_CHANNEL_PRIMARY));
    for ( int i=0; i<=MAX_MIDI_CHANNEL_NUMBER; i++) {
        QString str = QString("%1").arg(i+1);
        combx3->addItem( str, QVariant(i) );
    }

    connect( combx3, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index){
        qCDebug(PCW) << Q_FUNC_INFO << "index=" << index;

        if (index<0) return;

        QVariant data = combx3->itemData(index);
        bool ok;
        int midiChannel = data.toInt(&ok);
        if ( !ok ){
            qCCritical(PCW) << Q_FUNC_INFO << "can't convert data!";
        }
        else {
            SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].presetChangeContext.midiChannelNumbers[2] =
                    static_cast<uint8_t>(midiChannel);
            if (m_iaStateProceed) emit s_changed(m_curBnkNumber);
            bool notActive = (midiChannel == MIDI_CHANNEL_NOT_ACTIVE );

            lbl31->setDisabled(notActive);
            le3->setDisabled(notActive);

            int prNum = presetNumber(( SSXMSGS::g_GlobalSettings.useBankSelectMess == SSXMSGS::UseBankSelectMess::USE_BANK_SELECT),2,m_curBnkNumber,m_btnNumber);
            QString preset = (!notActive)?(QString("%1").arg(prNum)):(QString(""));
            le3->setText(preset);

            lbl34->setDisabled( notActive );
            sb34->setDisabled( notActive );
            if (!notActive )
                sb34->setValue(SSXMSGS::g_BanksSettings.at(m_curBnkNumber).buttonContext[m_btnNumber-1].presetChangeContext.programsNumbers[2] + 1);

            lbl33->setDisabled( notActive || (!( SSXMSGS::g_GlobalSettings.useBankSelectMess == SSXMSGS::UseBankSelectMess::USE_BANK_SELECT)));
            sb33->setDisabled( notActive || (!( SSXMSGS::g_GlobalSettings.useBankSelectMess == SSXMSGS::UseBankSelectMess::USE_BANK_SELECT)));
            if (!(notActive || (!( SSXMSGS::g_GlobalSettings.useBankSelectMess == SSXMSGS::UseBankSelectMess::USE_BANK_SELECT))))
                sb33->setValue(SSXMSGS::g_BanksSettings.at(m_curBnkNumber).buttonContext[m_btnNumber-1].presetChangeContext.banksNumbers[2]+1);
        }
    });
    connect( sb34, QOverload<int>::of(&QSpinBox::valueChanged), [=](int newValue){
        qCDebug(PCW) << Q_FUNC_INFO << "newValue=" << newValue;
        uint8_t data = static_cast<uint8_t>(newValue);
        SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].presetChangeContext.programsNumbers[2] = data-1;
        int prNum = presetNumber(( SSXMSGS::g_GlobalSettings.useBankSelectMess == SSXMSGS::UseBankSelectMess::USE_BANK_SELECT),2,m_curBnkNumber,m_btnNumber);
        QString preset = QString("%1").arg(prNum);
        le3->setText(preset);
        if (m_iaStateProceed) emit s_changed(m_curBnkNumber);
    });
    connect( sb33, QOverload<int>::of(&QSpinBox::valueChanged), [=](int newValue){
        qCDebug(PCW) << Q_FUNC_INFO << "newValue=" << newValue;
        uint8_t data = static_cast<uint8_t>(newValue);
        SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].presetChangeContext.banksNumbers[2] = data-1;
        int prNum = presetNumber(( SSXMSGS::g_GlobalSettings.useBankSelectMess == SSXMSGS::UseBankSelectMess::USE_BANK_SELECT),2,m_curBnkNumber,m_btnNumber);
        QString preset = QString("%1").arg(prNum);
        le3->setText(preset);
        if (m_iaStateProceed) emit s_changed(m_curBnkNumber);
    });

    ////////////////////////////////////////////////
    /// message 4
    auto grid4  = new QGridLayout();
    gb4->setLayout(grid4);

    /* program number */
    auto lbl44  = new QLabel(tr("Program Number"));
    auto sb44   = new QSpinBox(); sb44->setRange(1, 128); sb44->setWrapping(true);
    /* bank number */
    auto lbl43  = new QLabel(tr("Bank Number"));
    auto sb43   = new QSpinBox(); sb43->setRange(1, 8); sb43->setWrapping(true);
    /* preset number */
    auto lbl41  = new QLabel(tr("Preset Number"));
    auto le4    = new QLineEdit(); le4->setFixedWidth( 36 ); le4->setReadOnly(true);
    /*MIDI channel*/
    auto lbl42  = new QLabel(tr("MIDI Channel"));
    auto combx4 = new QComboBox();

    grid4->addWidget(lbl44, 0, 0, Qt::AlignRight);
    grid4->addWidget(sb44,  0, 1, Qt::AlignLeft);

    grid4->addWidget(lbl43, 1, 0, Qt::AlignRight);
    grid4->addWidget(sb43,  1, 1, Qt::AlignLeft);

    grid4->addWidget(lbl41, 2, 0, Qt::AlignRight);
    grid4->addWidget(le4,   2, 1, Qt::AlignLeft);

    grid4->addWidget(lbl42, 4, 0, Qt::AlignRight);
    grid4->addWidget(combx4,4, 1, Qt::AlignLeft);

    combx4->addItem(tr("Not Active"), QVariant(MIDI_CHANNEL_NOT_ACTIVE));
    combx4->addItem(tr("Primary"), QVariant(MIDI_CHANNEL_PRIMARY));
    for ( int i=0; i<=MAX_MIDI_CHANNEL_NUMBER; i++) {
        QString str = QString("%1").arg(i+1);
        combx4->addItem( str, QVariant(i) );
    }

    connect( combx4, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index){
        qCDebug(PCW) << Q_FUNC_INFO << "index=" << index;

        if ( index<0 ) return;

        QVariant data = combx4->itemData(index);
        bool ok;
        int midiChannel = data.toInt(&ok);
        if ( !ok ){
            qCCritical(PCW) << Q_FUNC_INFO << "can't convert data!";
        }
        else {
            SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].presetChangeContext.midiChannelNumbers[3] =
                    static_cast<uint8_t>(midiChannel);
            if (m_iaStateProceed) emit s_changed(m_curBnkNumber);
            bool notActive = (midiChannel == MIDI_CHANNEL_NOT_ACTIVE );

            lbl41->setDisabled(notActive);
            le4->setDisabled(notActive);

            int prNum = presetNumber(( SSXMSGS::g_GlobalSettings.useBankSelectMess == SSXMSGS::UseBankSelectMess::USE_BANK_SELECT),3,m_curBnkNumber,m_btnNumber);
            QString preset = (!notActive)?(QString("%1").arg(prNum)):(QString(""));
            le4->setText(preset);

            lbl44->setDisabled( notActive );
            sb44->setDisabled( notActive );
            if ( !notActive )
                sb44->setValue(SSXMSGS::g_BanksSettings.at(m_curBnkNumber).buttonContext[m_btnNumber-1].presetChangeContext.programsNumbers[3] + 1);

            lbl43->setDisabled( notActive || (!( SSXMSGS::g_GlobalSettings.useBankSelectMess == SSXMSGS::UseBankSelectMess::USE_BANK_SELECT)));
            sb43->setDisabled( notActive || (!( SSXMSGS::g_GlobalSettings.useBankSelectMess == SSXMSGS::UseBankSelectMess::USE_BANK_SELECT)));
            if (!(notActive || (!( SSXMSGS::g_GlobalSettings.useBankSelectMess == SSXMSGS::UseBankSelectMess::USE_BANK_SELECT))))
                sb43->setValue(SSXMSGS::g_BanksSettings.at(m_curBnkNumber).buttonContext[m_btnNumber-1].presetChangeContext.banksNumbers[3]+1);
        }
    });
    connect( sb44, QOverload<int>::of(&QSpinBox::valueChanged), [=](int newValue){
        qCDebug(PCW) << Q_FUNC_INFO << "newValue=" << newValue;
        uint8_t data = static_cast<uint8_t>(newValue);
        SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].presetChangeContext.programsNumbers[3] = data-1;
        int prNum = presetNumber(( SSXMSGS::g_GlobalSettings.useBankSelectMess == SSXMSGS::UseBankSelectMess::USE_BANK_SELECT),3,m_curBnkNumber,m_btnNumber);
        QString preset = QString("%1").arg(prNum);
        le4->setText(preset);
        if (m_iaStateProceed) emit s_changed(m_curBnkNumber);
    });
    connect( sb43, QOverload<int>::of(&QSpinBox::valueChanged), [=](int newValue){
        qCDebug(PCW) << Q_FUNC_INFO << "newValue=" << newValue;
        uint8_t data = static_cast<uint8_t>(newValue);
        SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].presetChangeContext.banksNumbers[3] = data-1;
        int prNum = presetNumber(( SSXMSGS::g_GlobalSettings.useBankSelectMess == SSXMSGS::UseBankSelectMess::USE_BANK_SELECT),3,m_curBnkNumber,m_btnNumber);
        QString preset = QString("%1").arg(prNum);
        le4->setText(preset);
        if (m_iaStateProceed) emit s_changed(m_curBnkNumber);
    });

    m_msgCombos << combx1 << combx2 << combx3 << combx4;

    ////////////////////////////////////////////////
    /// Initial IA States
    auto gbIA = new QGroupBox(tr("Initial IA States"));
    grid->addWidget( gbIA, 2, 0, 1, 3, Qt::AlignLeft );
    auto gridIA = new QGridLayout();
    gbIA->setLayout(gridIA);

    auto ia1 = new QCheckBox(tr("IA 1"));
    auto ia2 = new QCheckBox(tr("IA 2"));
    auto ia3 = new QCheckBox(tr("IA 3"));
    auto ia4 = new QCheckBox(tr("IA 4"));
    auto ia5 = new QCheckBox(tr("IA 5"));
    auto ia6 = new QCheckBox(tr("IA 6"));
    auto ia7 = new QCheckBox(tr("IA 7"));
    auto ia8 = new QCheckBox(tr("IA 8"));
    auto ia9 = new QCheckBox(tr("IA 9"));
    auto ia10= new QCheckBox(tr("IA 10"));
    auto ia11= new QCheckBox(tr("IA 11"));
    auto ia12= new QCheckBox(tr("IA 12"));

    gridIA->addWidget(ia1, 0, 0, Qt::AlignLeft);
    gridIA->addWidget(ia2, 0, 1, Qt::AlignLeft);
    gridIA->addWidget(ia3, 0, 2, Qt::AlignLeft);
    gridIA->addWidget(ia4, 0, 3, Qt::AlignLeft);
    gridIA->addWidget(ia5, 0, 4, Qt::AlignLeft);
    gridIA->addWidget(ia6, 0, 5, Qt::AlignLeft);

    gridIA->addWidget(ia7, 1, 0, Qt::AlignLeft);
    gridIA->addWidget(ia8, 1, 1, Qt::AlignLeft);
    gridIA->addWidget(ia9, 1, 2, Qt::AlignLeft);
    gridIA->addWidget(ia10,1, 3, Qt::AlignLeft);
    gridIA->addWidget(ia11,1, 4, Qt::AlignLeft);
    gridIA->addWidget(ia12,1, 5, Qt::AlignLeft);

    m_iaStateBG.setExclusive(false);
    m_iaStateBG.addButton(ia1, 0);
    m_iaStateBG.addButton(ia2, 1);
    m_iaStateBG.addButton(ia3, 2);
    m_iaStateBG.addButton(ia4, 3);
    m_iaStateBG.addButton(ia5, 4);
    m_iaStateBG.addButton(ia6, 5);
    m_iaStateBG.addButton(ia7, 6);
    m_iaStateBG.addButton(ia8, 7);
    m_iaStateBG.addButton(ia9, 8);
    m_iaStateBG.addButton(ia10, 9);
    m_iaStateBG.addButton(ia11, 10);
    m_iaStateBG.addButton(ia12, 11);

    connect(&m_iaStateBG, QOverload<int, bool>::of(&QButtonGroup::buttonToggled),[=](int id, bool checked){

        if (!m_iaStateProceed) return ;

        uint16_t mask = 0x0001;
        uint16_t _mask= static_cast<uint16_t>(mask << id);
        uint16_t data =
        SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].presetChangeContext.iaState;
        uint16_t res = 0;
        if (checked)
            res = data | _mask;
        else
            res = data ^ _mask;
        SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].presetChangeContext.iaState = res;
        qCDebug(PCW) << Q_FUNC_INFO << "m_curBnkNumber=" << m_curBnkNumber << "m_btnNumber=" << m_btnNumber << "id=" << id << "checked=" << checked ;
        qCDebug(PCW) << Q_FUNC_INFO << hex << "_mask=" << _mask << "data=" << data << "res=" << res;
        emit s_changed(m_curBnkNumber);
    });

    ////////////////////////////////////////////////
    /// Relay States
    auto gbRS = new QGroupBox(tr("Relay States"));
    grid->addWidget( gbRS, 0, 2, 3, 1, Qt::AlignLeft );
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

    connect( this, &QPresetChangeWidget::s_changed, QHomeWidget::getInstance(), &QHomeWidget::_changed );
}

QPresetChangeWidget::~QPresetChangeWidget(){
    qCDebug(PCW) << Q_FUNC_INFO;
}

int QPresetChangeWidget::presetNumber( bool b_useBankSelect, int number, int bank, int button){

    //qCDebug(PCW) << Q_FUNC_INFO << "number=" << number;
    //qCDebug(PCW) << Q_FUNC_INFO << "banksNumbers=" <<
    //                SSXMSGS::g_BanksSettings.at(m_curBnkNumber).buttonContext[m_btnNumber-1].presetChangeContext.banksNumbers[number];
    //qCDebug(PCW) << Q_FUNC_INFO << "programsNumbers=" <<
    //                SSXMSGS::g_BanksSettings.at(m_curBnkNumber).buttonContext[m_btnNumber-1].presetChangeContext.programsNumbers[number];

    if ( number >=0 && number <=3 ){
        if ( b_useBankSelect &&
             SSXMSGS::g_BanksSettings.at(bank).buttonContext[button-1].presetChangeContext.banksNumbers[number] > 7 )
            SSXMSGS::g_BanksSettings[bank].buttonContext[button-1].presetChangeContext.banksNumbers[number] = 0;

        if ( SSXMSGS::g_BanksSettings.at(bank).buttonContext[button-1].presetChangeContext.programsNumbers[number] > 127 )
            SSXMSGS::g_BanksSettings[bank].buttonContext[button-1].presetChangeContext.programsNumbers[number] = 0;
    }

    int res=0;
    if ( number >=0 && number <=3 ){
        res = ( b_useBankSelect )? \
        ((SSXMSGS::g_BanksSettings.at(bank).buttonContext[button-1].presetChangeContext.banksNumbers[number]/* + 1*/) *128 + \
          SSXMSGS::g_BanksSettings.at(bank).buttonContext[button-1].presetChangeContext.programsNumbers[number] + 1 ): \
        ( SSXMSGS::g_BanksSettings.at(bank).buttonContext[button-1].presetChangeContext.programsNumbers[number] + 1 );
    }
    //qCDebug(PCW) << Q_FUNC_INFO << "res=" << res;
    return (res);
}

void QPresetChangeWidget::relays( int index, int shift, QComboBox * combo ){
    qCDebug(PCW) << Q_FUNC_INFO << "index=" << index << "shift=" << shift << "m_iaStateProceed=" << m_iaStateProceed << "combo=" << hex << combo;
    if (!m_iaStateProceed) return;
    if (Q_NULLPTR == combo)return;
    QVariant var = combo->itemData( index);
    bool ok;
    int _mask = var.toInt(&ok);
    if (!ok){
        qCCritical(PCW) << Q_FUNC_INFO << "can't convert to int!";
        return;
    }
    uint8_t mask = static_cast<uint8_t>(_mask); mask <<= shift;
    uint8_t data = SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].relays;
    uint8_t res  = data | mask;
    SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].relays = res;
    if (m_iaStateProceed) emit s_changed(m_curBnkNumber);
}

QPresetChangeWidget * QPresetChangeWidget::getInstance(){
    return(g_preset);
}

void QPresetChangeWidget::setButton(int btnNumber, int curBnkNumber){
    qCDebug(PCW) << Q_FUNC_INFO << "btnNumber=" << btnNumber << "curBnkNumber=" << curBnkNumber;

    if ( curBnkNumber<0 ){
        qCCritical(PCW) << Q_FUNC_INFO << "got bad bank number!";
        return;
    }
    if (!(( btnNumber>=1 ) && ( btnNumber<=FOOT_BUTTONS_NUM ))){
        qCCritical(PCW) << Q_FUNC_INFO << "got bad button number!";
        return;
    }

    m_btnNumber     = btnNumber;
    m_curBnkNumber  = curBnkNumber;

    SSXMSGS::ButtonType buttonType = SSXMSGS::g_BanksSettings.at(m_curBnkNumber).buttonType[m_btnNumber-1];
    if ( buttonType != SSXMSGS::ButtonType::PRESET_CHANGE ){
        qCCritical(PCW) << Q_FUNC_INFO << "got bad button type!";
        return;
    }

    m_iaStateProceed = false;

    //m_useBankSelect = ( SSXMSGS::g_GlobalSettings.useBankSelectMess == SSXMSGS::UseBankSelectMess::USE_BANK_SELECT);

    for(int i=0; i<4; i++ ){
        int midiChannelNumber = SSXMSGS::g_BanksSettings.at(m_curBnkNumber).buttonContext[m_btnNumber-1].presetChangeContext.midiChannelNumbers[i];
        qCDebug(PCW) << Q_FUNC_INFO << "i=" << i << "midiChannelNumber=" << midiChannelNumber;
        int index = m_msgCombos.at(i)->findData(QVariant(midiChannelNumber));
        if (index>=0){
            m_msgCombos.at(i)->setCurrentIndex(-1);
            m_msgCombos.at(i)->setCurrentIndex(index);
        }
        else
            qCCritical(PCW) << Q_FUNC_INFO << "can't find data for combo=" << (i+1);
    }

    //set iaStates checkboxes
    uint16_t    mask        = 0x0001;
    int         bitNumbers  = m_iaStateBG.buttons().size();
    for(int i=0; i<bitNumbers; i++){
        uint16_t    data    = SSXMSGS::g_BanksSettings.at(m_curBnkNumber).buttonContext[m_btnNumber-1].presetChangeContext.iaState;
        uint16_t    _mask   = static_cast<uint16_t>(mask << i);
        bool        checked = ((data & _mask)>0);
        m_iaStateBG.buttons().at(i)->setChecked(checked);
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
    m_iaStateProceed = true;
}

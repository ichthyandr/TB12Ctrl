#include "dialog.h"
#include "qtbbutton.h"
#include "qpresetchangewidget.h"
#include "qcctwidget.h"
#include "qcccwidget.h"
#include "qccmwidget.h"

#include "qbuttonswidget.h"
#include "qhomewidget.h"
#include "qbtnsetupwidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QComboBox>
#include <QStackedLayout>
#include <QLayout>
#include <QFileDialog>
#include <QMessageBox>

#include <QMenuBar>
#include <QMenu>
#include <QActionGroup>

Q_LOGGING_CATEGORY( DLG, "DLG")

// object names
static const QString _sl            = QString("DlgStackedLayout");
static const QString _btnSetups     = QString("DlgQBtnSetupWidget");
static const QString _saveAct       = QString("SaveAct");

static Dialog * g_dialog{Q_NULLPTR};

Dialog::Dialog(QWidget *parent):QDialog(parent){

    qCDebug(DLG) << Q_FUNC_INFO;

    // remove question mark
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);

    g_dialog = this;

    auto mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

    ///
    ///
    QMenuBar* menuBar = new QMenuBar();
    QMenu *fileMenu = new QMenu(tr("File"));
    menuBar->addMenu(fileMenu);
    m_loadAct = fileMenu->addAction(QIcon(":/images/open_16.png"),tr("Load"));
    m_saveAct = fileMenu->addAction(QIcon(":/images/save_16.png"),tr("Save"));
    fileMenu->addSeparator();
    auto exitAct = fileMenu->addAction(QIcon(":/images/exit_16.png"),tr("Exit"));

    m_saveAct->setDisabled( true );
    m_loadAct->setEnabled( true );

    //Controller models
    QMenu *ctlMenu  = new QMenu(tr("Controller models"));
    m_ctlActMenu    = menuBar->addMenu(ctlMenu);

    auto tb5Act     = ctlMenu->addAction(tr("TB_5 MKII"));
    auto tb8Act     = ctlMenu->addAction(tr("TB_8 MKII"));
    auto tb12Act    = ctlMenu->addAction(tr("TB_12 MKII"));
    auto tbSep      = ctlMenu->addSeparator();
    tbSep->setText(tr("With pedals"));
    auto tb6pAct    = ctlMenu->addAction(tr("TB_6P MKII"));
    auto tb11pAct   = ctlMenu->addAction(tr("TB_11P MKII"));

    tb5Act->setCheckable(true);
    tb8Act->setCheckable(true);
    tb12Act->setCheckable(true);
    tb6pAct->setCheckable(true);
    tb11pAct->setCheckable(true);

    auto ctlGroup = new QActionGroup(this);
    ctlGroup->addAction(tb5Act);
    ctlGroup->addAction(tb8Act);
    ctlGroup->addAction(tb12Act);
    ctlGroup->addAction(tb6pAct);
    ctlGroup->addAction(tb11pAct);
    tb12Act->setChecked(true);

    m_modelsAct << tb5Act << tb8Act << tb12Act << tb6pAct << tb11pAct;

    this->layout()->setMenuBar(menuBar);

    ///

    auto sl = new QStackedLayout();
    mainLayout->addLayout(sl);
    sl->setObjectName(_sl);

    auto home           = new QHomeWidget();
    auto btnSetups      = new QBtnSetupWidget();

    btnSetups->setObjectName(_btnSetups);

    sl->addWidget( home );
    sl->addWidget( btnSetups );

    //setFixedSize(Hint().)
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);

    connect( QButtonsWidget::getInstance(), &QButtonsWidget::s_clickedBtn, this , &Dialog::_clickedBtn );
    connect( btnSetups, &QBtnSetupWidget::s_back, this, &Dialog::_back );

    connect( m_loadAct, &QAction::triggered, [=](){
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open data"), ".", "*.json");
        if ( !fileName.isEmpty()){
            QString err;
            if (!home->loadFile( fileName, err )){
                QMessageBox::critical(this, "Load error", err);
            }
        }
    });

    connect( m_saveAct, &QAction::triggered, [=](){
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save data"), ".", "*.json");
        if ( !fileName.isEmpty() ){
            QString err;
            if (!home->saveFile( fileName, err )){
                QMessageBox::critical(this, "Save error", err);
            }
        }
    });

    connect( exitAct, &QAction::triggered, [=](){
        close();
    });

    ///--------------------- models actions ----------------------
    connect( tb5Act,  &QAction::triggered, [=](){
        SSXMSGS::g_Model = SSXMSGS::TB_5_MKII;
        //home->reset();
        /*
        QButtonsWidget::getInstance()->maskBtns();
        QButtonsWidget::getInstance()->update(0);
        */
        QHomeWidget::getInstance()->_loadNames();
    });
    connect( tb8Act,  &QAction::triggered, [=](){
        SSXMSGS::g_Model = SSXMSGS::TB_8_MKII;
        //home->reset();
        /*
        QButtonsWidget::getInstance()->maskBtns();
        QButtonsWidget::getInstance()->update(0);
        */
        QHomeWidget::getInstance()->_loadNames();
    });
    connect( tb12Act, &QAction::triggered, [=](){
        SSXMSGS::g_Model = SSXMSGS::TB_12_MKII;
        //home->reset();
        /*
        QButtonsWidget::getInstance()->maskBtns();
        QButtonsWidget::getInstance()->update(0);
        */
        QHomeWidget::getInstance()->_loadNames();
    });
    connect( tb6pAct, &QAction::triggered, [=](){
        SSXMSGS::g_Model = SSXMSGS::TB_6P_MKII;
        //home->reset();
        /*
        QButtonsWidget::getInstance()->maskBtns();
        QButtonsWidget::getInstance()->update(0);
        */
        QHomeWidget::getInstance()->_loadNames();
    });
    connect( tb11pAct,&QAction::triggered, [=](){
        SSXMSGS::g_Model = SSXMSGS::TB_11P_MKII;
        //home->reset();
        /*
        QButtonsWidget::getInstance()->maskBtns();
        QButtonsWidget::getInstance()->update(0);
        */
        QHomeWidget::getInstance()->_loadNames();
    });
}

void Dialog::refreshModel(){
    qCDebug(DLG) << Q_FUNC_INFO << "SSXMSGS::g_Model=" << SSXMSGS::g_Model;
    switch (SSXMSGS::g_Model){
    case SSXMSGS::TB_5_MKII:
        m_modelsAct.at(0)->setChecked(true);
        break;
    case SSXMSGS::TB_8_MKII:
        m_modelsAct.at(1)->setChecked(true);
        break;
    case SSXMSGS::TB_12_MKII:
        m_modelsAct.at(2)->setChecked(true);
        break;
    case SSXMSGS::TB_6P_MKII:
        m_modelsAct.at(3)->setChecked(true);
        break;
    case SSXMSGS::TB_11P_MKII:
        m_modelsAct.at(4)->setChecked(true);
        break;
    }
}

Dialog * Dialog::getInstance(){
    return(g_dialog);
}

void Dialog::setCurrentBankNumber( int bankNumber ){
    qCDebug(DLG) << Q_FUNC_INFO << "bankNumber=" << bankNumber;
    if ( bankNumber >= 0 ){
        m_currentBankNumber = bankNumber;
    }
    else {
        qCCritical(DLG) << Q_FUNC_INFO << "got bad bank number!";
    }
}

void Dialog::_clickedBtn( int btnNumber ){
    qCDebug(DLG) << Q_FUNC_INFO << "number=" << btnNumber;
    auto sl  = findChild<QStackedLayout*>(_sl);
    auto btnSetups = findChild<QBtnSetupWidget*>(_btnSetups);
    if ( Q_NULLPTR == sl ){
        qCCritical(DLG) << Q_FUNC_INFO << "can't find stack!";
        return;
    }
    if ( Q_NULLPTR == btnSetups ){
        qCCritical(DLG) << Q_FUNC_INFO << "can't find button setup widget!";
        return;
    }
    btnSetups->setButton( btnNumber, m_currentBankNumber );
    sl->setCurrentIndex(1);
}

void Dialog::_back(){
    qCDebug(DLG) << Q_FUNC_INFO;
    auto sl  = findChild<QStackedLayout*>(_sl);
    if ( Q_NULLPTR == sl ){
        qCCritical(DLG) << Q_FUNC_INFO << "can't find stack!";
        return;
    }
    sl->setCurrentIndex(0);
}

void Dialog::enableSave( bool enable ){
    qCDebug(DLG) << Q_FUNC_INFO << "enable=" << enable;
    if ( Q_NULLPTR != m_saveAct ) m_saveAct->setEnabled( enable );
}

void Dialog::enableLoad( bool enable ){
    qCDebug(DLG) << Q_FUNC_INFO << "enable=" << enable;
    if ( Q_NULLPTR != m_loadAct ) m_loadAct->setEnabled( enable );
}

void Dialog::enableModels( bool enable ){
    qCDebug(DLG) << Q_FUNC_INFO << "enable=" << enable;
    if ( Q_NULLPTR != m_ctlActMenu ) m_ctlActMenu->setEnabled( enable );
}

Dialog::~Dialog(){
    qCDebug(DLG) << Q_FUNC_INFO;
}

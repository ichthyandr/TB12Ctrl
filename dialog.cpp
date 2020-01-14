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
    QMenu *fileMenu = new QMenu("File");
    menuBar->addMenu(fileMenu);
    m_loadAct = fileMenu->addAction(QIcon(":/images/open_16.png"),tr("Load"));
    m_saveAct = fileMenu->addAction(QIcon(":/images/save_16.png"),tr("Save"));
    fileMenu->addSeparator();
    auto exitAct = fileMenu->addAction(QIcon(":/images/exit_16.png"),tr("Exit"));

    m_saveAct->setDisabled( true );
    m_loadAct->setEnabled( true );

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
    qCDebug(DLG) << Q_FUNC_INFO;
    if ( Q_NULLPTR != m_saveAct ) m_saveAct->setEnabled( enable );
}

void Dialog::enableLoad( bool enable ){
    qCDebug(DLG) << Q_FUNC_INFO;
    if ( Q_NULLPTR != m_loadAct ) m_loadAct->setEnabled( enable );
}

Dialog::~Dialog(){
    qCDebug(DLG) << Q_FUNC_INFO;
}

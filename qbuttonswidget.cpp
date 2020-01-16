#include "qbuttonswidget.h"
#include "qtbbutton.h"
#include <QVBoxLayout>
Q_LOGGING_CATEGORY( BTN, "BTN" )

static QButtonsWidget * g_bw{Q_NULLPTR};

QButtonsWidget::QButtonsWidget(QWidget *parent) : QWidget(parent){
    qCDebug(BTN) << Q_FUNC_INFO;

    g_bw = this;

    auto mainLayout = new QVBoxLayout();
    setLayout( mainLayout );

    auto grid = new QGridLayout();
    mainLayout->addLayout( grid );

    auto pb1 = new QTBbutton( 1 );
    auto pb2 = new QTBbutton( 2 );
    auto pb3 = new QTBbutton( 3 );
    auto pb4 = new QTBbutton( 4 );
    auto pb5 = new QTBbutton( 5 );
    auto pb6 = new QTBbutton( 6 );
    auto pb7 = new QTBbutton( 7 );
    auto pb8 = new QTBbutton( 8 );
    auto pb9 = new QTBbutton( 9 );
    auto pb10= new QTBbutton(10 );
    auto pb11= new QTBbutton(11 );
    auto pb12= new QTBbutton(12 );

    grid->addWidget( pb1, 0, 0 );
    grid->addWidget( pb2, 0, 1 );
    grid->addWidget( pb3, 0, 2 );
    grid->addWidget( pb4, 0, 3 );

    grid->addWidget( pb5, 1, 0 );
    grid->addWidget( pb6, 1, 1 );
    grid->addWidget( pb7, 1, 2 );
    grid->addWidget( pb8, 1, 3 );

    grid->addWidget( pb9, 2, 0 );
    grid->addWidget( pb10,2, 1 );
    grid->addWidget( pb11,2, 2 );
    grid->addWidget( pb12,2, 3 );

    connect( pb1, &QTBbutton::clicked, [=](){emit s_clickedBtn(1);} );
    connect( pb2, &QTBbutton::clicked, [=](){emit s_clickedBtn(2);} );
    connect( pb3, &QTBbutton::clicked, [=](){emit s_clickedBtn(3);} );
    connect( pb4, &QTBbutton::clicked, [=](){emit s_clickedBtn(4);} );
    connect( pb5, &QTBbutton::clicked, [=](){emit s_clickedBtn(5);} );
    connect( pb6, &QTBbutton::clicked, [=](){emit s_clickedBtn(6);} );
    connect( pb7, &QTBbutton::clicked, [=](){emit s_clickedBtn(7);} );
    connect( pb8, &QTBbutton::clicked, [=](){emit s_clickedBtn(8);} );
    connect( pb9, &QTBbutton::clicked, [=](){emit s_clickedBtn(9);} );
    connect( pb10, &QTBbutton::clicked,[=](){emit s_clickedBtn(10);} );
    connect( pb11, &QTBbutton::clicked,[=](){emit s_clickedBtn(11);} );
    connect( pb12, &QTBbutton::clicked,[=](){emit s_clickedBtn(12);} );

    m_pBtns << pb1 << pb2 << pb3 << pb4 << pb5 << pb6 << pb7 << pb8 << pb9 << pb10 << pb11 << pb12;
}

QButtonsWidget::~QButtonsWidget(){
    qCDebug(BTN) << Q_FUNC_INFO;
}

QButtonsWidget * QButtonsWidget::getInstance(){
    return(g_bw);
}

void QButtonsWidget::update( int bank ){
    for( auto p_btn: m_pBtns ){
        p_btn->update( bank );
    }
}

void QButtonsWidget::update( int button, int bank ){
    if ( (button-1)< m_pBtns.size() )
        m_pBtns.at(button)->update( bank );
    else
        qCDebug(BTN) << Q_FUNC_INFO << "bad button number!";
}

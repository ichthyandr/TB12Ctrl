#include "qmidiin.h"
#include "qmidisysexmsgs.h"
#include <QQueue>

Q_LOGGING_CATEGORY(IN, "IN")

namespace QIN {
///////////////////////////////////////////////////////////////
///
///                     W O R K E R
///
///////////////////////////////////////////////////////////////

QMidiIn::QMidiIn(QObject *parent): QObject(parent){
    qCDebug(IN) << Q_FUNC_INFO;
    try {
      m_midiIn  = new RtMidiIn();
      m_midiIn->setCallback(&QMidiIn::callback, this);
      m_midiIn->ignoreTypes	( false, true, true );
      m_state   = READY;
    }
    catch ( RtMidiError &error ) {
        m_lastErr = QString( error.what() );
        qCCritical(IN) << Q_FUNC_INFO << "m_lastErr=" << m_lastErr;
    }
}

QMidiIn::~QMidiIn(){
    qCDebug(IN) << Q_FUNC_INFO;
    if ( Q_NULLPTR != m_midiIn ){
        closePort();
        delete m_midiIn;
    }
}

void QMidiIn::receive( QVector<uchar> midiMsg ){
    qCDebug(IN) << Q_FUNC_INFO << hex << midiMsg;
    //if ( MIDI_SYSEX_START == midiMsg.at(0))
    if ( 0 != midiMsg.length()) emit s_receive( midiMsg );
}

void QMidiIn::callback(double deltatime, std::vector<unsigned char> *message, void *userData){

    Q_UNUSED(deltatime);

    if ( Q_NULLPTR == message ){
        qCCritical(IN) << "message is NULL";
        return;
    }
    if ( Q_NULLPTR == userData ){
        qCCritical(IN) << "userData is NULL";
        return;
    }

    QMidiIn* qmidiIn = static_cast<QMidiIn*>(userData);
    QVector<uchar> midiMsg = QVector<uchar>::fromStdVector(*message);

    qmidiIn->receive( midiMsg );
}

workerState_t QMidiIn::openPort(QString name){
    qCDebug(IN) << Q_FUNC_INFO << "state=" << m_state << " name=" << name;
    if ( READY == m_state ){
        unsigned int i      = 0;
        unsigned int amount = m_midiIn->getPortCount();
        while ( i < amount ){
            i++;
            if(name == QString::fromStdString(m_midiIn->getPortName(i))){
                m_midiIn->openPort(i);
                break;
            }
        }
        if ( m_midiIn->isPortOpen() ){
            qCDebug(IN) << Q_FUNC_INFO << "opening port done";
            m_state = WORK;
        }
        else {
            qCCritical(IN) << Q_FUNC_INFO << "can't open port with name=" << name;
            m_lastErr = QString( "can't open port with name=%1").arg( name );
        }
    }
    return(m_state);
}

workerState_t QMidiIn::openPort(unsigned int index){
    qCDebug(IN) << Q_FUNC_INFO << "state=" << m_state << " index=" << index;
    if ( READY == m_state ){
        m_midiIn->openPort(index);
        if (m_midiIn->isPortOpen()){
            m_state = WORK;
            qCDebug(IN) << Q_FUNC_INFO << "opening port done";
        }
        else{
            qCCritical(IN) << Q_FUNC_INFO << "can't open port with index=" << index;
            m_lastErr = QString( "can't open port with index=%1").arg( index );
        }
    }
    return(m_state);
}

workerState_t QMidiIn::openVirtualPort(QString name){
    qCDebug(IN) << Q_FUNC_INFO << "state=" << m_state << " name=" << name;
    if ( READY == m_state ){
        m_midiIn->openVirtualPort(name.toStdString());
        m_state = WORK;
    }
    return(m_state);
}

void QMidiIn::closePort(){
    qCDebug(IN) << Q_FUNC_INFO << "state=" << m_state;
    if ( WORK == m_state ){
        m_midiIn->closePort();
        if (!m_midiIn->isPortOpen()){
            m_state = READY;
        }
        else {
            qCCritical(IN) << Q_FUNC_INFO << "can't close port";
        }
    }
}

QString QMidiIn::getLastError(){
    qCDebug(IN) << Q_FUNC_INFO << " m_lastError=" << m_lastErr;
    return (m_lastErr);
}
/*
workerState_t QMidiIn::returnState(){
    qCDebug(IN) << Q_FUNC_INFO << "state=" << m_state << " error=" << m_lastErr;
    return (m_state);
}

QString QMidiIn::returnError(){
    qCDebug(IN) << Q_FUNC_INFO << "state=" << m_state << " error=" << m_lastErr;
    return(m_lastErr);
}
*/


///////////////////////////////////////////////////////////////
///
///                  C O N T R O L L E R
///
///////////////////////////////////////////////////////////////
Controller::Controller( QObject *parent ):QObject(parent), m_enable(false), m_thread(){
    qCDebug(IN) << Q_FUNC_INFO;
}

Controller::~Controller(){
    qCDebug(IN) << Q_FUNC_INFO;
    off();
}

void Controller::off(){
    qCDebug(IN) << Q_FUNC_INFO << " thread ID=" << QThread::currentThreadId();
    if ( m_enable ){
        m_thread.quit();
        m_thread.wait();
        m_enable        = false;
        m_workerState   = FAILURE;
        m_workerLastErr = QString("");
    }
}

QStringList Controller::getPorts(){
    qCDebug(IN) << Q_FUNC_INFO;
    QStringList ports;
    RtMidiIn * _midiIn = Q_NULLPTR;

    try {
      _midiIn  = new RtMidiIn();
    }
    catch ( RtMidiError &error ) {
        QString err = QString( error.what() );
        qCCritical(IN) << Q_FUNC_INFO << err;
        if ( Q_NULLPTR != _midiIn ) delete _midiIn;
        return(ports);
    }
    qCDebug(IN) <<  Q_FUNC_INFO << "ports count=" << _midiIn->getPortCount();
    for(unsigned int i = 0; i < _midiIn->getPortCount(); i++){
        ports.append(QString::fromStdString(_midiIn->getPortName(i)));
    }
    delete _midiIn;
    return ports;
}

workerState_t Controller::on( const unsigned int index ){
    qCDebug(IN) << Q_FUNC_INFO << " thread ID=" << QThread::currentThreadId() << " port index=" << index;
    if ( !m_enable ){
        m_enable    = true;
        m_qmidiin = new QMidiIn();
        m_workerState   = m_qmidiin->openPort( index );

        if ( WORK != m_workerState ){
            m_workerLastErr = m_qmidiin->getLastError();
            m_enable = false;
            delete m_qmidiin;
            m_qmidiin = Q_NULLPTR;
            qCCritical( IN ) << Q_FUNC_INFO << "can't open midi port";
            return (m_workerState);
        }

        m_qmidiin->moveToThread(&m_thread);

        qRegisterMetaType<QVector<uchar>>();

        connect(&m_thread,  &QThread::finished,     m_qmidiin,  &QObject::deleteLater );
        connect(m_qmidiin,  &QMidiIn::s_receive,    this,       &Controller::_receive );

        m_thread.start();
        do {
            QThread::msleep(1);
            QThread::yieldCurrentThread();

        } while ( !m_thread.isRunning() );
    }
    return(m_workerState);
}

workerState_t Controller::on( const QString & portName ){
    qCDebug(IN) << Q_FUNC_INFO << " thread ID=" << QThread::currentThreadId() << " portName=" << portName;
    if ( !m_enable ){
        m_enable    = true;
        m_qmidiin = new QMidiIn();
        m_workerState   = m_qmidiin->openPort( portName );

        if ( WORK != m_workerState ){
            m_workerLastErr = m_qmidiin->getLastError();
            m_enable = false;
            delete m_qmidiin;
            m_qmidiin = Q_NULLPTR;
            qCCritical( IN ) << Q_FUNC_INFO << "can't open midi port";
            return (m_workerState);
        }

        m_qmidiin->moveToThread(&m_thread);

        qRegisterMetaType<QVector<uchar>>();

        connect(&m_thread,  &QThread::finished,     m_qmidiin,  &QObject::deleteLater );
        connect(m_qmidiin,  &QMidiIn::s_receive,    this,       &Controller::_receive );

        m_thread.start();
        do {
            QThread::msleep(1);
            QThread::yieldCurrentThread();

        } while ( !m_thread.isRunning() );
    }
    return(m_workerState);
}

void Controller::_enableACK(){
    qCDebug(IN) << Q_FUNC_INFO;
    m_enableACK = true;
}

void Controller::_receive( QVector<uchar> midiMsg ){
    qCDebug(IN) << Q_FUNC_INFO << "m_enable=" << m_enable << "midiMsg=" << hex << midiMsg;

    if (!m_enable) return;

    uint8_t code = SSXMSGS::decode( midiMsg );
    switch (code){
    case GLOBALS_MSG:
        qCDebug(IN) << Q_FUNC_INFO << "GLOBALS_MSG decoded!";
        // выдать сигнал
        emit s_msg();
        break;
    case BANKS_MSG:
        qCDebug(IN) << Q_FUNC_INFO << "BANKS_MSG decoded!";
        // выдать сигнал
        emit s_msg();
        break;
    case ACK_MSG:
#if 0
        qCDebug(IN) << Q_FUNC_INFO << "ACK_MSG decoded!";
        // выдать сигнал
        emit s_ack();
#endif
        if ( m_enableACK ){
            qCDebug(IN) << Q_FUNC_INFO << "ACK_MSG decoded! m_enableACK=" << m_enableACK;
            // запретить обработку
            m_enableACK = false;
            // выдать сигнал
            emit s_ack();
        }
        else {
            qCDebug(IN) << Q_FUNC_INFO << "ACK_MSG decoded! m_enableACK=" << m_enableACK << "DROP ACK!!!!!";
        }
        break;
    case EOT_MSG:
        qCDebug(IN) << Q_FUNC_INFO << "EOT_MSG decoded!";
        // выдать сигнал
        emit s_eot();
        break;
    default:
        qCCritical(IN) << Q_FUNC_INFO << "can't decode message " << hex << midiMsg;
    }
}

bool Controller::isEnabled() const {
    qCDebug(IN) << Q_FUNC_INFO << " m_enable=" << m_enable;
    return( m_enable );
}

workerState_t Controller::getWorkerState(){
    qCDebug(IN) << Q_FUNC_INFO << "m_workerState=" << m_workerState;
    return( m_workerState );
}

QString Controller::getWorkerError(){
    qCDebug(IN) << Q_FUNC_INFO << "m_workerLastErr=" << m_workerLastErr;
    return( m_workerLastErr );
}
}//namespace QIN

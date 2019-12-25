#include "qmidiout.h"

Q_LOGGING_CATEGORY(OUT, "OUT")

namespace QOUT{
///////////////////////////////////////////////////////////////
///
///                     W O R K E R
///
///////////////////////////////////////////////////////////////

QMidiOut::QMidiOut(QObject *parent): QObject(parent){
    qCDebug(OUT) << Q_FUNC_INFO;
    try {
      m_midiOut  = new RtMidiOut();
      m_state   = READY;
    }
    catch ( RtMidiError &error ) {
        m_lastErr = QString( error.what() );
        qCCritical(OUT) << Q_FUNC_INFO << "m_lastErr=" << m_lastErr;
    }
}

QMidiOut::~QMidiOut(){
    qCDebug(OUT) << Q_FUNC_INFO;
    if ( Q_NULLPTR != m_midiOut ){
        closePort();
        delete m_midiOut;
    }
}

workerState_t QMidiOut::openPort(unsigned int index){
    qCDebug(OUT) << Q_FUNC_INFO << "state=" << m_state << " index=" << index;
    if ( READY == m_state ){
        m_midiOut->openPort(index);
        if (m_midiOut->isPortOpen()){
            m_state = WORK;
            qCDebug(OUT) << Q_FUNC_INFO << "opening port done";
        }
        else{
            qCCritical(OUT) << Q_FUNC_INFO << "can't open port with index=" << index;
            m_lastErr = QString( "can't open port with index=%1").arg( index );
        }
    }
    return(m_state);
}

workerState_t QMidiOut::openPort(QString name){
    qCDebug(OUT) << Q_FUNC_INFO << "state=" << m_state << " name=" << name;
    if ( READY == m_state ){
        unsigned int i      = 0;
        unsigned int amount = m_midiOut->getPortCount();
        while ( i < amount ){
            i++;
            if(name == QString::fromStdString(m_midiOut->getPortName(i))){
                m_midiOut->openPort(i);
                break;
            }
        }
        if ( m_midiOut->isPortOpen() ){
            qCDebug(OUT) << Q_FUNC_INFO << "opening port done";
            m_state = WORK;
        }
        else {
            qCCritical(OUT) << Q_FUNC_INFO << "can't open port with name=" << name;
            m_lastErr = QString( "can't open port with name=%1").arg( name );
        }
    }
    return(m_state);
}

workerState_t QMidiOut::openVirtualPort(QString name){
    qCDebug(OUT) << Q_FUNC_INFO << "state=" << m_state << " name=" << name;
    if ( READY == m_state ){
        m_midiOut->openVirtualPort(name.toStdString());
        m_state = WORK;
    }
    return(m_state);
}

void QMidiOut::closePort(){
    qCDebug(OUT) << Q_FUNC_INFO << "state=" << m_state;
    if ( WORK == m_state ){
        m_midiOut->closePort();
        if (!m_midiOut->isPortOpen()){
            m_state = READY;
        }
        else {
            qCCritical(OUT) << Q_FUNC_INFO << "can't close port";
        }
    }
}

void QMidiOut::_send(midiMsg_t midiMsg ){
    qCDebug(OUT) << Q_FUNC_INFO << "state=" << m_state << " midiMsg=" << hex << midiMsg;
    if ( WORK == m_state ){
/*
        while( !m_gotACK ){
            QThread::msleep(1);
            QThread::yieldCurrentThread();
        }
        m_gotACK = false;
        std::vector<unsigned char> message = midiMsg.toStdVector();
        if (message.size()>0){
            m_midiOut->sendMessage( &message );
        }
        */
        m_midiMsgQueue.enqueue(midiMsg);
        qCDebug(OUT) << Q_FUNC_INFO << "state=" << m_state << "enqueue midiMsg=" << hex << midiMsg;
    }
    else
        qCCritical(OUT) << Q_FUNC_INFO << "bad worker state=" << m_state;

}

QString QMidiOut::getLastError(){
    qCDebug(OUT) << Q_FUNC_INFO << " m_lastError=" << m_lastErr;
    return (m_lastErr);
}

void QMidiOut::_ack(){
    qCDebug(OUT) << Q_FUNC_INFO;
    if ( !m_midiMsgQueue.isEmpty() ){
        midiMsg_t midiMsg = m_midiMsgQueue.dequeue();
        std::vector<unsigned char> message = midiMsg.toStdVector();
        if (message.size()>0){
            m_midiOut->sendMessage( &message );
            qCDebug(OUT) << Q_FUNC_INFO << "send mdiMsg=" << hex << midiMsg;
            emit s_sent();
        }
    }
    if (m_midiMsgQueue.isEmpty()){
        qCDebug(OUT) << Q_FUNC_INFO << "end of transmission";
        emit s_end();
    }
}

///////////////////////////////////////////////////////////////
///
///                  C O N T R O L L E R
///
///////////////////////////////////////////////////////////////

Controller::Controller( QObject *parent ):QObject(parent), m_enable(false), m_thread(){
    qCDebug(OUT) << Q_FUNC_INFO;
}

Controller::~Controller(){
    qCDebug(OUT) << Q_FUNC_INFO;
    off();
}

workerState_t Controller::on( const unsigned int index ){
    qCDebug(OUT) << Q_FUNC_INFO << " thread ID=" << QThread::currentThreadId() << " index=" << index;
    if ( !m_enable ){
        m_enable        = true;
        m_qmidiout      = new QMidiOut();
        m_workerState   = m_qmidiout->openPort( index );

        if ( WORK != m_workerState ){
            m_workerLastErr = m_qmidiout->getLastError();
            m_enable = false;
            delete m_qmidiout;
            m_qmidiout = Q_NULLPTR;
            qCCritical( OUT ) << Q_FUNC_INFO << "can't open midi port";
            return (m_workerState);
        }

        m_qmidiout->moveToThread(&m_thread);

        qRegisterMetaType<QVector<uchar>>();

        connect(&m_thread,  &QThread::finished,     m_qmidiout,     &QObject::deleteLater );
        connect(this,       &Controller::s_send,    m_qmidiout,     &QMidiOut::_send);
        connect(this,       &Controller::s_ack,     m_qmidiout,     &QMidiOut::_ack);

        connect(m_qmidiout, &QMidiOut::s_sent,      this,           &Controller::_sent);
        connect(m_qmidiout, &QMidiOut::s_end,       this,           &Controller::_end);

        m_thread.start();
        do {
            QThread::msleep(1);
            QThread::yieldCurrentThread();

        } while ( !m_thread.isRunning() );
    }
    return( m_workerState );
}

workerState_t Controller::on( const QString & portName ){
    qCDebug(OUT) << Q_FUNC_INFO << " thread ID=" << QThread::currentThreadId() << " portName=" << portName;
    if ( !m_enable ){
        m_enable        = true;
        m_qmidiout      = new QMidiOut();
        m_workerState   = m_qmidiout->openPort( portName );

        if ( WORK != m_workerState ){
            m_workerLastErr = m_qmidiout->getLastError();
            m_enable = false;
            delete m_qmidiout;
            m_qmidiout = Q_NULLPTR;
            qCCritical( OUT ) << Q_FUNC_INFO << "can't open midi port";
            return (m_workerState);
        }

        m_qmidiout->moveToThread(&m_thread);

        qRegisterMetaType<QVector<uchar>>();

        connect(&m_thread,  &QThread::finished,     m_qmidiout,     &QObject::deleteLater );
        connect(this,       &Controller::s_send,    m_qmidiout,     &QMidiOut::_send);
        connect(this,       &Controller::s_ack,     m_qmidiout,     &QMidiOut::_ack);

        connect(m_qmidiout, &QMidiOut::s_sent,      this,           &Controller::_sent);
        connect(m_qmidiout, &QMidiOut::s_end,       this,           &Controller::_end);

        m_thread.start();
        do {
            QThread::msleep(1);
            QThread::yieldCurrentThread();

        } while ( !m_thread.isRunning() );
    }
    return( m_workerState );
}

void Controller::off(){
    qCDebug(OUT) << Q_FUNC_INFO << " thread ID=" << QThread::currentThreadId();
    if ( m_enable ){
        m_thread.quit();
        m_thread.wait();
        m_enable        = false;
        m_workerState   = FAILURE;
        m_workerLastErr = QString("");
    }
}

QStringList Controller::getPorts(){
    qCDebug(OUT) << Q_FUNC_INFO;
    QStringList ports;
    RtMidiOut * _midiOut = Q_NULLPTR;

    try {
        _midiOut  = new RtMidiOut();
    }
    catch ( RtMidiError &error ) {
        QString err = QString( error.what() );
        qCCritical(OUT) << Q_FUNC_INFO << err;
        if ( Q_NULLPTR != _midiOut ) delete _midiOut;
        return( ports );
    }
    qCDebug(OUT) << Q_FUNC_INFO << "out ports=" << _midiOut->getPortCount();
    for(unsigned int i = 0; i < _midiOut->getPortCount(); i++){
        ports.append(QString::fromStdString(_midiOut->getPortName(i)));
    }
    delete _midiOut;
    return(ports);
}

bool Controller::isEnabled() const {
    qCDebug(OUT) << Q_FUNC_INFO << " m_enable=" << m_enable;
    return( m_enable );
}

void Controller::send(midiMsg_t midiMsg){
    qCDebug(OUT) << Q_FUNC_INFO << " m_enable=" << m_enable << " midiMsg=" << midiMsg;
    if ( m_enable ){
        emit s_send(midiMsg);
    }
    else {
        qCCritical(OUT) << Q_FUNC_INFO << "Controller is not enabled!";
    }
}

void Controller::send(){
    qCDebug(OUT) << Q_FUNC_INFO << " m_enable=" << m_enable;
    if ( m_enable ){
        QVector<midiMsg_t> midiMsgList = SSXMSGS::encode();
        for( auto midiMsg: midiMsgList ){
            emit s_send(midiMsg);
        }
        /// первый ack отсюда, чтобы запусть передачу
        emit s_ack();
    }
    else {
        qCCritical(OUT) << Q_FUNC_INFO << "Controller is not enabled!";
    }
}

void Controller::sendBank(int number){
    qCDebug(OUT) << Q_FUNC_INFO << " m_enable=" << m_enable << " number=" << number;
    if ( m_enable ){
        QVector<midiMsg_t> midiMsgList = SSXMSGS::encodeBank( number );
        for( auto midiMsg: midiMsgList ){
            emit s_send(midiMsg);
        }
        /// первый ack отсюда, чтобы запусть передачу
        emit s_ack();
    }
    else {
        qCCritical(OUT) << Q_FUNC_INFO << "Controller is not enabled!";
    }
}

void Controller::sendBySequence( const QList<int> sequence ){
    qCDebug(OUT) << Q_FUNC_INFO << "sequence=" << sequence;
    if ( sequence.isEmpty() ){
        qCDebug(OUT) << Q_FUNC_INFO << "sequence is empty";
        return;
    }
    if ( m_enable ){
        QVector<midiMsg_t> midiMsgList;
        for( int number: sequence ){
            if ( -1 == number ){
                // send globals
                midiMsgList << SSXMSGS::encodeGlobals();
            }
            else if ( number < 128 ){
                // send bank
                midiMsgList << SSXMSGS::encodeBank( number );
            }
        }
        for( auto midiMsg: midiMsgList ){
            emit s_send(midiMsg);
        }
        /// первый ack отсюда, чтобы запусть передачу
        emit s_ack();
    }
    else {
        qCCritical(OUT) << Q_FUNC_INFO << "Controller is not enabled!";
    }
}

workerState_t Controller::getWorkerState(){
    qCDebug(OUT) << Q_FUNC_INFO << "m_workerState=" << m_workerState;
    return( m_workerState );
}

QString Controller::getWorkerError(){
    qCDebug(OUT) << Q_FUNC_INFO << "m_workerLastErr=" << m_workerLastErr;
    return( m_workerLastErr );
}

void Controller::_ack(){
    qCDebug(OUT) << Q_FUNC_INFO;
    emit s_ack();
}

void Controller::_sent(){
    qCDebug(OUT) << Q_FUNC_INFO;
    emit s_sent();
    emit s_enableACK();
}

void Controller::_end(){
    qCDebug(OUT) << Q_FUNC_INFO;
    emit s_end();
}


}//namespace QOUT

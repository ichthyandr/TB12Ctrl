#ifndef QMIDIOUT_H
#define QMIDIOUT_H
#include <QObject>
#include <QString>
#include <QThread>
#include <QQueue>
#include <QList>
#include "rtmidi-4.0.0/RtMidi.h"
#include <QLoggingCategory>
#include "qmidisysexmsgs.h"
Q_DECLARE_LOGGING_CATEGORY(OUT)


namespace QOUT{

typedef enum {
    FAILURE = 0,
    READY,
    WORK
} workerState_t;

/// класс воркера
class QMidiOut : public QObject{
    Q_OBJECT
public:
    /// конструктор
    QMidiOut(QObject *parent = Q_NULLPTR);
    /// деструктор
    ~QMidiOut();
    /// открыть порт
    /// \param index[in] - номер порта
    workerState_t openPort(unsigned int index);
    /// открыть порт
    /// \param name[in] - имя порта
    workerState_t openPort(QString name);
    /// открыть виртуальный порт
    /// \param name[in] - имя порта
    workerState_t openVirtualPort(QString name);
    /// вернуть текст ошибки
    /// \return текст ошибки
    QString getLastError();
private:
    /// хендлер
    RtMidiOut *     m_midiOut{Q_NULLPTR};
    /// состояние класса
    workerState_t   m_state{FAILURE};
    /// последняя ошибка
    QString         m_lastErr{""};
private:
    /// закрыть порт
    void closePort();
    /// буфер сообщений на отправку
    QQueue<midiMsg_t> m_midiMsgQueue;
public slots:
    /// передать сообщение
    /// \param midiMsg[in] - midiSysEx сообщение
    void _send( midiMsg_t midiMsg );
    /// обработать сигнал контроллера s_ack
    void _ack();
signals:
    /// сигнал контроллеру, сообщение передано
    void s_sent();
    /// сигнал контроллеру конец передачи
    void s_end();
};


/// класс контроллера
class Controller: public QObject {
    Q_OBJECT
private:
    /// признак работы
    bool            m_enable{false};
    /// рабочий поток
    QThread         m_thread;
    /// адрес воркера
    QMidiOut *      m_qmidiout{Q_NULLPTR};
    /// состояние воркера
    workerState_t   m_workerState{FAILURE};
    /// последняя ошибка воркера
    QString         m_workerLastErr{""};

signals:
    /// сигнал воркеру на передачу
    void s_send(QVector<uchar> midiMsg);
    /// сигнал воркеру, что ack принят
    void s_ack();
    /// сигнал в gui - сообщение передано
    void s_sent();
    /// сигнал в gui - все сообщения переданы
    void s_end();
public:
    /// конструктор класса
    ///\public
    Controller( QObject *parent = Q_NULLPTR );
    /// деструктор класса
    ///\public
    virtual ~Controller();
    /// запустить управление
    ///\param portName  - имя порта
    ///\return состояние воркера
    workerState_t on( const QString & portName );
    /// запустить управление
    ///\param index[in]  - индекс порта
    ///\return состояние воркера
    workerState_t on( const unsigned int index );
    /// отключить управление
    void off();
    /// вернуть список портов
    /// \return список портов
    static QStringList getPorts();
    /// \brief возвращает состояние контроллера
    /// \return true - в работе
    bool isEnabled() const;
    /// передать сообщение
    ///\param midiMsg[in] - midiSysEx сообщение
    void send(midiMsg_t midiMsg);
    /// передать все настройки в виде кодированных сообщений
    void send();
    /// передать настройки банка на контроллер
    /// \param number[in] - номер банка
    void sendBank(int number);
    /// передать настройки в контроллер согласно секвенции:
    /// -1                              = передать глобальные настройки
    /// от 0 включительно и менее 128   = передать банк с номером
    void sendBySequence( const QList<int> sequence );
    /// вернуть состояние воркера
    workerState_t getWorkerState();
    /// вернуть текст последней ошибки воркера
    QString getWorkerError();
public slots:
    /// принять сигнал s_ack от qmidiin
    void _ack();
    /// принять сигнал s_sent от воркера
    void _sent();
    /// принять сигнал s_end от воркера
    void _end();
signals:
    /// подать сигнал в qmidiin о разрешении обработки ACK после передачи сообщения qmidiout
    /// фикс: при частом включении/выключении qmidiin/qmidiout с контроллера проскакивают ACK
    void s_enableACK();
};
}
//Q_DECLARE_METATYPE(QVector<uchar>)

#endif // QMIDIOUT_H

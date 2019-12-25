#ifndef QMIDIIN_H
#define QMIDIIN_H
#include <QThread>
#include <QObject>
#include <QVector>
#include <QString>
#include "rtmidi-4.0.0/RtMidi.h"
#include <QLoggingCategory>
#include "qmidisysexmsgs.h"
Q_DECLARE_LOGGING_CATEGORY(IN)

namespace QIN{

typedef enum {
    FAILURE = 0,
    READY,
    WORK
} workerState_t;

/// класс чтения входящих сообщений, worker
class QMidiIn : public QObject {
    Q_OBJECT
public:
    /// конструктор
    explicit QMidiIn(QObject *parent = Q_NULLPTR);
    /// деструктор
    ~QMidiIn();
    /// принять сообщение
    /// \param midiMsg[in] - входящее сообщение
    void receive( QVector<uchar> midiMsg );
    /// открыть порт
    /// \param name[in] - имя порта
    workerState_t openPort(QString name);
    /// открыть порт
    /// \param index[in] - индекс порта
    workerState_t openPort(unsigned int index);
    /// открыть виртуальный порт
    /// \param name[in] - имя порта
    workerState_t openVirtualPort(QString name);
    /// вернуть текст ошибки
    /// \return текст ошибки
    QString getLastError();
private:
    /// хендлер класса RtMidiIn
    RtMidiIn *      m_midiIn{Q_NULLPTR};
    /// сообщение о последней ошибке
    QString         m_lastErr{""};
    /// состояние класса
    workerState_t   m_state{FAILURE};
    /// callback for RtMidiIn
    /// \param deltatime[in]    - дельта времени
    /// \param message[in]      - принятое сообщение
    /// \param userData[in]     - адрес передаваемых данных
    static void callback( double deltatime, std::vector< unsigned char > *message, void *userData );
    /// закрыть порт
    void closePort();
signals:
    /// передать принятое сообщение
    void s_receive( QVector<uchar> midiMsg );
};

/// контроллер
class Controller: public QObject {
    Q_OBJECT
private:
    /// признак работы
    bool            m_enable{false};
    /// рабочий поток
    QThread         m_thread;
    /// адрес воркера
    QMidiIn *       m_qmidiin{Q_NULLPTR};
    /// состояние воркера
    workerState_t   m_workerState{FAILURE};
    /// текст последней ошибки воркера
    QString         m_workerLastErr{""};
    /// разрешение на обработку ACK
    bool            m_enableACK{false};
signals:
    /// сигнал принятого ACK_MSG
    void s_ack();
    /// сигнал принятого EOT_MSG
    void s_eot();
    /// сигнал принятого сообщения GLOBALS_MSG или BANKS_MSG
    void s_msg();
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
    static QStringList getPorts();
    /// \brief возвращает состояние контроллера
    /// \return true - в работе
    bool isEnabled() const;
    /// вернуть состояние воркера
    workerState_t getWorkerState();
    /// вернуть последнюю ошибку воркера
    QString getWorkerError();
public slots:
    /// получить сообщение
    void _receive( QVector<uchar> midiMsg );
    /// получить сообщение от qmidiout на разрешение обработки ACK
    void _enableACK();
};

}

Q_DECLARE_METATYPE(QVector<uchar>)

#endif // QMIDIIN_H

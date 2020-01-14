#ifndef QHOMEWIDGET_H
#define QHOMEWIDGET_H

#include <QWidget>
#include <QSet>
#include <QLoggingCategory>
Q_DECLARE_LOGGING_CATEGORY(CMN)

#include "qmidi/qmidiin.h"
#include "qmidi/qmidiout.h"
#include "qindicator.h"

class QButtonsWidget;
/// закладка home
class QHomeWidget : public QWidget
{
    Q_OBJECT
public:
    /// конструктор
    explicit QHomeWidget(QWidget *parent = Q_NULLPTR);
    /// деструктор
    ~QHomeWidget();
    /// get instance
    static QHomeWidget * getInstance();
    /// загрузить файл данных
    /// \param fileName[in] - file name
    /// \param err[out] - error string
    /// \return true - if success
    bool loadFile( QString fileName, QString & err );
    /// сохранить файл данных
    /// \param fileName[in] - file name
    /// \param err[out] - error string
    /// \return true - if success
    bool saveFile( QString fileName, QString & err );
private:
    /// набор команд:
    /// -1      изменены Globals settings
    /// 0 .. N  изменены Bank settings #0 .. N
    QSet<int> m_cmdSet;
    /// input MIDI port number
    int m_inPortIndex{-1};
    /// output MIDI port number
    int m_outPortIndex{-1};
    /// true, if MIDIin & MIDIout connected
    bool m_Connected{false};
    /// true, if data loaded;
    bool m_dataLoaded{false};
    /// признак передачи
    bool m_transmission{false};
    /// хендлер QMidiIn
    QIN::Controller     m_qmidiin;
    /// хендлер QMidiOut
    QOUT::Controller    m_qmidiout;
    /// индикатор приема
    QIndicator          m_rxIndicator;
    /// индикатор передачи
    QIndicator          m_txIndicator;
    /// обновить заголовок окна и кнопку sendAll:
    /// если m_cmdSet содержит команды, то
    /// вывести в заголовок [config changed]
    /// и кнопку sendAll разрешить
    void updTitle();
public slots:
    /// слот отслеживания изменений
    ///\param number[in] - номер измененного банка или -1 если изменены globals
    void _changed( int number );
private slots:
    /// загрузить имена банков в комбо
    void _loadNames();
};

#endif // QHOMEWIDGET_H

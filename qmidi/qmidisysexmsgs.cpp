/*utf8*/
#include "qmidisysexmsgs.h"
Q_LOGGING_CATEGORY(SSX,  "SSX")

namespace SSXMSGS {

// byte[in]	- input byte
// return	- encoded sequence 2 bytes
static
QVector<uint8_t> encodeByte( const uint8_t byte ){
    QVector<uint8_t> data;
    data.push_back((byte >> 4 ) & 0x0F);
    data.push_back(byte & 0x0F);
    return( data );
}

// Data[in] - sequence 2 bytes
// byte[out] - decoded byte
static
bool decodeByte( const QVector<uint8_t> & Data, uint8_t & byte ){
    if ( 2 != Data.size()) return (false);
    for( const uchar & _byte: Data ){
        if ( _byte > 0x7F ) return(false);
    }
    byte = ((Data.at(0)<<4)&0xF0)|(Data.at(1)&0x0F);
    return(true);
}

GlobalSettings          g_GlobalSettings;
QVector<BankSettings>   g_BanksSettings;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///									G L O B A L S		S E T T I N G S
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

GlobalSettings::GlobalSettings(){
    bnkNum              = 0;                    //Текущий номер банка. В настройках он меняется когда выбираем банк для сохранения/загрузки( Banks )
    midiChanNum         = 0;                    //System Setup -> MIDI channel. Диапазон от 0 до 15, на экране отображается от 1 до 16
    useBankSelectMess   = DONT_USE_BANK_SELECT; //System Setup -> Prg. ch. mode
    bankSelectMessType  = MSB;                  //System Setup -> Bnk. Sel mode
    bnkSwOnBoard        = NO_EXT_PEDAL;         //System Setup -> Bank sw. mode
    Show_pr_name        = PRESET_AND_BANK;      //System Setup -> Show pr. name
    targetDevice        = TARGET_DEVICE_AUTO;   //System Setup -> Target device
    usbBaudrate         = MIDI_BAUD;            //System Setup -> USB baudrate
    inputThrough[0]     = IN_TO_NONE;           //System Setup ->MIDI thru map
    inputThrough[1]     = IN_TO_NONE;
    maxBankNumber       = 0;                    //System Setup ->Max. bank.
    screenBrightness    = 10;                   //System Setup -> Screen brightness
    screenContrast      = 255;                  //System Setup ->Screen contrast


    buttonHoldTime      = ms1500;               //Exp&Tap&Tune -> BUT hold time. Диапазон от 1 до 15, в меню отобржажается как 100ms, 200ms, ..., 1500ms
    tapDisplayType      = ON_SCREEN;            //Exp&Tap&Tune -> Tap display
    tapType             = TAP_CONST_VALUE;      //Exp&Tap&Tune -> Tap type
    pedalLedView        = PEDAL_LED_MODE_DUAL;  //Pedal view -> Display type
    pedalTunerScheme    = Scheme1;
    pedalBrightness     = 5;


    for( int i = 0; i < 3; ++i)	{
        pLowPos[i]  = 0;
        pHighPos[i] = 127;
        expPtype[i] = LINEAL;
    }
}

GlobalSettings::GlobalSettings(const GlobalSettings &other){
    ::memcpy( this, &other, sizeof(other));
}

GlobalSettings::~GlobalSettings(){}


// IDs globals settings
#define GS_ID_bnkNum				0
#define GS_ID_midiChanNum			1
#define GS_ID_useBankSelectMess		2
#define GS_ID_bankSelectMessType	3
#define GS_ID_bnkSwOnBoard			4
#define GS_ID_showPresetBank		5
#define GS_ID_targetDevice			6
#define GS_ID_usbBaudrate			7
#define GS_ID_inputThrough			8
#define GS_ID_maxBankNumber			9
#define GS_ID_screenBrightness		10
#define GS_ID_screenContrast		11
#define GS_ID_expPedalType			12
#define GS_ID_buttonHoldTime		13
#define GS_ID_tapDisplayType		14
#define GS_ID_tapType				15
#define GS_ID_pedalLedView			16
#define GS_ID_pedalTunerScheme		17
#define GS_ID_pedalBrightness		18
#define GS_ID_pedalsCalibrationLo	19
#define GS_ID_pedalsCalibrationHi	20

static
uint8_t * getDataAndLenghtGS(const int paramID, size_t & InputDataLen){
    void * addr= Q_NULLPTR;
    switch ( paramID ){
        case GS_ID_bnkNum:
            InputDataLen	= sizeof(g_GlobalSettings.bnkNum);
            addr            = &g_GlobalSettings.bnkNum;
            break;
        case GS_ID_midiChanNum:
            InputDataLen	= sizeof(g_GlobalSettings.midiChanNum);
            addr            = &g_GlobalSettings.midiChanNum;
            break;
        case GS_ID_useBankSelectMess:
            InputDataLen	= sizeof(g_GlobalSettings.useBankSelectMess);
            addr            = &g_GlobalSettings.useBankSelectMess;
            break;
        case GS_ID_bankSelectMessType:
            InputDataLen	= sizeof(g_GlobalSettings.bankSelectMessType);
            addr            = &g_GlobalSettings.bankSelectMessType;
            break;
        case GS_ID_bnkSwOnBoard:
            InputDataLen	= sizeof(g_GlobalSettings.bnkSwOnBoard);
            addr            = &g_GlobalSettings.bnkSwOnBoard;
            break;
        case GS_ID_showPresetBank:
            InputDataLen	= sizeof(g_GlobalSettings.Show_pr_name);
            addr            = &g_GlobalSettings.Show_pr_name;
            break;
        case GS_ID_targetDevice:
            InputDataLen	= sizeof(g_GlobalSettings.targetDevice);
            addr            = &g_GlobalSettings.targetDevice;
            break;
        case GS_ID_usbBaudrate:
            InputDataLen	= sizeof(g_GlobalSettings.usbBaudrate);
            addr            = &g_GlobalSettings.usbBaudrate;
            break;
        case GS_ID_inputThrough:
            InputDataLen	= sizeof(g_GlobalSettings.inputThrough);
            addr            = &g_GlobalSettings.inputThrough[0];
            break;
        case GS_ID_maxBankNumber:
            InputDataLen	= sizeof(g_GlobalSettings.maxBankNumber);
            addr            = &g_GlobalSettings.maxBankNumber;
            break;
        case GS_ID_screenBrightness:
            InputDataLen	= sizeof(g_GlobalSettings.screenBrightness);
            addr            = &g_GlobalSettings.screenBrightness;
            break;
        case GS_ID_screenContrast:
            InputDataLen	= sizeof(g_GlobalSettings.screenContrast);
            addr            = &g_GlobalSettings.screenContrast;
            break;
        case GS_ID_expPedalType:
            InputDataLen	= sizeof(g_GlobalSettings.expPtype);
            addr    		= &g_GlobalSettings.expPtype[0];
            break;
        case GS_ID_buttonHoldTime:
            InputDataLen	= sizeof(g_GlobalSettings.buttonHoldTime);
            addr    		= &g_GlobalSettings.buttonHoldTime;
            break;
        case GS_ID_tapDisplayType:
            InputDataLen	= sizeof(g_GlobalSettings.tapDisplayType);
            addr            = &g_GlobalSettings.tapDisplayType;
            break;
        case GS_ID_tapType:
            InputDataLen	= sizeof(g_GlobalSettings.tapType);
            addr            = &g_GlobalSettings.tapType;
            break;
        case GS_ID_pedalLedView:
            InputDataLen	= sizeof(g_GlobalSettings.pedalLedView);
            addr            = &g_GlobalSettings.pedalLedView;
            break;
        case GS_ID_pedalTunerScheme:
            InputDataLen	= sizeof(g_GlobalSettings.pedalTunerScheme);
            addr            = &g_GlobalSettings.pedalTunerScheme;
            break;
        case GS_ID_pedalBrightness:
            InputDataLen	= sizeof(g_GlobalSettings.pedalBrightness);
            addr            = &g_GlobalSettings.pedalBrightness;
            break;
        case GS_ID_pedalsCalibrationLo:
            InputDataLen	= sizeof(g_GlobalSettings.pLowPos);
            addr            = &g_GlobalSettings.pLowPos[0];
            break;
        case GS_ID_pedalsCalibrationHi:
            InputDataLen	= sizeof(g_GlobalSettings.pHighPos);
            addr            = &g_GlobalSettings.pHighPos[0];
            break;
    }
    uint8_t * res = static_cast<uint8_t*>(addr);
    return(res);
}

midiMsg_t encode_GS_param( const uint8_t paramID ){
    midiMsg_t midiMsg;
    /* заголовок */
    /*  № по порядку
        0 - номер устройства в сети
        1 - код модели устройства от производителя
        2 - признак глобальной настройки = 0
        3 - paramID
        4 - encoded data
    */
    /* хвост */
    size_t	inputDataLen	= 0;            //input data length in bytes
    uint8_t * pInputData    = Q_NULLPTR;	//input data address

    pInputData = getDataAndLenghtGS( paramID, inputDataLen );

    if ( Q_NULLPTR==pInputData ) return(midiMsg);

    midiMsg.push_back(MIDI_SYSEX_START);
    midiMsg.push_back((MANUFACTURER_ID>>16)&0xFF );
    midiMsg.push_back((MANUFACTURER_ID>>8)&0xFF );
    midiMsg.push_back( MANUFACTURER_ID&0xFF );

    midiMsg.push_back(NETWORK_NUMBER);
    midiMsg.push_back(MODEL_NUMBER);
    midiMsg.push_back(GLOBALS_MSG);
    midiMsg.push_back(paramID);

    for( size_t i=0; i < inputDataLen; i++ ){
        uint8_t byte = *(pInputData + i);
        midiMsg << encodeByte( byte );
    }

    midiMsg.push_back(MIDI_SYSEX_END);

    return(midiMsg);
}

bool decode_GS_param( const uint8_t paramID, midiMsg_t & payload ){

    //qCDebug(SSX) << Q_FUNC_INFO << "paramID=" << hex << paramID << "payload=" << hex << payload ;

    typedef enum {
        GLOB_SETS = 0,
        GS_ID,
        DATA
    } State_t;

    if ( 4 > payload.length() ) return(false);

    size_t      inputDataLen    = 0;            //input data length in bytes
    uint8_t *   pInputData      = Q_NULLPTR;	//input data address

    pInputData = getDataAndLenghtGS( paramID, inputDataLen );

    //qCDebug(SSX) << Q_FUNC_INFO << "pInputData=" << hex << pInputData << "inputDataLen=" << dec << inputDataLen;

    if ( Q_NULLPTR==pInputData ) return(false);

    int             counter = 0;
    State_t			state	= GLOB_SETS;
    bool			_do_	= true;

    while(1) {
        switch( state ){
            case GLOB_SETS:
                if ( GLOBALS_MSG != payload.at(counter) ) {
                    //qCDebug(SSX) << Q_FUNC_INFO << "bad GS_IS!";
                    return (false);
                }
                state = GS_ID;
                break;
            case GS_ID:
                if ( paramID != payload.at(counter) ) {
                    //qCDebug(SSX) << Q_FUNC_INFO << "bad paramID!";
                    return (false);
                }
                state = DATA;
                break;
            case DATA:
                _do_ = false;
                break;
        }
        if ( false == _do_ ) break;
        counter++;
        if ( counter >= payload.length() ) return (false);
    }

    // декодирование
    for ( size_t i=0; i<inputDataLen; i++ ){
        uint8_t             byte = 0;
        QVector<uint8_t>    pair;
        pair.push_back( payload.at(counter) );
        pair.push_back( payload.at(counter + 1) );
        if (!decodeByte( pair, byte )) return(false);
        *(pInputData + i) = byte;

        //qCDebug(SSX) << Q_FUNC_INFO << "decoded:" << hex << byte;

        counter+=2;
    }
    return (true);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///									B A N K			S E T T I N G S
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BankSettings::BankSettings(){

    strcpy_s(BankName, BANK_NAME_NMAX_SIZE, "Unnamed 1" );
    pedalsCc[0] = 1;
    pedalsCc[1] = 7;
    pedalsCc[2] = 11;
    pedalsCc[3] = 128;
    tapCc       = 30;
    tunerCc     = 31;

    for (uint8_t i = 0; i < FOOT_BUTTONS_NUM; ++i)
    {
        buttonType[i] = PRESET_CHANGE;//All buttons will preset switchers
        buttonContext[i].presetChangeContext.programsNumbers[0] = i;//preset numbers will match button number;
        buttonContext[i].presetChangeContext.programsNumbers[1] = i;
        buttonContext[i].presetChangeContext.programsNumbers[2] = i;
        buttonContext[i].presetChangeContext.programsNumbers[3] = i;
        //Send only first PC message
        buttonContext[i].presetChangeContext.midiChannelNumbers[0] = MIDI_CHANNEL_PRIMARY;
        buttonContext[i].presetChangeContext.midiChannelNumbers[1] = MIDI_CHANNEL_NOT_ACTIVE;
        buttonContext[i].presetChangeContext.midiChannelNumbers[2] = MIDI_CHANNEL_NOT_ACTIVE;
        buttonContext[i].presetChangeContext.midiChannelNumbers[3] = MIDI_CHANNEL_NOT_ACTIVE;


        /*buttonContext[i].presetChangeContext.iaState = i;*/

        //CC and NRpN context
        buttonContext[i].commonContext.contolAndNrpnChangeContext_.autoSendState        = 0;
        buttonContext[i].commonContext.contolAndNrpnChangeContext_.ctrlLsbNumber        = 26;
        buttonContext[i].commonContext.contolAndNrpnChangeContext_.ctrlMsbFreezeNumber  = 128;
        buttonContext[i].commonContext.contolAndNrpnChangeContext_.paramLsbOffValue     = 0x00;
        buttonContext[i].commonContext.contolAndNrpnChangeContext_.paramMsbOffValue     = 0x00;
        buttonContext[i].commonContext.contolAndNrpnChangeContext_.paramLsbOnValue      = 127;
        buttonContext[i].commonContext.contolAndNrpnChangeContext_.paramMsbOnValue      = 127;
    }
}

BankSettings::BankSettings(const BankSettings &other){
    ::memcpy( this, &other, sizeof(other));
}

BankSettings::~BankSettings(){}


/// IDs banks settings
/*
    Значения ParamID от 0 до (N<FOOT_BUTTONS_NUM)
    соответствуют номеру контекста кнопки для данного банка
*/

#define BS_ID_tapCC					123
#define BS_ID_pedalsCC				124
#define BS_ID_tunerCC				125
#define BS_ID_buttonType			126
#define BS_ID_bankName				127


static
uint8_t * getDataAndLenghtBS( const int paramID, size_t & InputDataLen, BankSettings & Bank ){
    void * addr= Q_NULLPTR;
    if ( paramID < FOOT_BUTTONS_NUM ){
        // передается контекс кнопки с № paramID = 0 ... (N <FOOT_BUTTONS_NUM)
        InputDataLen    = sizeof(Bank.buttonContext[paramID]);
        addr            = &Bank.buttonContext[paramID];
        uint8_t * res   = static_cast<uint8_t*>(addr);
        return(res);
    }
    switch ( paramID ){
        case BS_ID_tapCC:
            InputDataLen    = sizeof(Bank.tapCc);
            addr            = &Bank.tapCc;
            break;
        case BS_ID_pedalsCC:
            InputDataLen	= sizeof(Bank.pedalsCc);
            addr            = &Bank.pedalsCc[0];
            break;
        case BS_ID_tunerCC:
            InputDataLen	= sizeof(Bank.tunerCc);
            addr            = &(Bank.tunerCc);
            break;
        case BS_ID_buttonType:
            InputDataLen	= sizeof(Bank.buttonType);
            addr            = &(Bank.buttonType[0]);
            break;
        case BS_ID_bankName:
            InputDataLen	= sizeof(Bank.BankName);
            addr            = &(Bank.BankName[0]);
            break;
    }
    uint8_t * res = static_cast<uint8_t*>(addr);
    return(res);
}

midiMsg_t encode_BS_param( const uint8_t paramID, const uint8_t bankNumber, BankSettings& Bank ){
    midiMsg_t midiMsg;
    if ( 127 < bankNumber ) return(midiMsg);
    /* заголовок */
    /*  № по порядку
        0 - номер устройства в сети
        1 - код модели устройства от производителя
        2 - признак настройки банка = 1
        3 - номер банка bankNumber
        4 - ID= BS_ID_...
        ... data
    */
    /*хвост*/
    size_t	inputDataLen	= 0;            //input data length in bytes
    uint8_t * pInputData    = Q_NULLPTR;	//input data address

    pInputData = getDataAndLenghtBS( paramID, inputDataLen, Bank );

    if ( Q_NULLPTR==pInputData ) return(midiMsg);

    midiMsg.push_back(MIDI_SYSEX_START);
    midiMsg.push_back((MANUFACTURER_ID>>16)&0xFF );
    midiMsg.push_back((MANUFACTURER_ID>>8)&0xFF );
    midiMsg.push_back( MANUFACTURER_ID&0xFF );

    midiMsg.push_back(NETWORK_NUMBER);
    midiMsg.push_back(MODEL_NUMBER);
    midiMsg.push_back(BANKS_MSG);
    midiMsg.push_back(bankNumber);
    midiMsg.push_back(paramID);

    for( size_t i=0; i < inputDataLen; i++ ){
        uint8_t byte = *(pInputData + i);
        midiMsg << encodeByte( byte );
    }

    midiMsg.push_back(MIDI_SYSEX_END);

    return(midiMsg);
}

bool decode_BS_param( const uint8_t paramID, midiMsg_t & payload){
    typedef enum {
        BANK_SETS = 0,
        BANK_NUMBER,
        BS_ID,
        DATA
    } State_t;

    if (5 > payload.length()) return (false);

    int             counter     = 0;
    State_t			state       = BANK_SETS;
    bool			_do_        = true;
    uint8_t			bankNumber	= 0;

    while(1) {
        switch( state ){
            case BANK_SETS:
                if ( BANKS_MSG != payload.at(counter) ) return (false);
                state = BANK_NUMBER;
                break;
            case BANK_NUMBER:
                if ( 0x7F < payload.at(counter) ) return (false);
                state = BS_ID;
                bankNumber = payload.at(counter);
                if ( bankNumber > 127 ) return(false);
                break;
            case BS_ID:
                if ( paramID != payload.at(counter) ) return (false);
                state = DATA;
                break;
            case DATA:
                _do_ = false;
                break;

        }
        if ( false == _do_ ) break;
        counter++;
        if ( counter >= payload.length() ) return (false);
    }

    // нумерация банков с 0, добавляем банки если необходимо
    while( (bankNumber+1) > g_BanksSettings.size() ){
        BankSettings bank;
        g_BanksSettings << bank;
    }

    size_t	inputDataLen	= 0;            //input data length in bytes
    uint8_t * pInputData    = Q_NULLPTR;	//input data address

    pInputData = getDataAndLenghtBS( paramID, inputDataLen, g_BanksSettings[bankNumber] );

    if ( Q_NULLPTR==pInputData ) return(false);

    // декодирование
    for ( size_t i=0; i<inputDataLen; i++ ){
        uint8_t             byte = 0;
        QVector<uint8_t>    pair;
        pair.push_back( payload.at(counter) );
        pair.push_back( payload.at(counter + 1) );
        if (!decodeByte( pair, byte )) return(false);
        *(pInputData + i) = byte;
        counter+=2;
    }

    return (true);
}

void init(){
    qCDebug(SSX) << Q_FUNC_INFO;

    g_BanksSettings.clear();
    BankSettings bank;
    g_BanksSettings << bank;

    qCDebug(SSX) << "sizeof GlobalSettings = " << sizeof(GlobalSettings);
    qCDebug(SSX) << "sizeof UseBankSelectMess = " << sizeof(UseBankSelectMess);

    qCDebug(SSX) << "sizeof BankSettings = " << sizeof(BankSettings);
    qCDebug(SSX) << "sizeof ButtonContext= " << sizeof(ButtonContext);
    qCDebug(SSX) << "sizeof CommonContext = " << sizeof(CommonContext);
}

QVector<midiMsg_t> encode(){
    QVector<midiMsg_t> midiMsgList;

    midiMsgList << encode_GS_param( GS_ID_bnkNum );
    midiMsgList << encode_GS_param( GS_ID_midiChanNum );
    midiMsgList << encode_GS_param( GS_ID_useBankSelectMess );
    midiMsgList << encode_GS_param( GS_ID_bankSelectMessType );
    midiMsgList << encode_GS_param( GS_ID_bnkSwOnBoard );
    midiMsgList << encode_GS_param( GS_ID_showPresetBank );
    midiMsgList << encode_GS_param( GS_ID_targetDevice );
    midiMsgList << encode_GS_param( GS_ID_usbBaudrate );
    midiMsgList << encode_GS_param( GS_ID_inputThrough );
    midiMsgList << encode_GS_param( GS_ID_maxBankNumber );
    midiMsgList << encode_GS_param( GS_ID_screenBrightness );
    midiMsgList << encode_GS_param( GS_ID_screenContrast );
    midiMsgList << encode_GS_param( GS_ID_expPedalType );
    midiMsgList << encode_GS_param( GS_ID_buttonHoldTime );
    midiMsgList << encode_GS_param( GS_ID_tapDisplayType );
    midiMsgList << encode_GS_param( GS_ID_tapType );
    midiMsgList << encode_GS_param( GS_ID_pedalLedView );
    midiMsgList << encode_GS_param( GS_ID_pedalTunerScheme );
    midiMsgList << encode_GS_param( GS_ID_pedalBrightness );
    midiMsgList << encode_GS_param( GS_ID_pedalsCalibrationLo );
    midiMsgList << encode_GS_param( GS_ID_pedalsCalibrationHi );

    for( int i=0; i < g_BanksSettings.size(); i++ ){
        BankSettings bank = g_BanksSettings.at(i);

        // сохраняем контекс кнопки № 0 ... N
        for(uint8_t btnNum=0; btnNum<FOOT_BUTTONS_NUM; btnNum++)
            midiMsgList << encode_BS_param( btnNum, static_cast<uint8_t>(i), bank );

        midiMsgList << encode_BS_param( BS_ID_tapCC,        static_cast<uint8_t>(i), bank );
        midiMsgList << encode_BS_param( BS_ID_pedalsCC,     static_cast<uint8_t>(i), bank );
        midiMsgList << encode_BS_param( BS_ID_tunerCC,      static_cast<uint8_t>(i), bank );
        midiMsgList << encode_BS_param( BS_ID_buttonType,   static_cast<uint8_t>(i), bank );
        midiMsgList << encode_BS_param( BS_ID_bankName,     static_cast<uint8_t>(i), bank );
    }

    return(midiMsgList);
}

QVector<midiMsg_t> encodeGlobals(){
    QVector<midiMsg_t> midiMsgList;
    midiMsgList << encode_GS_param( GS_ID_bnkNum );
    midiMsgList << encode_GS_param( GS_ID_midiChanNum );
    midiMsgList << encode_GS_param( GS_ID_useBankSelectMess );
    midiMsgList << encode_GS_param( GS_ID_bankSelectMessType );
    midiMsgList << encode_GS_param( GS_ID_bnkSwOnBoard );
    midiMsgList << encode_GS_param( GS_ID_showPresetBank );
    midiMsgList << encode_GS_param( GS_ID_targetDevice );
    midiMsgList << encode_GS_param( GS_ID_usbBaudrate );
    midiMsgList << encode_GS_param( GS_ID_inputThrough );
    midiMsgList << encode_GS_param( GS_ID_maxBankNumber );
    midiMsgList << encode_GS_param( GS_ID_screenBrightness );
    midiMsgList << encode_GS_param( GS_ID_screenContrast );
    midiMsgList << encode_GS_param( GS_ID_expPedalType );
    midiMsgList << encode_GS_param( GS_ID_buttonHoldTime );
    midiMsgList << encode_GS_param( GS_ID_tapDisplayType );
    midiMsgList << encode_GS_param( GS_ID_tapType );
    midiMsgList << encode_GS_param( GS_ID_pedalLedView );
    midiMsgList << encode_GS_param( GS_ID_pedalTunerScheme );
    midiMsgList << encode_GS_param( GS_ID_pedalBrightness );
    midiMsgList << encode_GS_param( GS_ID_pedalsCalibrationLo );
    midiMsgList << encode_GS_param( GS_ID_pedalsCalibrationHi );

    return(midiMsgList);
}

QVector<midiMsg_t> encodeBank(int number){
    QVector<midiMsg_t> midiMsgList;
    if ( number < 0 ){
        qCCritical(SSX) << Q_FUNC_INFO << "number is negative";
        return(midiMsgList);
    }
    if ( number >= g_BanksSettings.size() ){
        qCCritical(SSX) << Q_FUNC_INFO << "number is too big";
        return(midiMsgList);
    }

    BankSettings bank = g_BanksSettings.at(number);

    // сохраняем контекс кнопки № 0 ... N
    for(uint8_t btnNum=0; btnNum<FOOT_BUTTONS_NUM; btnNum++)
        midiMsgList << encode_BS_param( btnNum, static_cast<uint8_t>(number), bank );

    midiMsgList << encode_BS_param( BS_ID_tapCC,        static_cast<uint8_t>(number), bank );
    midiMsgList << encode_BS_param( BS_ID_pedalsCC,     static_cast<uint8_t>(number), bank );
    midiMsgList << encode_BS_param( BS_ID_tunerCC,      static_cast<uint8_t>(number), bank );
    midiMsgList << encode_BS_param( BS_ID_buttonType,   static_cast<uint8_t>(number), bank );
    midiMsgList << encode_BS_param( BS_ID_bankName,     static_cast<uint8_t>(number), bank );

    return(midiMsgList);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///									T R A C E		S E T T I N G S
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
TRACE FUNCTIONS CODE in bits, send as bytes chain in ack message
---------------------------------------------------------------
[7] - allways 0
[6]--------------
[5] Error
[4] number
[3]--------------
[2] 1- success 0-fail
[1]--------------
[0] function ID
*/
/* trace length buffer */
#define TRACE_LENGTH					(40) /* ACK full msg length = 8*/
/* functions IDs */
#define handleMidiSysExSettings_ID		(1)
#define load_GS_param_ID				(2)
#define load_BS_param_ID				(3)
/* ErrNo for handleMidiSysExSettings */
#define HMS_BAD_FIRST_CHECK				(1)
#define HMS_NO_MIDI_SYSEX_START			(2)
#define HMS_BAD_MANUFACTURER_ID_0		(3)
#define HMS_BAD_MANUFACTURER_ID_1		(4)
#define HMS_BAD_MANUFACTURER_ID_2		(5)
#define HMS_BAD_NETWORK_NUMBER			(6)
#define HMS_BAD_MODEL_NUMBER			(7)
#define HMS_COUNTER_OVERRUN				(8)
#define HMS_NO_PAYLOAD					(9)
#define HMS_NO_DECODE					(10)
/* ErrNo for load_GS_param */
#define LGS_BAD_PDATA					(1)
#define LGS_BAD_PGLOBALS				(2)
#define LGS_BAD_LEN						(3)
#define LGS_BAD_GDL_GS					(4)
#define LGS_BAD_GLOBALS_MSG				(5)
#define LGS_BAD_PARAMS_ID				(6)
#define LGS_COUNTER_OVERRUN				(7)
#define LGS_NO_DECODE_BYTE				(8)
/* ErrNo for load_BS_param */
#define LBS_BAD_PDATA					(1)
#define LBS_BAD_PGLOBALS				(2)
#define LBS_BAD_LEN						(3)
#define LBS_BAD_BANKS_MSG				(4)
#define LBS_BAD_BANK_NUM1				(5)
#define LBS_BAD_BANK_NUM2				(6)
#define LBS_BAD_PARAMS_ID				(7)
#define LBS_COUNTER_OVERRUN				(8)
#define LBS_BAD_GDL_BS					(9)
#define LBS_NO_DECODE_BYTE				(8)

#include <QMap>
static
void decodeTRACE( QVector<uint8_t> trace ){
    if ( trace.isEmpty() ){
        qCDebug(SSX) << Q_FUNC_INFO << "trace is empty";
        return;
    }
    if ( trace.length() < TRACE_LENGTH ){
        qCDebug(SSX) << Q_FUNC_INFO << "trace bad length";
        return;
    }
    uint8_t tcode;

    // handleMidiSysExSettings
    tcode = trace.takeFirst();
    if ( handleMidiSysExSettings_ID == (tcode & 0x03)){
        if ( 0 != (tcode & 0x04) ){
            qCDebug(SSX) << Q_FUNC_INFO << "handleMidiSysExSettings SUCC";
        }
        else {
            QString str = "handleMidiSysExSettings FAIL ";
            tcode = (tcode >> 3) & 0x0f;
            QMap<int, QString> errMap = {
                {HMS_BAD_FIRST_CHECK,       "HMS_BAD_FIRST_CHECK"},
                {HMS_NO_MIDI_SYSEX_START,   "HMS_NO_MIDI_SYSEX_START"},
                {HMS_BAD_MANUFACTURER_ID_0, "HMS_BAD_MANUFACTURER_ID_0"},
                {HMS_BAD_MANUFACTURER_ID_1, "HMS_BAD_MANUFACTURER_ID_1"},
                {HMS_BAD_MANUFACTURER_ID_2, "HMS_BAD_MANUFACTURER_ID_2"},
                {HMS_BAD_NETWORK_NUMBER,    "HMS_BAD_NETWORK_NUMBER"},
                {HMS_BAD_MODEL_NUMBER,      "HMS_BAD_MODEL_NUMBER"},
                {HMS_COUNTER_OVERRUN,       "HMS_COUNTER_OVERRUN"},
                {HMS_NO_PAYLOAD,            "HMS_NO_PAYLOAD"},
                {HMS_NO_DECODE,             "HMS_NO_DECODE"}
            };
            QString err = errMap.value(tcode);
            if ( 0 == err.length() ) err="UNDEF";
            qCDebug(SSX) << Q_FUNC_INFO << str << err;
        }
    }
    else {
        qCDebug(SSX) << Q_FUNC_INFO << "not handleMidiSysExSettings!!!! abort";
        return;
    }
    // load_GS_param
    QMap< int, QString > gsMap = {
        {1, "GS_ID_bnkNum"},
        {2, "GS_ID_midiChanNum"},
        {3, "GS_ID_useBankSelectMess"},
        {4, "GS_ID_bankSelectMessType"},
        {5, "GS_ID_bnkSwOnBoard"},
        {6, "GS_ID_showPresetBank"},
        {7, "GS_ID_targetDevice"},
        {8, "GS_ID_usbBaudrate"},
        {9, "GS_ID_inputThrough"},
        {10,"GS_ID_maxBankNumber"},
        {11,"GS_ID_screenBrightness"},
        {12,"GS_ID_screenContrast"},
        {13,"GS_ID_expPedalType"},
        {14,"GS_ID_buttonHoldTime"},
        {15,"GS_ID_tapDisplayType"},
        {16,"GS_ID_tapType"},
        {17,"GS_ID_pedalLedView"},
        {18,"GS_ID_pedalTunerScheme"},
        {19,"GS_ID_pedalBrightness"},
        {20,"GS_ID_pedalsCalibrationLo"},
        {21,"GS_ID_pedalsCalibrationHi"}
    };
    QMap<int, QString> errGsMap = {
        {LGS_BAD_PDATA,                 "LGS_BAD_PDATA"},
        {LGS_BAD_PGLOBALS,              "LGS_BAD_PGLOBALS"},
        {LGS_BAD_LEN,                   "LGS_BAD_LEN"},
        {LGS_BAD_GDL_GS,                "LGS_BAD_GDL_GS"},
        {LGS_BAD_GLOBALS_MSG,           "LGS_BAD_GLOBALS_MSG"},
        {LGS_BAD_PARAMS_ID,             "LGS_BAD_PARAMS_ID"},
        {LGS_COUNTER_OVERRUN,           "LGS_COUNTER_OVERRUN"},
        {LGS_NO_DECODE_BYTE,            "LGS_NO_DECODE_BYTE"}
    };

    for( int i=1; i<=21; i++ ){
        tcode = trace.takeFirst();
        if ( load_GS_param_ID == (tcode & 0x03)){
            QString str = "load_GS_param " + gsMap.value(i) + " ";
            if ( 0 != (tcode & 0x04) ){
                str += "SUCC";
                qCDebug(SSX) << Q_FUNC_INFO << str;
            }
            else {
                str += "FAIL ";
                tcode = (tcode>>3) & 0x0f;
                QString err = errGsMap.value(tcode);
                if ( 0 == err.length() ) err="UNDEF";
                qCDebug(SSX) << Q_FUNC_INFO << str << err;
            }
        }
        else {
            qCDebug(SSX) << Q_FUNC_INFO << "not load_GS_param!!!! abort";
            return;
        }
    }
    //load_BS_param
    QMap< int, QString > bsMap = {
        { 1,"BS_ID_ButtonCotext_1"},
        { 2,"BS_ID_ButtonCotext_2"},
        { 3,"BS_ID_ButtonCotext_3"},
        { 4,"BS_ID_ButtonCotext_4"},
        { 5,"BS_ID_ButtonCotext_5"},
        { 6,"BS_ID_ButtonCotext_6"},
        { 7,"BS_ID_ButtonCotext_7"},
        { 8,"BS_ID_ButtonCotext_8"},
        { 9,"BS_ID_ButtonCotext_9"},
        {10,"BS_ID_ButtonCotext_10"},
        {11,"BS_ID_ButtonCotext_11"},
        {12,"BS_ID_ButtonCotext_12"},
        {13,"BS_ID_tapCC"},
        {14,"BS_ID_pedalsCC"},
        {15,"BS_ID_tunerCC"},
        {16,"BS_ID_buttonType"},
        {17,"BS_ID_bankName"}
    };
    QMap<int, QString> errBsMap = {
        {LBS_BAD_PDATA,             "LBS_BAD_PDATA"},
        {LBS_BAD_PGLOBALS,          "LBS_BAD_PGLOBALS"},
        {LBS_BAD_LEN,               "LBS_BAD_LEN"},
        {LBS_BAD_BANKS_MSG,         "LBS_BAD_BANKS_MSG"},
        {LBS_BAD_BANK_NUM1,         "LBS_BAD_BANK_NUM1"},
        {LBS_BAD_BANK_NUM2,         "LBS_BAD_BANK_NUM2"},
        {LBS_BAD_PARAMS_ID,         "LBS_BAD_PARAMS_ID"},
        {LBS_COUNTER_OVERRUN,       "LBS_COUNTER_OVERRUN"},
        {LBS_BAD_GDL_BS,            "LBS_BAD_GDL_BS"},
        {LBS_NO_DECODE_BYTE,        "LBS_NO_DECODE_BYTE"},
    };
    for( int i=1; i<=17; i++ ){
        tcode = trace.takeFirst();
        if ( load_BS_param_ID == (tcode & 0x03)){
            QString str = "load_BS_param " + bsMap.value(i) + " ";
            if ( 0 != (tcode & 0x04) ){
                str += "SUCC";
                qCDebug(SSX) << Q_FUNC_INFO << str;
            }
            else {
                str += "FAIL ";
                tcode = (tcode>>3) & 0x0f;
                QString err = errBsMap.value(tcode);
                if ( 0 == err.length() ) err="UNDEF";
                qCDebug(SSX) << Q_FUNC_INFO << str << err;
            }
        }
        else {
            qCDebug(SSX) << Q_FUNC_INFO << "not load_BS_param_ID!!!! abort";
            return;
        }
    }
}

uint8_t decode(const midiMsg_t &midiMsg ){
    typedef enum {
        START_SYSEX = 0,
        MANUFACTURER_ID_0,
        MANUFACTURER_ID_1,
        MANUFACTURER_ID_2,
        NETWORK_NUM,
        MODEL_NUM,
        PAYLOAD
    } State_t;

    if ( 0 == midiMsg.length()) return(UNKNOWN_MSG);

    bool                _do_				= true;				// true - do loop while, false - exit from loop
    uint8_t             byte				= 0;
    int                 counter				= 0;
    State_t             state				= START_SYSEX;
    uint32_t            manfId				= MANUFACTURER_ID;
    QVector<uint8_t>    payload;

    while ( 1 ){
        switch (state){
            case START_SYSEX:
                if ( MIDI_SYSEX_START != midiMsg.at(counter) ){
                    qCDebug(SSX) << Q_FUNC_INFO << "no MIDI_SYSEX_START";
                    return(UNKNOWN_MSG);
                }
                state = MANUFACTURER_ID_0;
                break;
            case MANUFACTURER_ID_0:
                byte = (manfId >> 16) & 0x7F;
                if ( byte != midiMsg.at(counter) ){
                    qCDebug(SSX) << Q_FUNC_INFO << "bad MANUFACTURER_ID_0";
                    return(UNKNOWN_MSG);
                }
                state = MANUFACTURER_ID_1;
                break;
            case MANUFACTURER_ID_1:
                byte = (manfId >> 8) & 0x7F;
                if ( byte != midiMsg.at(counter) ){
                    qCDebug(SSX) << Q_FUNC_INFO << "bad MANUFACTURER_ID_1";
                    return(UNKNOWN_MSG);
                }
                state = MANUFACTURER_ID_2;
                break;
            case MANUFACTURER_ID_2:
                byte = manfId & 0x7F;
                if ( byte != midiMsg.at(counter) ){
                    qCDebug(SSX) << Q_FUNC_INFO << "bad MANUFACTURER_ID_2";
                    return(UNKNOWN_MSG);
                }
                state = NETWORK_NUM;
                break;
            case NETWORK_NUM:
                if ( NETWORK_NUMBER != midiMsg.at(counter) ){
                    qCDebug(SSX) << Q_FUNC_INFO << "bad NETWORK_NUMBER";
                    return(UNKNOWN_MSG);
                }
                state = MODEL_NUM;
                break;
            case MODEL_NUM:
                if ( MODEL_NUMBER != midiMsg.at(counter) ){
                    qCDebug(SSX) << Q_FUNC_INFO << "bad MODEL_NUMBER";
                    return(UNKNOWN_MSG);
                }
                state = PAYLOAD;
                break;
            case PAYLOAD:
                if ( MIDI_SYSEX_END == midiMsg.at(counter) )
                    _do_ = false;
                else
                    payload << midiMsg.at(counter);
                break;
        }
        if ( false == _do_ ) break;
        counter++;
        if ( counter >= midiMsg.length() ){
            qCDebug(SSX) << Q_FUNC_INFO << "bad len counter=" << counter << "msglen=" << midiMsg.length();
            return(UNKNOWN_MSG);
        }
    }

    qCDebug(SSX) << Q_FUNC_INFO << "len=" << payload.length() << "payload=" << hex << payload;
#if 0
    // декодируем ACK
    if ((1==payload.length())&&( ACK_MSG==payload.at(0))){
        qCDebug(SSX) << Q_FUNC_INFO << "ACK_MSG decoded";
        return(ACK_MSG);
    }
#endif
    // декодируем ACK
    if ( ACK_MSG==payload.at(0)){
        qCDebug(SSX) << Q_FUNC_INFO << "ACK_TRACE_MSG decoded";
        payload.pop_front();
        decodeTRACE( payload );
        return(ACK_MSG);
    }
    // декодируем EOT
    if ((1==payload.length())&&( EOT_MSG==payload.at(0))){
        qCDebug(SSX) << Q_FUNC_INFO << "EOT_MSG decoded";
        return(EOT_MSG);
    }
    // decode globals
    if (decode_GS_param( GS_ID_bnkNum,              payload )){
        qCDebug(SSX) << Q_FUNC_INFO << "GS_ID_bnkNum decoded";
        return(GLOBALS_MSG);
    }
    if (decode_GS_param( GS_ID_midiChanNum,         payload )){
        qCDebug(SSX) << Q_FUNC_INFO << "GS_ID_midiChanNum decoded";
        return(GLOBALS_MSG);
    }
    if (decode_GS_param( GS_ID_useBankSelectMess,   payload )){
        qCDebug(SSX) << Q_FUNC_INFO << "GS_ID_useBankSelectMess decoded";
        return(GLOBALS_MSG);
    }
    if (decode_GS_param( GS_ID_bankSelectMessType,  payload )){
        qCDebug(SSX) << Q_FUNC_INFO << "GS_ID_bankSelectMessType decoded";
        return(GLOBALS_MSG);
    }
    if (decode_GS_param( GS_ID_bnkSwOnBoard,        payload )){
        qCDebug(SSX) << Q_FUNC_INFO << "GS_ID_bnkSwOnBoard decoded";
        return(GLOBALS_MSG);
    }
    if (decode_GS_param( GS_ID_showPresetBank,      payload )){
        qCDebug(SSX) << Q_FUNC_INFO << "GS_ID_showPresetBank decoded";
        return(GLOBALS_MSG);
    }
    if (decode_GS_param( GS_ID_targetDevice,        payload )){
        qCDebug(SSX) << Q_FUNC_INFO << "GS_ID_targetDevice decoded";
        return(GLOBALS_MSG);
    }
    if (decode_GS_param( GS_ID_usbBaudrate,         payload )){
        qCDebug(SSX) << Q_FUNC_INFO << "GS_ID_usbBaudrate decoded";
        return(GLOBALS_MSG);
    }
    if (decode_GS_param( GS_ID_inputThrough,        payload )){
        qCDebug(SSX) << Q_FUNC_INFO << "GS_ID_inputThrough decoded";
        return(GLOBALS_MSG);
    }
    if (decode_GS_param( GS_ID_maxBankNumber,       payload )){
        qCDebug(SSX) << Q_FUNC_INFO << "GS_ID_maxBankNumber decoded";
        return(GLOBALS_MSG);
    }
    if (decode_GS_param( GS_ID_screenBrightness,    payload )){
        qCDebug(SSX) << Q_FUNC_INFO << "GS_ID_screenBrightness decoded";
        return(GLOBALS_MSG);
    }
    if (decode_GS_param( GS_ID_screenContrast,      payload )){
        qCDebug(SSX) << Q_FUNC_INFO << "GS_ID_screenContrast decoded";
        return(GLOBALS_MSG);
    }
    if (decode_GS_param( GS_ID_expPedalType,        payload )){
        qCDebug(SSX) << Q_FUNC_INFO << "GS_ID_expPedalType decoded";
        return(GLOBALS_MSG);
    }
    if (decode_GS_param( GS_ID_buttonHoldTime,      payload )){
        qCDebug(SSX) << Q_FUNC_INFO << "GS_ID_buttonHoldTime decoded";
        return(GLOBALS_MSG);
    }
    if (decode_GS_param( GS_ID_tapDisplayType,      payload )){
        qCDebug(SSX) << Q_FUNC_INFO << "GS_ID_tapDisplayType decoded";
        return(GLOBALS_MSG);
    }
    if (decode_GS_param( GS_ID_tapType,             payload )){
        qCDebug(SSX) << Q_FUNC_INFO << "GS_ID_tapType decoded";
        return(GLOBALS_MSG);
    }
    if (decode_GS_param( GS_ID_pedalLedView,        payload )){
        qCDebug(SSX) << Q_FUNC_INFO << "GS_ID_pedalLedView decoded";
        return(GLOBALS_MSG);
    }
    if (decode_GS_param( GS_ID_pedalTunerScheme,    payload )){
        qCDebug(SSX) << Q_FUNC_INFO << "GS_ID_pedalTunerScheme decoded";
        return(GLOBALS_MSG);
    }
    if (decode_GS_param( GS_ID_pedalBrightness,     payload )){
        qCDebug(SSX) << Q_FUNC_INFO << "GS_ID_pedalBrightness decoded";
        return(GLOBALS_MSG);
    }
    if (decode_GS_param( GS_ID_pedalsCalibrationLo, payload )){
        qCDebug(SSX) << Q_FUNC_INFO << "GS_ID_pedalsCalibrationLo decoded";
        return(GLOBALS_MSG);
    }
    if (decode_GS_param( GS_ID_pedalsCalibrationHi, payload )){
        qCDebug(SSX) << Q_FUNC_INFO << "GS_ID_pedalsCalibrationHi decoded";
        return(GLOBALS_MSG);
    }

    // decode banks messages
    // пытаемся декодировать контекст кнопки
    for(uint8_t btnNum=0; btnNum<FOOT_BUTTONS_NUM; btnNum++){
        if (decode_BS_param( btnNum, payload)){
            qCDebug(SSX) << Q_FUNC_INFO << "DECODED BS context for btn=" << btnNum;
            return(BANKS_MSG);
        }
    }

    if (decode_BS_param( BS_ID_tapCC,               payload )){
        qCDebug(SSX) << Q_FUNC_INFO << "BS_ID_tapCC decoded";
        return(BANKS_MSG);
    }
    if (decode_BS_param( BS_ID_pedalsCC,            payload )){
        qCDebug(SSX) << Q_FUNC_INFO << "BS_ID_pedalsCC decoded";
        return(BANKS_MSG);
    }
    if (decode_BS_param( BS_ID_tunerCC,             payload )){
        qCDebug(SSX) << Q_FUNC_INFO << "BS_ID_tunerCC decoded";
        return(BANKS_MSG);
    }
    if (decode_BS_param( BS_ID_buttonType,          payload )){
        qCDebug(SSX) << Q_FUNC_INFO << "BS_ID_buttonType decoded";
        return(BANKS_MSG);
    }
    if (decode_BS_param( BS_ID_bankName,            payload )){
        qCDebug(SSX) << Q_FUNC_INFO << "BS_ID_bankName decoded";
        return(BANKS_MSG);
    }
    qCDebug(SSX) << Q_FUNC_INFO << "got unknown param!";

    return(UNKNOWN_MSG);
}

}//SSXMSGS

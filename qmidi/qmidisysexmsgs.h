/*utf8*/
#ifndef QMIDISYSEXMSGS_H
#define QMIDISYSEXMSGS_H
#include <QtGlobal>
#include <QVector>
#include <QLoggingCategory>
Q_DECLARE_LOGGING_CATEGORY(SSX)

/// header definitions
#define MANUFACTURER_ID         (0x007F7F)
#define NETWORK_NUMBER          (0x00)
#define MODEL_NUMBER            (0x00)

#define MAX_MIDI_CHANNEL_NUMBER (15)
#define MIDI_CHANNEL_PRIMARY    (MAX_MIDI_CHANNEL_NUMBER + 1)//If this number set as MIDI channel - channel number will taken from global setings
#define MIDI_CHANNEL_NOT_ACTIVE (MAX_MIDI_CHANNEL_NUMBER + 2)//If this number set as MIDI channel - no midi message will be send

/*Общее количество MIDI интерфейсов на контроллере 2. MIDI разъемы и USB*/
#define TOTAL_MIDI_INTERFACES   (2)

#define FOOT_BUTTONS_NUM        (12)
#define BANK_NAME_NMAX_SIZE     (13)
#define BUTTON_NAME_MAX_SIZE	(17)
#define MAX_PROGRAMS_TO_SEND    (4)

// midi system events
#define MIDI_SYSEX_START        (0xF0)
#define MIDI_SYSEX_END          (0xF7)


// признаки настройки - глобальный или банка
#define GLOBALS_MSG             (0x00)
#define BANKS_MSG               (0x01)
#define ACK_MSG                 (0x02)
#define EOT_MSG                 (0x03)
#define UNKNOWN_MSG             (0xFF)

// button context relays
#define RELAY_MASK_NO_ACTION	(0x00)
#define RELAY_MASK_CLOSE		(0x01)
#define RELAY_MASK_OPEN         (0x02)
#define RELAY_MASK_MOMENTARY	(0x03)

typedef QVector<uchar>          midiMsg_t;

namespace SSXMSGS {

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///									M O D E L S
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// models types
typedef enum {
    // models w/o pedals
    TB_5_MKII       = 5,    //number buttons
    TB_8_MKII       = 8,
    TB_12_MKII      = 12,
    // models with pedals
    TB_6P_MKII      = 106,  // number buttons - 100
    TB_11P_MKII     = 111,
} t_Models;

/// global decloration g_Model
extern t_Models g_Model;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///									G L O B A L S		S E T T I N G S
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef enum : uint8_t {
    USE_BANK_SELECT = 0,
    DONT_USE_BANK_SELECT
} UseBankSelectMess;

typedef enum : uint8_t {
    MSB = 0,
    LSB,
    MSB_AND_LSB
} BankSelectMessType;

typedef enum : uint8_t {
    NO_EXT_PEDAL = 0,
    EXT_PEDAL_BANK_SWITCH,
    EXT_PEDAL_PRESET_SWITCH,
} ExternalBs2Pedal;

typedef enum : uint8_t {
    ONLY_PRESET = 0,
    ONLY_BANK,
    PRESET_AND_BANK
} ShowPresetBank;

typedef enum : uint8_t {
    TARGET_DEVICE_AUTO = 0,
    TARGET_DEVICE_OFF,
    TARGET_DEVICE_AXE_ST,
    TARGET_DEVICE_AXE_ULTRA,
    TARGET_DEVICE_AX8,
    TARGET_DEVICE_AXEII,
    TARGET_DEVICE_AXEII_XL,
    TARGET_DEVICE_AXEII_XL_PLUS,
    TARGET_DEVICE_AXEIII,
    TARGET_DEVICE_KPA,
    TARGET_DEVICE_G_MAJOR,
} TargetDevice;

typedef enum : uint8_t {
    MIDI_BAUD = 0,
    CUSTOM_BAUD_57600
} UsbBaudrate;

typedef enum : uint8_t {
    IN_TO_NONE = 0,         //Don't send input to any out
    IN_TO_MIDI_OUT,         //Send input to midi output
    IN_TO_USB_OUT,          //Send input to usb output
    IN_TO_MIDI_AND_USB_OUT  //Send input both to midi and to usb output
} InputThrough;

typedef enum : uint8_t {
    LINEAL = 0,
    LOGARITHMIC,
    BACK_LOGARITHMIC
} ExpPedalType;

typedef enum : uint8_t {
    ms100 = 0,
    ms200,
    ms300,
    ms400,
    ms500,
    ms600,
    ms700,
    ms800,
    ms900,
    ms1000,
    ms1100,
    ms1200,
    ms1300,
    ms1400,
    ms1500,
    ms1600,
    ms1700,
    ms1800,
    ms1900,
    ms2000
} HoldTime;

typedef enum : uint8_t {
    NOT_SHOW_TAP = 0,
    ON_SCREEN,
    ON_ACTIVE_PRESET,
    LED1,
    LED2,
    LED3,
    LED4,
    LED5,
    LED6,
    LED7,
    LED8,
    LED9,
    LED10,
    LED11,
    LED12
} TapDisplayType;

typedef enum : uint8_t {
    TAP_TOGGLE = 0,
    TAP_MOMETRARY,
    TAP_CONST_VALUE
} TapType;

typedef enum : uint8_t {
    PEDAL_LED_MODE_BUTTON = 0,
    PEDAL_LED_MODE_TUNER,
    PEDAL_LED_MODE_DUAL
} PedalLedView;

typedef enum : uint8_t {
    Scheme1 = 0,
    Scheme2
} PedalTunerScheme;

#pragma pack(push, 1)
/// global settings struct
struct GlobalSettings {

    uint8_t             bnkNum;                             //Текущий номер банка. В настройках он меняется когда выбираем банк для сохранения/загрузки( Banks )
    uint8_t             midiChanNum;                        //System Setup -> MIDI channel. Диапазон от 0 до 15, на экране отображается от 1 до 16
    UseBankSelectMess   useBankSelectMess;                  //System Setup -> Prg. ch. mode
    BankSelectMessType  bankSelectMessType;                 //System Setup -> Bnk. Sel mode
    ExternalBs2Pedal    bnkSwOnBoard;                       //System Setup -> Bank sw. mode
    ShowPresetBank      Show_pr_name;                       //System Setup -> Show pr. name
    TargetDevice        targetDevice;                       //System Setup -> Target device
    UsbBaudrate         usbBaudrate;                        //System Setup -> USB baudrate
    InputThrough        inputThrough[TOTAL_MIDI_INTERFACES];//System Setup ->MIDI thru map
    uint8_t             maxBankNumber;                      //System Setup ->Max. bank
    uint8_t             screenBrightness;                   //System Setup -> Screen brightness. отображаем от 1 до 10
    uint8_t             screenContrast;                     //System Setup ->Screen contrast. от 0 до 255
    ExpPedalType        expPtype[3];                        //Exp&Tap&Tune -> Exp. P1(2) type.
    HoldTime            buttonHoldTime;                     //Exp&Tap&Tune -> BUT hold time. Диапазон от 1 до 15, в меню отобржажается как 100ms, 200ms, ..., 1500ms
    TapDisplayType      tapDisplayType;                     //Exp&Tap&Tune -> Tap display
    TapType             tapType;                            //Exp&Tap&Tune -> Tap type
    PedalLedView        pedalLedView;                       //Pedal view -> Display type
    PedalTunerScheme    pedalTunerScheme;                   //Pedal view -> Tuner scheme. Тут пока только два значения 0 и 1. Отобразить можно как Scheme 1 и  Scheme 2
    uint8_t             pedalBrightness;                    //Pedal view -> BRIGHTNESS. Яркость светодидов педали. отображаем от 1 до 10
    //This setting are not mapped into GUI, but is stores in global settings during pedals calibration
    uint8_t             pLowPos[3];
    uint8_t             pHighPos[3];

    /// конструктор
    GlobalSettings();
    /// конструктор
    GlobalSettings(const GlobalSettings &other);
    /// деструктор
    ~GlobalSettings();
};
#pragma pack(pop)

/// global decloration g_GlobalSettings
extern GlobalSettings g_GlobalSettings;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///									B A N K			S E T T I N G S
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef uint8_t CtrlChangeNum;
typedef uint8_t BankNumber;

/// buttons types
typedef enum : uint8_t {
    PRESET_CHANGE = 0,
    CC_TOGGLE,
    CC_MOMENTARY,
    CC_CONST_VAL,
    NRPN_TOGGLE,
    NRPN_MOMENTARY,
    NRPN_CONST_VAL,
    NOTE_ON,
    NOTE_OFF,
    PRESET_UP,
    PRESET_DOWN,
    BANK_UP,
    BANK_DOWN,
    BANK_TO,
    BUTTON_TYPE_TOTAL_ELEMENTS
} ButtonType;

#define VENDOR_BLOCK_ID_NONE 255
typedef enum : uint8_t {
    ID_CONTROL = 2,
    ID_TUNER = 35,
    ID_IRCAPTURE,
    ID_INPUT1,
    ID_INPUT2,
    ID_INPUT3,
    ID_INPUT4,
    ID_INPUT5, // USB Input
    ID_OUTPUT1,
    ID_OUTPUT2,
    ID_OUTPUT3,
    ID_OUTPUT4,
    ID_COMP1,
    ID_COMP2,
    ID_COMP3,
    ID_COMP4,
    ID_GRAPHEQ1,
    ID_GRAPHEQ2,
    ID_GRAPHEQ3,
    ID_GRAPHEQ4,
    ID_PARAEQ1,
    ID_PARAEQ2,
    ID_PARAEQ3,
    ID_PARAEQ4,
    ID_DISTORT1,
    ID_DISTORT2,
    ID_DISTORT3,
    ID_DISTORT4,
    ID_CAB1,
    ID_CAB2,
    ID_CAB3,
    ID_CAB4,
    ID_REVERB1,
    ID_REVERB2,
    ID_REVERB3,
    ID_REVERB4,
    ID_DELAY1,
    ID_DELAY2,
    ID_DELAY3,
    ID_DELAY4,
    ID_MULTITAP1,
    ID_MULTITAP2,
    ID_MULTITAP3,
    ID_MULTITAP4,
    ID_CHORUS1,
    ID_CHORUS2,
    ID_CHORUS3,
    ID_CHORUS4,
    ID_FLANGER1,
    ID_FLANGER2,
    ID_FLANGER3,
    ID_FLANGER4,
    ID_ROTARY1,
    ID_ROTARY2,
    ID_ROTARY3,
    ID_ROTARY4,
    ID_PHASER1,
    ID_PHASER2,
    ID_PHASER3,
    ID_PHASER4,
    ID_WAH1,
    ID_WAH2,
    ID_WAH3,
    ID_WAH4,
    ID_FORMANT1,
    ID_FORMANT2,
    ID_FORMANT3,
    ID_FORMANT4,
    ID_VOLUME1,
    ID_VOLUME2,
    ID_VOLUME3,
    ID_VOLUME4,
    ID_TREMOLO1,
    ID_TREMOLO2,
    ID_TREMOLO3,
    ID_TREMOLO4,
    ID_PITCH1,
    ID_PITCH2,
    ID_PITCH3,
    ID_PITCH4,
    ID_FILTER1,
    ID_FILTER2,
    ID_FILTER3,
    ID_FILTER4,
    ID_FUZZ1,
    ID_FUZZ2,
    ID_FUZZ3,
    ID_FUZZ4,
    ID_ENHANCER1,
    ID_ENHANCER2,
    ID_ENHANCER3,
    ID_ENHANCER4,
    ID_MIXER1,
    ID_MIXER2,
    ID_MIXER3,
    ID_MIXER4,
    ID_SYNTH1,
    ID_SYNTH2,
    ID_SYNTH3,
    ID_SYNTH4,
    ID_VOCODER1,
    ID_VOCODER2,
    ID_VOCODER3,
    ID_VOCODER4,
    ID_MEGATAP1,
    ID_MEGATAP2,
    ID_MEGATAP3,
    ID_MEGATAP4,
    ID_CROSSOVER1,
    ID_CROSSOVER2,
    ID_CROSSOVER3,
    ID_CROSSOVER4,
    ID_GATE1,
    ID_GATE2,
    ID_GATE3,
    ID_GATE4,
    ID_RINGMOD1,
    ID_RINGMOD2,
    ID_RINGMOD3,
    ID_RINGMOD4,
    ID_MULTICOMP1,
    ID_MULTICOMP2,
    ID_MULTICOMP3,
    ID_MULTICOMP4,
    ID_TENTAP1,
    ID_TENTAP2,
    ID_TENTAP3,
    ID_TENTAP4,
    ID_RESONATOR1,
    ID_RESONATOR2,
    ID_RESONATOR3,
    ID_RESONATOR4,
    ID_LOOPER1,
    ID_LOOPER2,
    ID_LOOPER3,
    ID_LOOPER4,
    ID_TONEMATCH1,
    ID_TONEMATCH2,
    ID_TONEMATCH3,
    ID_TONEMATCH4,
    ID_RTA1,
    ID_RTA2,
    ID_RTA3,
    ID_RTA4,
    ID_PLEX1,
    ID_PLEX2,
    ID_PLEX3,
    ID_PLEX4,
    ID_FBSEND1,
    ID_FBSEND2,
    ID_FBSEND3,
    ID_FBSEND4,
    ID_FBRETURN1,
    ID_FBRETURN2,
    ID_FBRETURN3,
    ID_FBRETURN4,
    ID_MIDIBLOCK,
    ID_MULTIPLEXER1,
    ID_MULTIPLEXER2,
    ID_MULTIPLEXER3,
    ID_MULTIPLEXER4,
    ID_IRPLAYER1,
    ID_IRPLAYER2,
    ID_IRPLAYER3,
    ID_IRPLAYER4,
    ID_FOOTCONTROLLER,
    ID_PRESET_FC,
    ID_NONE = VENDOR_BLOCK_ID_NONE
} AxeFxIII_EFFECT_ID;

#pragma pack(push, 1)
///This is using for all CC and NRPN button types
struct ContolAndNrpnChangeContext {
    ///MSB num for MRPN message or CC num for
    uint8_t                     ctrlMsbFreezeNumber;
    ///CC num for CC message
    uint8_t                     ctrlLsbNumber;
    uint8_t                     paramMsbOnValue;
    ///CC val for CC message
    uint8_t                     paramLsbOnValue;
    uint8_t                     paramMsbOffValue;
    ///CC val for CC message
    uint8_t                     paramLsbOffValue;
    uint8_t                     autoSendState;
    ///custom number from vendor SysEx, attach to this button as IA
    uint8_t                     vendorBlockId;
};

struct NoteOnConstContext {
    uint8_t                     noteNumber;
    uint8_t                     noteVelocity;
};

/// определение CommonContext
typedef union {
    NoteOnConstContext			noteOnConstContext_;
    ContolAndNrpnChangeContext  contolAndNrpnChangeContext_;
    BankNumber					bankNumber;
} CommonContext;

struct PresetChangeContext{
    /*Нумерация битов реле от старшего к младшему по 2 бита	на значение*/
    //uint8_t relays;
    uint8_t                     midiChannelNumbers[MAX_PROGRAMS_TO_SEND];
    uint16_t                    programsNumbers[MAX_PROGRAMS_TO_SEND];//14 bit
    uint8_t                     banksNumbers[MAX_PROGRAMS_TO_SEND];
    uint16_t                    iaState;//14 bit
};

/// button context struct
struct ButtonContext {
    ///Used in most type of buttons, so let it be always available
    uint8_t                     relays;
    PresetChangeContext         presetChangeContext;
    CommonContext               commonContext;
    BankNumber                  bankNumber;
    ///some text data
    char                        nameAlias[BUTTON_NAME_MAX_SIZE];
};

/// bank settings structure
struct BankSettings {
    CtrlChangeNum               tapCc;
    ///0,1 - external pedals, 2 - onbpard pedal, 3 - onboard pedal alternate number
    CtrlChangeNum               pedalsCc[4];
    CtrlChangeNum               tunerCc;

    ButtonType                  buttonType[FOOT_BUTTONS_NUM];
    ButtonContext               buttonContext[FOOT_BUTTONS_NUM];

    char                        BankName[BANK_NAME_NMAX_SIZE];

    /// «Bank select action» group
    /// 0 - off, 1 -on Send PC checkbox
    uint8_t                     selectBankAction;
    /// «Bank select action» group
    /// 0 ... 127 (on GUI 1...128)
    uint8_t                     selectBankActionProgNum;

    /// конструктор
    BankSettings();
    /// конструктор
    BankSettings(const BankSettings &other);
    /// деструктор
    ~BankSettings();
};
#pragma pack(pop)

/// global decloration g_BanksSettings
extern QVector<BankSettings>   g_BanksSettings;

/// to init the module
void init();

/// encode all settings into midi sysext messages
/// \return enocoded messages
QVector<midiMsg_t> encode();

/// encode global settings into midi sysext messages
/// \return enocoded messages
QVector<midiMsg_t> encodeGlobals();

/// encode bank settings into midi sysext messages
/// \param number[in] - bank number
/// \return enocoded messages
QVector<midiMsg_t> encodeBank(int number);

/// decode midi message
/// \param midiMsg[in] - midi message
/// \return if decoded 0x00 - GLOBALS_MSG, 0x01 - BANKS_MSG, 0x02 - ACK_MSG, 0x03 - EOT_MSG, else 0xFF - UNKNOWN_MSG
uint8_t decode( const midiMsg_t & midiMsg );

/// save settings to file
/// \param fileName[in] - file name
/// \param error[in,out]- error text
/// \return true - if success, else - false
bool save( QString fileName, QString & error );

/// read settings from file
/// \param fileName[in] - file name
/// \param error[in,out]- error text
/// \return true - if success
bool load(QString fileName , QString &error);

} //SSXMSGS


#endif // QMIDISYSEXMSGS_H

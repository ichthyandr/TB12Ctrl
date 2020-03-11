#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>

#include "qhomewidget.h"
#include "qnrpntwidget.h"
#include "qmidi/qmidisysexmsgs.h"

Q_LOGGING_CATEGORY( NPRNT, "NPRNT" )

static QNrpnTWidget *   g_NRPNt{Q_NULLPTR};
static QNrpnTWidget *   g_NRPNm{Q_NULLPTR};

QNrpnTWidget::QNrpnTWidget(bool toggle, QWidget *parent) : QWidget(parent), m_bToggle(toggle){
    qCDebug(NPRNT) << Q_FUNC_INFO;

    if (m_bToggle)
        g_NRPNt = this;
    else
        g_NRPNm = this;

    auto vMainLayout = new QVBoxLayout();
    setLayout( vMainLayout );

    auto hMainLayout = new QHBoxLayout();
    vMainLayout->addLayout( hMainLayout );
    vMainLayout->addStretch(1000);

    auto grid = new QGridLayout();
    hMainLayout->addLayout( grid );
    hMainLayout->addStretch(1000);

    QString title = (m_bToggle)?tr("NRPN Toggle"):tr("NRPN Momentary");

    auto gbNPRNt    = new QGroupBox(title);
    auto gbRS       = new QGroupBox(tr("Relay States"));
    auto gbVendor   = (m_bToggle)?(new QGroupBox(tr("AxeFxIII Effect"),this)):Q_NULLPTR;

    grid->addWidget( gbNPRNt, 0, 0, Qt::AlignLeft );
    grid->addWidget( gbRS,  0, 1, Qt::AlignLeft );
    if (m_bToggle) grid->addWidget( gbVendor,  0, 2, Qt::AlignLeft );

    // Vendor
    if (m_bToggle) {
        auto vbxVID  = new QVBoxLayout();
        gbVendor->setLayout(vbxVID);

        auto gridVID= new QGridLayout();
        vbxVID->addLayout(gridVID);
        vbxVID->addStretch(1000);

        auto lblVID = new QLabel(tr("Effect ID"));
        auto cbxVID = new QComboBox( this );

        m_vbxVID = cbxVID;

        cbxVID->addItem(tr("Control"),      QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_CONTROL));
        cbxVID->addItem(tr("Tuner"),        QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_TUNER));
        cbxVID->addItem(tr("IRCapture"),    QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_IRCAPTURE));
        cbxVID->addItem(tr("Input1"),       QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_INPUT1));
        cbxVID->addItem(tr("Input2"),       QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_INPUT2));
        cbxVID->addItem(tr("Input3"),       QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_INPUT3));
        cbxVID->addItem(tr("Input4"),       QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_INPUT4));
        cbxVID->addItem(tr("Input5"),       QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_INPUT5));
        cbxVID->addItem(tr("Output1"),      QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_OUTPUT1));
        cbxVID->addItem(tr("Output2"),      QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_OUTPUT2));
        cbxVID->addItem(tr("Output3"),      QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_OUTPUT3));
        cbxVID->addItem(tr("Output4"),      QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_OUTPUT4));
        cbxVID->addItem(tr("Comp1"),        QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_COMP1));
        cbxVID->addItem(tr("Comp2"),        QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_COMP2));
        cbxVID->addItem(tr("Comp3"),        QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_COMP3));
        cbxVID->addItem(tr("Comp4"),        QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_COMP4));
        cbxVID->addItem(tr("GraphEQ1"),     QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_GRAPHEQ1));
        cbxVID->addItem(tr("GraphEQ2"),     QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_GRAPHEQ2));
        cbxVID->addItem(tr("GraphEQ3"),     QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_GRAPHEQ3));
        cbxVID->addItem(tr("GraphEQ4"),     QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_GRAPHEQ4));
        cbxVID->addItem(tr("ParaEQ1"),      QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_PARAEQ1));
        cbxVID->addItem(tr("ParaEQ2"),      QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_PARAEQ2));
        cbxVID->addItem(tr("ParaEQ3"),      QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_PARAEQ3));
        cbxVID->addItem(tr("ParaEQ4"),      QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_PARAEQ4));
        cbxVID->addItem(tr("Distort1"),     QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_DISTORT1));
        cbxVID->addItem(tr("Distort2"),     QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_DISTORT2));
        cbxVID->addItem(tr("Distort3"),     QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_DISTORT3));
        cbxVID->addItem(tr("Distort4"),     QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_DISTORT4));
        cbxVID->addItem(tr("Cab1"),         QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_CAB1));
        cbxVID->addItem(tr("Cab2"),         QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_CAB2));
        cbxVID->addItem(tr("Cab3"),         QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_CAB3));
        cbxVID->addItem(tr("Cab4"),         QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_CAB4));
        cbxVID->addItem(tr("Reverb1"),      QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_REVERB1));
        cbxVID->addItem(tr("Reverb2"),      QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_REVERB2));
        cbxVID->addItem(tr("Reverb3"),      QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_REVERB3));
        cbxVID->addItem(tr("Reverb4"),      QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_REVERB4));
        cbxVID->addItem(tr("Delay1"),       QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_DELAY1));
        cbxVID->addItem(tr("Delay2"),       QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_DELAY2));
        cbxVID->addItem(tr("Delay3"),       QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_DELAY3));
        cbxVID->addItem(tr("Delay4"),       QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_DELAY4));
        cbxVID->addItem(tr("MultiTAP1"),    QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_MULTITAP1));
        cbxVID->addItem(tr("MultiTAP2"),    QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_MULTITAP2));
        cbxVID->addItem(tr("MultiTAP3"),    QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_MULTITAP3));
        cbxVID->addItem(tr("MultiTAP4"),    QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_MULTITAP4));
        cbxVID->addItem(tr("Chorus1"),      QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_CHORUS1));
        cbxVID->addItem(tr("Chorus2"),      QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_CHORUS2));
        cbxVID->addItem(tr("Chorus3"),      QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_CHORUS3));
        cbxVID->addItem(tr("Chorus4"),      QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_CHORUS4));
        cbxVID->addItem(tr("Flanger1"),     QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_FLANGER1));
        cbxVID->addItem(tr("Flanger2"),     QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_FLANGER2));
        cbxVID->addItem(tr("Flanger3"),     QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_FLANGER3));
        cbxVID->addItem(tr("Flanger4"),     QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_FLANGER4));
        cbxVID->addItem(tr("Rotary1"),      QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_ROTARY1));
        cbxVID->addItem(tr("Rotary2"),      QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_ROTARY2));
        cbxVID->addItem(tr("Rotary3"),      QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_ROTARY3));
        cbxVID->addItem(tr("Rotary4"),      QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_ROTARY4));
        cbxVID->addItem(tr("Phaser1"),      QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_PHASER1));
        cbxVID->addItem(tr("Phaser2"),      QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_PHASER2));
        cbxVID->addItem(tr("Phaser3"),      QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_PHASER3));
        cbxVID->addItem(tr("Phaser4"),      QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_PHASER4));
        cbxVID->addItem(tr("Wah1"),         QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_WAH1));
        cbxVID->addItem(tr("Wah2"),         QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_WAH2));
        cbxVID->addItem(tr("Wah3"),         QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_WAH3));
        cbxVID->addItem(tr("Wah4"),         QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_WAH4));
        cbxVID->addItem(tr("Formant1"),     QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_FORMANT1));
        cbxVID->addItem(tr("Formant2"),     QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_FORMANT2));
        cbxVID->addItem(tr("Formant3"),     QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_FORMANT3));
        cbxVID->addItem(tr("Formant4"),     QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_FORMANT4));
        cbxVID->addItem(tr("Volume1"),      QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_VOLUME1));
        cbxVID->addItem(tr("Volume2"),      QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_VOLUME2));
        cbxVID->addItem(tr("Volume3"),      QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_VOLUME3));
        cbxVID->addItem(tr("Volume4"),      QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_VOLUME4));
        cbxVID->addItem(tr("Tremolo1"),     QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_TREMOLO1));
        cbxVID->addItem(tr("Tremolo2"),     QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_TREMOLO2));
        cbxVID->addItem(tr("Tremolo3"),     QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_TREMOLO3));
        cbxVID->addItem(tr("Tremolo4"),     QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_TREMOLO4));
        cbxVID->addItem(tr("Pitch1"),       QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_PITCH1));
        cbxVID->addItem(tr("Pitch2"),       QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_PITCH2));
        cbxVID->addItem(tr("Pitch3"),       QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_PITCH3));
        cbxVID->addItem(tr("Pitch4"),       QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_PITCH4));
        cbxVID->addItem(tr("Filter1"),      QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_FILTER1));
        cbxVID->addItem(tr("Filter2"),      QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_FILTER2));
        cbxVID->addItem(tr("Filter3"),      QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_FILTER3));
        cbxVID->addItem(tr("Filter4"),      QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_FILTER4));
        cbxVID->addItem(tr("Fuzz1"),        QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_FUZZ1));
        cbxVID->addItem(tr("Fuzz2"),        QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_FUZZ2));
        cbxVID->addItem(tr("Fuzz3"),        QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_FUZZ3));
        cbxVID->addItem(tr("Fuzz4"),        QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_FUZZ4));
        cbxVID->addItem(tr("Enhancer1"),    QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_ENHANCER1));
        cbxVID->addItem(tr("Enhancer2"),    QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_ENHANCER2));
        cbxVID->addItem(tr("Enhancer3"),    QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_ENHANCER3));
        cbxVID->addItem(tr("Enhancer4"),    QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_ENHANCER4));
        cbxVID->addItem(tr("Mixer1"),       QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_MIXER1));
        cbxVID->addItem(tr("Mixer2"),       QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_MIXER2));
        cbxVID->addItem(tr("Mixer3"),       QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_MIXER3));
        cbxVID->addItem(tr("Mixer4"),       QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_MIXER4));
        cbxVID->addItem(tr("Synth1"),       QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_SYNTH1));
        cbxVID->addItem(tr("Synth2"),       QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_SYNTH2));
        cbxVID->addItem(tr("Synth3"),       QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_SYNTH3));
        cbxVID->addItem(tr("Synth4"),       QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_SYNTH4));
        cbxVID->addItem(tr("Vocoder1"),     QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_VOCODER1));
        cbxVID->addItem(tr("Vocoder2"),     QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_VOCODER2));
        cbxVID->addItem(tr("Vocoder3"),     QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_VOCODER3));
        cbxVID->addItem(tr("Vocoder4"),     QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_VOCODER4));
        cbxVID->addItem(tr("Megatap1"),     QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_MEGATAP1));
        cbxVID->addItem(tr("Megatap2"),     QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_MEGATAP2));
        cbxVID->addItem(tr("Megatap3"),     QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_MEGATAP3));
        cbxVID->addItem(tr("Megatap4"),     QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_MEGATAP4));
        cbxVID->addItem(tr("Crossover1"),   QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_CROSSOVER1));
        cbxVID->addItem(tr("Crossover2"),   QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_CROSSOVER2));
        cbxVID->addItem(tr("Crossover3"),   QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_CROSSOVER3));
        cbxVID->addItem(tr("Crossover4"),   QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_CROSSOVER4));
        cbxVID->addItem(tr("Gate1"),        QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_GATE1));
        cbxVID->addItem(tr("Gate2"),        QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_GATE2));
        cbxVID->addItem(tr("Gate3"),        QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_GATE3));
        cbxVID->addItem(tr("Gate4"),        QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_GATE4));
        cbxVID->addItem(tr("Ringmod1"),     QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_RINGMOD1));
        cbxVID->addItem(tr("Ringmod2"),     QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_RINGMOD2));
        cbxVID->addItem(tr("Ringmod3"),     QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_RINGMOD3));
        cbxVID->addItem(tr("Ringmod4"),     QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_RINGMOD4));
        cbxVID->addItem(tr("Multicomp1"),   QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_MULTICOMP1));
        cbxVID->addItem(tr("Multicomp2"),   QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_MULTICOMP2));
        cbxVID->addItem(tr("Multicomp3"),   QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_MULTICOMP3));
        cbxVID->addItem(tr("Multicomp4"),   QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_MULTICOMP4));
        cbxVID->addItem(tr("Tentap1"),      QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_TENTAP1));
        cbxVID->addItem(tr("Tentap2"),      QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_TENTAP2));
        cbxVID->addItem(tr("Tentap3"),      QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_TENTAP3));
        cbxVID->addItem(tr("Tentap4"),      QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_TENTAP4));
        cbxVID->addItem(tr("Resonator1"),   QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_RESONATOR1));
        cbxVID->addItem(tr("Resonator2"),   QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_RESONATOR2));
        cbxVID->addItem(tr("Resonator3"),   QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_RESONATOR3));
        cbxVID->addItem(tr("Resonator4"),   QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_RESONATOR4));
        cbxVID->addItem(tr("Looper1"),      QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_LOOPER1));
        cbxVID->addItem(tr("Looper2"),      QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_LOOPER2));
        cbxVID->addItem(tr("Looper3"),      QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_LOOPER3));
        cbxVID->addItem(tr("Looper4"),      QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_LOOPER4));
        cbxVID->addItem(tr("Tonematch1"),   QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_TONEMATCH1));
        cbxVID->addItem(tr("Tonematch2"),   QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_TONEMATCH2));
        cbxVID->addItem(tr("Tonematch3"),   QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_TONEMATCH3));
        cbxVID->addItem(tr("Tonematch4"),   QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_TONEMATCH4));
        cbxVID->addItem(tr("Rta1"),         QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_RTA1));
        cbxVID->addItem(tr("Rta2"),         QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_RTA2));
        cbxVID->addItem(tr("Rta3"),         QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_RTA3));
        cbxVID->addItem(tr("Rta4"),         QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_RTA4));
        cbxVID->addItem(tr("Plex1"),        QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_PLEX1));
        cbxVID->addItem(tr("Plex2"),        QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_PLEX2));
        cbxVID->addItem(tr("Plex3"),        QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_PLEX3));
        cbxVID->addItem(tr("Plex4"),        QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_PLEX4));
        cbxVID->addItem(tr("FBsend1"),      QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_FBSEND1));
        cbxVID->addItem(tr("FBsend2"),      QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_FBSEND2));
        cbxVID->addItem(tr("FBsend3"),      QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_FBSEND3));
        cbxVID->addItem(tr("FBsend4"),      QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_FBSEND4));
        cbxVID->addItem(tr("FBreturn1"),    QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_FBRETURN1));
        cbxVID->addItem(tr("FBreturn2"),    QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_FBRETURN2));
        cbxVID->addItem(tr("FBreturn3"),    QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_FBRETURN3));
        cbxVID->addItem(tr("FBreturn4"),    QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_FBRETURN4));
        cbxVID->addItem(tr("MIDIblock"),    QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_MIDIBLOCK));
        cbxVID->addItem(tr("Multiplexer1"), QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_MULTIPLEXER1));
        cbxVID->addItem(tr("Multiplexer2"), QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_MULTIPLEXER2));
        cbxVID->addItem(tr("Multiplexer3"), QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_MULTIPLEXER3));
        cbxVID->addItem(tr("Multiplexer4"), QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_MULTIPLEXER4));
        cbxVID->addItem(tr("IRplayer1"),    QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_IRPLAYER1));
        cbxVID->addItem(tr("IRplayer2"),    QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_IRPLAYER2));
        cbxVID->addItem(tr("IRplayer3"),    QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_IRPLAYER3));
        cbxVID->addItem(tr("IRplayer4"),    QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_IRPLAYER4));
        cbxVID->addItem(tr("FootController"),    QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_FOOTCONTROLLER));
        cbxVID->addItem(tr("Preset_FC"),    QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_PRESET_FC));
        cbxVID->addItem(tr("None"),         QVariant(SSXMSGS::AxeFxIII_EFFECT_ID::ID_NONE));

        gridVID->addWidget( lblVID,   0, 0, Qt::AlignRight);
        gridVID->addWidget( cbxVID,   0, 1, Qt::AlignLeft);

        connect(cbxVID, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index){
            if ( m_bProceed ){
                QVariant var = cbxVID->itemData(index);
                bool ok;
                int data = var.toInt(&ok);
                if ( ok ){
                    uint8_t data8 = static_cast<uint8_t>(data);
                    SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].commonContext.contolAndNrpnChangeContext_.vendorBlockId = data8;
                    emit s_changed(m_curBnkNumber);
                }
                else
                    qCCritical(CMN) << Q_FUNC_INFO << "can't convert data for vendorBlockId";
            }
        });
    }

    // NRPN Toggle
    auto vbxNPRNt  = new QVBoxLayout();
    auto gridNPRNt = new QGridLayout();

    gbNPRNt->setLayout(vbxNPRNt);
    vbxNPRNt->addLayout(gridNPRNt);
    vbxNPRNt->addStretch(1000);

    auto lbl1   = new QLabel(tr("CC#99"));
    auto sb1    = new QSpinBox(); sb1->setRange(0, 127); sb1->setWrapping(true);

    auto lbl2   = new QLabel(tr("CC#98"));
    auto sb2    = new QSpinBox(); sb2->setRange(0, 127); sb2->setWrapping(true);

    auto lbl3   = new QLabel(tr("CC#6 ON value"));
    auto sb3    = new QSpinBox(); sb3->setRange(0, 127); sb3->setWrapping(true);

    auto lbl4   = new QLabel(tr("CC#38 ON value"));
    auto sb4    = new QSpinBox(); sb4->setRange(0, 127); sb4->setWrapping(true);

    auto lbl5   = new QLabel(tr("CC#6 OFF value"));
    auto sb5    = new QSpinBox(); sb5->setRange(0, 127); sb5->setWrapping(true);

    auto lbl6   = new QLabel(tr("CC#38 OFF value"));
    auto sb6    = new QSpinBox(); sb6->setRange(0, 127); sb6->setWrapping(true);

    auto chb1   = new QCheckBox(tr("Send value when preset changed"));

    m_labels    << lbl1<< lbl2<< lbl3<< lbl4<< lbl5<< lbl6;
    m_spinBoxes << sb1 << sb2 << sb3 << sb4 << sb5 << sb6;
    m_checkBoxes<< chb1;

    gridNPRNt->addWidget( lbl1,   0, 0, Qt::AlignRight);
    gridNPRNt->addWidget( sb1,    0, 1, Qt::AlignLeft);

    gridNPRNt->addWidget( lbl2,   1, 0, Qt::AlignRight);
    gridNPRNt->addWidget( sb2,    1, 1, Qt::AlignLeft);

    gridNPRNt->addWidget( lbl3,   2, 0, Qt::AlignRight);
    gridNPRNt->addWidget( sb3,    2, 1, Qt::AlignLeft);

    gridNPRNt->addWidget( lbl4,   3, 0, Qt::AlignRight);
    gridNPRNt->addWidget( sb4,    3, 1, Qt::AlignLeft);

    gridNPRNt->addWidget( lbl5,   4, 0, Qt::AlignRight);
    gridNPRNt->addWidget( sb5,    4, 1, Qt::AlignLeft);

    gridNPRNt->addWidget( lbl6,   5, 0, Qt::AlignRight);
    gridNPRNt->addWidget( sb6,    5, 1, Qt::AlignLeft);

    if (m_bToggle)
        gridNPRNt->addWidget( chb1,   6, 0, 1, 2, Qt::AlignLeft);

    connect( sb1, QOverload<int>::of(&QSpinBox::valueChanged), [=](int newValue){
        if (!m_bProceed) return;
        qCDebug(NPRNT) << Q_FUNC_INFO << "newValue=" << newValue << "m_bProceed=" << m_bProceed;
        uint8_t data = static_cast<uint8_t>(newValue);
        SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].commonContext.contolAndNrpnChangeContext_.ctrlMsbFreezeNumber = data;
        emit s_changed(m_curBnkNumber);
    });
    connect( sb2, QOverload<int>::of(&QSpinBox::valueChanged), [=](int newValue){
        if (!m_bProceed) return;
        qCDebug(NPRNT) << Q_FUNC_INFO << "newValue=" << newValue << "m_bProceed=" << m_bProceed;
        uint8_t data = static_cast<uint8_t>(newValue);
        SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].commonContext.contolAndNrpnChangeContext_.ctrlLsbNumber = data;
        emit s_changed(m_curBnkNumber);
    });
    connect( sb3, QOverload<int>::of(&QSpinBox::valueChanged), [=](int newValue){
        if (!m_bProceed) return;
        qCDebug(NPRNT) << Q_FUNC_INFO << "newValue=" << newValue << "m_bProceed=" << m_bProceed;
        uint8_t data = static_cast<uint8_t>(newValue);
        SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].commonContext.contolAndNrpnChangeContext_.paramMsbOnValue = data;
        emit s_changed(m_curBnkNumber);
    });
    connect( sb4, QOverload<int>::of(&QSpinBox::valueChanged), [=](int newValue){
        if (!m_bProceed) return;
        qCDebug(NPRNT) << Q_FUNC_INFO << "newValue=" << newValue << "m_bProceed=" << m_bProceed;
        uint8_t data = static_cast<uint8_t>(newValue);
        SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].commonContext.contolAndNrpnChangeContext_.paramLsbOnValue = data;
        emit s_changed(m_curBnkNumber);
    });
    connect( sb5, QOverload<int>::of(&QSpinBox::valueChanged), [=](int newValue){
        if (!m_bProceed) return;
        qCDebug(NPRNT) << Q_FUNC_INFO << "newValue=" << newValue << "m_bProceed=" << m_bProceed;
        uint8_t data = static_cast<uint8_t>(newValue);
        SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].commonContext.contolAndNrpnChangeContext_.paramMsbOffValue = data;
        emit s_changed(m_curBnkNumber);
    });
    connect( sb6, QOverload<int>::of(&QSpinBox::valueChanged), [=](int newValue){
        if (!m_bProceed) return;
        qCDebug(NPRNT) << Q_FUNC_INFO << "newValue=" << newValue << "m_bProceed=" << m_bProceed;
        uint8_t data = static_cast<uint8_t>(newValue);
        SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].commonContext.contolAndNrpnChangeContext_.paramLsbOffValue = data;
        emit s_changed(m_curBnkNumber);
    });
    if (m_bToggle){
        connect( chb1,QOverload<int>::of(&QCheckBox::stateChanged), [=](int state){
            if (!m_bProceed) return;
            qCDebug(NPRNT) << Q_FUNC_INFO << "state=" << state << "m_bProceed=" << m_bProceed;
            SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].commonContext.contolAndNrpnChangeContext_.autoSendState =
                    (Qt::Checked == state)?1:0;
            emit s_changed(m_curBnkNumber);
        });
    }

    // Relay States
    auto vbxRS  = new QVBoxLayout();
    auto gridRS = new QGridLayout();
    gbRS->setLayout(vbxRS);
    vbxRS->addLayout(gridRS);
    vbxRS->addStretch(1000);

    auto rlbl1 = new QLabel(tr("Relay 1"));
    auto rlbl2 = new QLabel(tr("Relay 2"));
    auto rlbl3 = new QLabel(tr("Relay 3"));
    auto rlbl4 = new QLabel(tr("Relay 4"));

    auto rlcbx1 = new QComboBox();
    auto rlcbx2 = new QComboBox();
    auto rlcbx3 = new QComboBox();
    auto rlcbx4 = new QComboBox();

    gridRS->addWidget(rlbl1, 0, 0 );
    gridRS->addWidget(rlbl2, 1, 0 );
    gridRS->addWidget(rlbl3, 2, 0 );
    gridRS->addWidget(rlbl4, 3, 0 );

    gridRS->addWidget(rlcbx1, 0, 1 );
    gridRS->addWidget(rlcbx2, 1, 1 );
    gridRS->addWidget(rlcbx3, 2, 1 );
    gridRS->addWidget(rlcbx4, 3, 1 );

    rlcbx1->addItem(tr("No Action"),    QVariant(RELAY_MASK_NO_ACTION));
    rlcbx1->addItem(tr("Close"),        QVariant(RELAY_MASK_CLOSE));
    rlcbx1->addItem(tr("Open"),         QVariant(RELAY_MASK_OPEN));
    rlcbx1->addItem(tr("Momentary"),    QVariant(RELAY_MASK_MOMENTARY));

    rlcbx2->addItem(tr("No Action"),    QVariant(RELAY_MASK_NO_ACTION));
    rlcbx2->addItem(tr("Close"),        QVariant(RELAY_MASK_CLOSE));
    rlcbx2->addItem(tr("Open"),         QVariant(RELAY_MASK_OPEN));
    rlcbx2->addItem(tr("Momentary"),    QVariant(RELAY_MASK_MOMENTARY));

    rlcbx3->addItem(tr("No Action"),    QVariant(RELAY_MASK_NO_ACTION));
    rlcbx3->addItem(tr("Close"),        QVariant(RELAY_MASK_CLOSE));
    rlcbx3->addItem(tr("Open"),         QVariant(RELAY_MASK_OPEN));
    rlcbx3->addItem(tr("Momentary"),    QVariant(RELAY_MASK_MOMENTARY));

    rlcbx4->addItem(tr("No Action"),    QVariant(RELAY_MASK_NO_ACTION));
    rlcbx4->addItem(tr("Close"),        QVariant(RELAY_MASK_CLOSE));
    rlcbx4->addItem(tr("Open"),         QVariant(RELAY_MASK_OPEN));
    rlcbx4->addItem(tr("Momentary"),    QVariant(RELAY_MASK_MOMENTARY));

    m_relCombos << rlcbx1 << rlcbx2 << rlcbx3 << rlcbx4;

    connect(rlcbx1, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index){
        relays(index, 0 , rlcbx1);
    });
    connect(rlcbx2, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index){
        relays(index, 2 , rlcbx2);
    });
    connect(rlcbx3, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index){
        relays(index, 4 , rlcbx3);
    });
    connect(rlcbx4, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index){
        relays(index, 6 , rlcbx4);
    });

    connect( this, &QNrpnTWidget::s_changed,    QHomeWidget::getInstance(), &QHomeWidget::_changed );
}

void QNrpnTWidget::relays( int index, int shift, QComboBox * combo ){
    qCDebug(NPRNT) << Q_FUNC_INFO << "index=" << index << "shift=" << shift << "m_bProceed=" << m_bProceed << "combo=" << hex << combo;
    if (!m_bProceed) return;
    if (Q_NULLPTR == combo)return;

    uint8_t mask = static_cast<uint8_t>(0x03 << shift);
    mask = ~mask;
    uint8_t dataALL = SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].relays;
    dataALL &= mask;

    QVariant var = combo->itemData( index);
    bool ok;
    int data = var.toInt(&ok);
    if (!ok){
        qCCritical(NPRNT) << Q_FUNC_INFO << "can't convert to int!";
        return;
    }
    uint8_t _data = static_cast<uint8_t>(data);
    _data <<= shift;
    uint8_t res  = _data | dataALL;
    SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].relays = res;

    emit s_changed(m_curBnkNumber);
}

QNrpnTWidget::~QNrpnTWidget(){
    qCDebug(NPRNT) << Q_FUNC_INFO;
}

QNrpnTWidget * QNrpnTWidget::getInstance( bool toggle ){
    if (toggle) return(g_NRPNt);
    return(g_NRPNm);
}

void QNrpnTWidget::setButton(int btnNumber, int curBnkNumber){
    qCDebug(NPRNT) << Q_FUNC_INFO << "btnNumber=" << btnNumber << "curBnkNumber=" << curBnkNumber;
    if ( curBnkNumber<0 ){
        qCCritical(NPRNT) << Q_FUNC_INFO << "got bad bank number!";
        return;
    }
    if (!(( btnNumber>=1 ) && ( btnNumber<=FOOT_BUTTONS_NUM ))){
        qCCritical(NPRNT) << Q_FUNC_INFO << "got bad button number!";
        return;
    }
    m_btnNumber     = btnNumber;
    m_curBnkNumber  = curBnkNumber;

    SSXMSGS::ButtonType buttonType = SSXMSGS::g_BanksSettings.at(m_curBnkNumber).buttonType[m_btnNumber-1];
    if ( m_bToggle && ( buttonType != SSXMSGS::ButtonType::NRPN_TOGGLE) ){
        qCCritical(NPRNT) << Q_FUNC_INFO << "got bad button type for NRPN_TOGGLE!";
        return;
    }
    if ((!m_bToggle) && (buttonType != SSXMSGS::ButtonType::NRPN_MOMENTARY)){
        qCCritical(NPRNT) << Q_FUNC_INFO << "got bad button type for NRPN_MOMENTARY!";
        return;
    }

    ////// start of init ////
    m_bProceed=false;
    /// установка CC#99
    auto sb1 = m_spinBoxes.at(0);
    uint8_t value1 =
            SSXMSGS::g_BanksSettings.at(m_curBnkNumber).buttonContext[m_btnNumber-1].commonContext.contolAndNrpnChangeContext_.ctrlMsbFreezeNumber;
    if ( value1 > 127 ){
        value1 = 127;
        SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].commonContext.contolAndNrpnChangeContext_.ctrlMsbFreezeNumber = 127;
    }
    sb1->setValue(value1);
    /// установка CC#98
    auto sb2 = m_spinBoxes.at(1);
    uint8_t value2 =
            SSXMSGS::g_BanksSettings.at(m_curBnkNumber).buttonContext[m_btnNumber-1].commonContext.contolAndNrpnChangeContext_.ctrlLsbNumber;
    if ( value2 > 127 ){
        value2 = 127;
        SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].commonContext.contolAndNrpnChangeContext_.ctrlLsbNumber = 127;
    }
    sb2->setValue(value2);
    /// установка CC#6 ON value
    auto sb3 = m_spinBoxes.at(2);
    uint8_t value3 =
            SSXMSGS::g_BanksSettings.at(m_curBnkNumber).buttonContext[m_btnNumber-1].commonContext.contolAndNrpnChangeContext_.paramMsbOnValue;
    if ( value3 > 127 ){
        value3 = 127;
        SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].commonContext.contolAndNrpnChangeContext_.paramMsbOnValue = 127;
    }
    sb3->setValue(value3);
    /// установка CC#38 ON value
    auto sb4 = m_spinBoxes.at(3);
    uint8_t value4 =
            SSXMSGS::g_BanksSettings.at(m_curBnkNumber).buttonContext[m_btnNumber-1].commonContext.contolAndNrpnChangeContext_.paramLsbOnValue;
    if ( value4 > 127 ){
        value4 = 127;
        SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].commonContext.contolAndNrpnChangeContext_.paramLsbOnValue = 127;
    }
    sb4->setValue(value4);
    /// установка CC#6 OFF value
    auto sb5 = m_spinBoxes.at(4);
    uint8_t value5 =
            SSXMSGS::g_BanksSettings.at(m_curBnkNumber).buttonContext[m_btnNumber-1].commonContext.contolAndNrpnChangeContext_.paramMsbOffValue;
    if ( value5 > 127 ){
        value5 = 127;
        SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].commonContext.contolAndNrpnChangeContext_.paramMsbOffValue = 127;
    }
    sb5->setValue(value5);
    /// установка CC#38 OFF value
    auto sb6 = m_spinBoxes.at(5);
    uint8_t value6 =
            SSXMSGS::g_BanksSettings.at(m_curBnkNumber).buttonContext[m_btnNumber-1].commonContext.contolAndNrpnChangeContext_.paramLsbOffValue;
    if ( value6 > 127 ){
        value6 = 127;
        SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].commonContext.contolAndNrpnChangeContext_.paramLsbOffValue = 127;
    }
    sb6->setValue(value6);

    if (m_bToggle){
        /// set "Send value when preset changed"
        auto chb1 = m_checkBoxes.at(0);
        uint8_t autoSendState   =
                SSXMSGS::g_BanksSettings.at(m_curBnkNumber).buttonContext[m_btnNumber-1].commonContext.contolAndNrpnChangeContext_.autoSendState;
        chb1->setChecked( 0!= autoSendState );
    }
    //set up relays
    int i = 0;
    for( auto combo: m_relCombos ){
        uint8_t data    = SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].relays;
        int     res     = (data >> i) & 0x03;
        int index = combo->findData(QVariant(res));
        combo->setCurrentIndex(index);
        i+=2;
    }

    if (m_bToggle){
        uint8_t data = SSXMSGS::g_BanksSettings[m_curBnkNumber].buttonContext[m_btnNumber-1].commonContext.contolAndNrpnChangeContext_.vendorBlockId;
        int index = m_vbxVID->findData(QVariant(data));
        m_vbxVID->setCurrentIndex( index );
    }
    ////// end of init /////
    m_bProceed=true;

    qCDebug(NPRNT) << Q_FUNC_INFO << "m_bProceed=" << m_bProceed;
}


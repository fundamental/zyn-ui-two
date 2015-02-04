import QtQuick 2.2
import ZynAddSubFX 1.0
import "qrc:/qml/"

Blank {
    VLayout {
        anchors.fill: parent
        weights: [0.1,0.8,0.1]
        HLayout {
            Logo {}
            MainMenu {}
            StatusPane {}
            ControlPane {}
        }
        HLayout {
            weights: [0.1,0.9]
            VLayout {
                Box {label: "part settings"}
                SelectorGrid { n: 16}
                Box {label: "browser"}
                Box {label: "mixer"}
                Box {label: "kit editor"}
                SelectorGrid { n: 16 }
                Box {label: "arp"}
                Box {label: "effects"}
                Box {label: "adsynth"}
                SelectorGrid {
                    n: 8
                    label: "voices"
                }
                Box {label: "subsynth"}
                Box {label: "padsynth"}
            }
            AdVoice {}
        }
        HLayout {
            weights: [0.05,0.55,0.05,0.05,0.05,0.05,0.05,0.05,0.1]
            ModWheel {}
            Keyboard {}
            Knob {label: "velocity"}
            Knob {label: "vrand"}
            Knob {label: "octave"}
            Knob {label: "qwer"}
            Knob {label: "zxcv"}
            Knob {label: "c.val"}
            DropDown {
                label: "controller"
                text: "74: filter frequency"
            }

        }
    }
}

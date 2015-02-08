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
            ZLayout {
                vertical: true
                Box {
                    label: "part settings"
                    aspect: 0.3
                    zscale:  0.3
                }
                SelectorGrid { n: 16}
                    
                Box {label: "browser";aspect: 0.3; zscale:  0.3}
                Box {label: "mixer";aspect: 0.3; zscale:  0.3}
                Box {label: "kit editor";aspect: 0.3; zscale:  0.3}
                SelectorGrid { n: 16 }
                Box {label: "arp";aspect: 0.3; zscale:  0.3}
                Box {label: "effects";aspect: 0.3; zscale:  0.3}
                Box {label: "adsynth";aspect: 0.3; zscale:  0.3}
                SelectorGrid {
                    n: 8
                    label: "voices"
                }
                Box {label: "subsynth";aspect: 0.3; zscale:  0.3}
                Box {label: "padsynth";aspect: 0.3; zscale:  0.3}
            }
            AdVoice {}
        }
        ZLayout {
            ModWheel {}
            Keyboard {
                property variant aspect: 1.0/15.5
                property variant zscale: 1
            }
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

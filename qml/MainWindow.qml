import QtQuick 2.2
import ZynAddSubFX 1.0
import "qrc:/qml/"

Background {
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
            MainSidebar {}
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

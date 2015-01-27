import QtQuick 2.2
import ZynAddSubFX 1.0
import "qrc:/qml/"

VLayout {
    Module {
        label: "amplitude"
        AltKnob {
            label: "vol"
        }
        AltKnob {
            label: "v.sns"
        }
        Knob {
            label: "pan"
        }
    }
    Module {
        label: "amplitude envelope"
        copyable: true
        editable: true
        Repeater {
            model: ["a.dt", "d.dt", 
            "s.val", "r.dt", "stretch"]
            Knob { label: modelData }
        }
        Button {
            label: "frcr"
        }
        Button {
            label: "l"
        }
    }
}

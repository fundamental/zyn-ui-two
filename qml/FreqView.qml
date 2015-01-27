import QtQuick 2.2
import ZynAddSubFX 1.0
import "qrc:/qml/"

VLayout {
    Module {
        label: "frequency"
        AltKnob {
            label: "detune"
        }
        AltKnob {
            label: "octave"
        }
        Button {
            label: "440Hz"
        }
        AltKnob {
            label: "eq.t."
        }
        DropDown {
            label: "det. type"
            text:  "e1200c."
        }
        AltKnob {
            label: "c. det."
        }
    }
    Module {
        label: "frequency envelope"
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
    }
}

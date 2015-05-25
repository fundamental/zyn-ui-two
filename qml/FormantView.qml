import QtQuick 2.2
import ZynAddSubFX 1.0
import "qrc:/qml/"

ZLayout {
    anchors.fill: parent
    Graph {
        expandable: true
        aspect: 0.4
    }

    ZLayout {
        vertical: true
        ZLayout {
            Knob { label: "num. form" }
            Knob { label: "fr.sl"}
            Knob { label: "vw.cl"}
            Div  {}
            Knob { label: "c.f." }
            Knob { label: "oct."}
            expandable: true
        }
        Module {
            label: "vowel parameters"
            copyable: true
            Knob { label: "vowel no." }
            Knob { label: "formant" }
            Div  {}
            Knob { label: "freq" }
            Knob { label: "q" }
            Knob { label: "amp" }
            expandable: true
        }
        Module {
            label: "sequence parameters"
            Knob { label: "seq. size" }
            Knob { label: "s. pos" }
            Knob { label: "vowel" }
            Knob { label: "strch" }
            Button { label: "neg. input" }
            expandable: true
        }
    }

}

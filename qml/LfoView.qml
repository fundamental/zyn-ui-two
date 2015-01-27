import QtQuick 2.2
import ZynAddSubFX 1.0
import "qrc:/qml/"

Module {
    copyable: true
    Knob { label: "freq." }
    Knob { label: "depth" }
    Knob { label: "start" }
    Knob { label: "delay" }
    Knob { label: "str" }
    Knob { label: "a.r." }
    Knob { label: "f.r." }
    DropDown {
        text: "sine"
        label: "type"
    }
    Button {
        label: "c."
    }
}

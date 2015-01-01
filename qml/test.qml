import QtQuick 2.2
import ZynAddSubFX 1.0
import "qrc:/qml/"


HLayout {
    id: layout
    anchors.fill: parent
    Knob {}
    Graph {}
    Button {}
    EnvView {}
        HLayout {
            id: row
            VSlider {}
            VSlider {}
            VSlider {}
            VSlider {}
    }
    VLayout {
        HSlider {}
        HSlider {}
        HSlider {}
        HSlider {}
    }
    DropDown {}
}

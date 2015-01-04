import QtQuick 2.2
import ZynAddSubFX 1.0
import "qrc:/qml/"


Blank {
    HALayout {
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
            Knob {}
            HSlider {}
            HSlider {}
            HSlider {}
        }
        DropDown {}
    }
}

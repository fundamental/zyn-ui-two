import QtQuick 2.2
import ZynAddSubFX 1.0
import "qrc:/qml/"


Blank {
    ZLayout {
        id: layout
        anchors.fill: parent
        Knob {
            zscale: 2.0
        }
        Graph {
            zscale: 2
            aspect: 2
        }
        PowButton {}
        Button {
            zscale: 0.333
            aspect: 0.333
        }
        EnvView {
            expandable: true
            zscale: 4
        }
        HLayout {
            property real zscale: 5
            property real aspect: 10
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
        DropDown {
            aspect: 0.333
        }
        TextList {
            zscale: 3
            aspect: 3
        }
    }
}

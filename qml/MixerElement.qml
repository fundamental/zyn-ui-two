import QtQuick 2.2
import ZynAddSubFX 1.0
import "qrc:/qml/"

ZLayout {
    vertical: true
    Box {
        label: "1"
        zscale: 0.333
        aspect: 0.333
    }
    Button {
        label: "Digital\nPiano"
    }
    Box {
        label: "80"
        zscale: 0.333
        aspect: 0.333
    }
    HLayout {
        property bool expandable: true
        VBar {
            t: 0.5
        }
        VBar {
            t: 0.3
        }
        VAltSlider {
            t: 0.5
        }
    }
    PanKnob {
        t: 0.0
    }
    DropDown {
        label: "Part 1"
    }
    Button {
        label: "Edit"
    }
}

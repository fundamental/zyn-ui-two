import QtQuick 2.0
import ZynAddSubFX 1.0
import QtQuick.Layouts 1.1
import "qrc:/qml/"


RowLayout {
    id: layout
    anchors.fill: parent
    spacing: 6
    Knob {
        Layout.fillWidth: true
        Layout.fillHeight: true
    }
    Graph {
        Layout.fillWidth: true
        Layout.fillHeight: true
    }
    Button {
        Layout.fillWidth: true
        Layout.fillHeight: true
    }
    EnvView {
        Layout.fillWidth: true
        Layout.fillHeight: true
    }
    VSlider {
        Layout.fillWidth: true
        Layout.fillHeight: true
    }
    HSlider {
        Layout.fillWidth: true
        Layout.fillHeight: true
    }
    DropDown {
        Layout.fillWidth: true
        Layout.fillHeight: true
    }
}

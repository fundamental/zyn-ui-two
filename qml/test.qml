import QtQuick 2.0
import ZynAddSubFX 1.0
import QtQuick.Layouts 1.1
import "qrc:/qml/"


RowLayout {
    id: layout
    anchors.fill: parent
    spacing: 6
    Graph {
        Layout.fillWidth: true
        Layout.fillHeight: true
    }
    Knob {
        Layout.fillWidth: true
        Layout.fillHeight: true
    }
    Knob {
        Layout.fillWidth: true
        Layout.fillHeight: true
    }
}

import QtQuick 2.2
import ZynAddSubFX 1.0
import "qrc:/qml/"

Blank {
    VLayout {
        anchors.fill: parent
        Repeater {
            model: 10
            KitRow {
                property bool expandable: true;
            }
        }
    }
}

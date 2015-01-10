import QtQuick 2.2
import ZynAddSubFX 1.0
import "qrc:/qml/"

Blank {
    HLayout {
        anchors.fill: parent
        Repeater {
            model: 10
            MixerElement {
                property bool expandable: true;
            }
        }
    }
}

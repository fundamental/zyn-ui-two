import QtQuick 2.2
import ZynAddSubFX 1.0
import "qrc:/qml/"

Background {
    VLayout {
        anchors.fill: parent
        weights: [0.1,0.8,0.1]
        HLayout {
            Logo {}
            MainMenu {}
            StatusPane {}
            ControlPane {}
        }
        HLayout {
            weights: [0.1,0.9]
            MainSidebar {}
            AdVoice {}
        }
        MainFooter {}
    }
}

import QtQuick 2.2
import ZynAddSubFX 1.0
import "qrc:/qml/"

Blank {
    VLayout {
        anchors.fill: parent
        HLayout {
            AmpView    {}
            FreqView   {}
            FilterView {}
        }
        FormantView {}
        HLayout {
            LfoView { label: "amplitude lfo" }
            LfoView { label: "frequency lfo" }
            LfoView { label: "filter    lfo" }
        }
    }
}

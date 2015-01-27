import QtQuick 2.2
import ZynAddSubFX 1.0
import "qrc:/qml/"

VLayout {
    HLayout {
        AmpView    {}
        FreqView   {}
        FilterView {}
    }
    EnvEdit {}
    HLayout {
        LfoView { label: "amplitude lfo" }
        LfoView { label: "frequency lfo" }
        LfoView { label: "filter    lfo" }
    }
}

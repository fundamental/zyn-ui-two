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
        BandwidthModule {}
        Module {
            title: "Misc"
            Button {
                label: "stereo"
            }
            Button {
                label: "harmonic settings"
            }
            Button {
                label: "clear"
            }
            AltKnob {
                label: "filter stages"
            }
            DropDown {
                title: "mag. type"
                label: "linear"
            }
            DropDown {
                title: "start"
                label: "random"
            }
        }
    }
}

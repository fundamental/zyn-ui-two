import ZynAddSubFX 1.0
import "qrc:/qml/"

ZLayout {
    ModWheel {}
    Keyboard {
        aspect: 1/20.0
        zscale: 2.0
    }
    Knob {label: "velocity"}
    Knob {label: "vrand"}
    Knob {label: "octave"}
    Knob {label: "qwer"}
    Knob {label: "zxcv"}
    Knob {label: "c.val"}
    DropDown {
        label: "controller"
        text: "74: filter frequency"
    }

}

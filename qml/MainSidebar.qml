import ZynAddSubFX 1.0
import "qrc:/qml/"

ZLayout {
    vertical: true
    Button {
        label: "part settings"
        renderer: "relaxed"
        zscale:  0.3
        expandable: true
    }
    SelectorGrid { n: 16}

    Button {label: "browser";renderer: "relaxed";zscale:  0.3;expandable: true}
    Button {label: "mixer";renderer: "relaxed";zscale:  0.3;expandable: true}
    Button {label: "kit editor";renderer: "relaxed";zscale:  0.3;expandable: true}
    SelectorGrid { n: 16 }
    Button {label: "arp";renderer: "relaxed";zscale:  0.3;expandable: true}
    Button {label: "effects";renderer: "relaxed";zscale:  0.3;expandable: true}
    Button {label: "adsynth";renderer: "relaxed";zscale:  0.3;expandable: true}
    SelectorGrid {
        n: 8
        label: "voices"
    }
    Button {label: "subsynth";renderer: "relaxed";zscale:  0.3;expandable: true}
    Button {label: "padsynth";renderer: "relaxed";zscale:  0.3;expandable: true}
}

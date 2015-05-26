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
    SelectorGrid { aspect: 1;n: 16}

    Button {label: "browser";renderer: "relaxed";zscale:  0.3;expandable: true}
    Button {label: "mixer";renderer: "relaxed";zscale:  0.3;expandable: true}
    Button {label: "kit editor";renderer: "relaxed";zscale:  0.3;expandable: true}
    SelectorGrid { aspect: 1;n: 16 }
    Button {label: "arp";renderer: "relaxed";zscale:  0.3;expandable: true}
    Button {label: "effects";renderer: "relaxed";zscale:  0.3;expandable: true}
    ZLayout {
        PowButton {}
        Button {label: "adsynth";renderer: "relaxed";zscale:  0.3;expandable: true}
        zscale: 0.3
    }
    SelectorGrid {
        aspect: 1;
        n: 8
        label: "voices"
    }
    ZLayout {
        PowButton {}
        Button {label: "subsynth";renderer: "relaxed";expandable: true}
        zscale: 0.3
    }
    ZLayout {
        PowButton {}
        Button {label: "padsynth";renderer: "relaxed";expandable: true}
        zscale: 0.3
    }
}

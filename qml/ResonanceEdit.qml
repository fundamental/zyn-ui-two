VLayout {
    ResonanceView{}
    HLayout{
        Button {label: "enable"}
        AltKnob {label: "max"}
        AltKnob {label: "c.f."}
        VLayout {
            Button {label: "interp"}
            Button {label: "p.1 st"}
        }
        Button { label: "zero" }
        Button { label: "smooth"}
        Button { label: "random1"}
        Button { label: "random2"}
        Button { label: "random3"}
        TextLine { label: "0.17 kHz, -10.2dB"}
    }
}

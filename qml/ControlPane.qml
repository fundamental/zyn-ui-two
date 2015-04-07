HLayout {
    VLayout {
        HSlider {text: "fine detune"}
        Button  {label: "receive nrpns"}
    }
    VLayout {
        HSlider {text: "master vol"}
        HSlider {text: "key shift"}
    }
    DbMeter {}
    Button {renderer: "icon-panic"}
}

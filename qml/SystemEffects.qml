HLayout {
    VLayout {
        DropDown {text: "reverb"}
        HSlider  {text: "1"}
        DropDown {text: "echo"}
        HSlider  {text: "2"}
        DropDown {text: "chorus"}
        HSlider  {text: "3"}
        DropDown {text: "phaser"}
        HSlider  {text: "4"}
        DropDown {text: "alienwah"}
        HSlider  {text: "5"}
        DropDown {text: "distortion"}
        HSlider  {text: "6"}
    }
    VLayout{
        ReverbView     {}
        EchoView       {}
        DistortionView {}
        SendToGrid {}
    }
}

import ZynAddSubFX 1.0
import "qrc:/qml/"

Blank {
HLayout {
    VLayout {
        TextLine {label: "search..."}
        HLayout {
            OptionList {
                //resetable: true
                label: "bank"
            }
            OptionList {
                label: "type"
            }
            OptionList {
                label: "sub type"
            }
        }
    }
    VLayout {
        MenuBar {
            Radio {
                //options: ["read", "write"]
            }
        }
        HLayout {
            OptionList {
                numbered: true
                label: "preset name"
            }
            VLayout {
                TextBox {
                    label: "author"
                }
                TextBox {
                    label: "comment"
                }
                TextBox {
                    label: "copyright"
                }
            }
        }
    }
}
}

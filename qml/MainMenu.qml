import ZynAddSubFX 1.0
import "qrc:/qml/"

ZGrid {
    rows: 2
    cols: 3
    DropDown { text: "file" }
    HLayout {
        Button {}
        Button {}
    }
    Button {}
    Button {}
    Button {label: "midi learn"}
    Button {label: "aquire"}
}

import ZynAddSubFX 1.0
import "qrc:/qml/"

ZGrid {
    rows: 2
    cols: 3
    DropDown { text: "file" }
    Button {renderer: "icon-keyboard"}
    Button {label: "midi learn"}
    ZLayout {
        Button {renderer: "icon-record"}
        Button {renderer: "icon-pause"}
    }
    Button {renderer: "icon-stop"}
    Button {label: "aquire"}
}

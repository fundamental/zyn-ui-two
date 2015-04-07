import ZynAddSubFX 1.0
import "qrc:/qml/"

ZGrid {
    rows: 2
    cols: 3
    DropDown { text: "file" }
    Button {}
    Button {label: "midi learn"}
    HLayout {
        Button {renderer: "icon-record"}
        Button {renderer: "icon-pause"}
    }
    Button {renderer: "icon-stop"}
    Button {label: "aquire"}
}

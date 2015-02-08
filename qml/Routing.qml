import QtQuick 2.2
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.2
import ZynAddSubFX 1.0
import "qrc:/qml/"

ZGrid {
    id : gridmain
    rows: 10
    cols: 10

    Repeater {
        id:gridgenerate
        model: 100
        HSlider {}
    }
}

import QtQuick 1.0
import ZynAddSubFX 1.0
import "qrc:/qml/"



Rectangle {
    color: 'black'
    width: 400
    height: 100
    Row {
        id: positioner
        width:400
        height:100
        Knob {
            width: 100
            height: 100
        }
        Graph {
            width: 100
            height: 100
        }
        Knob {
            width: 100
            height: 100
        }
        Knob {
            width: 100
            height: 100
        }
        Knob {
            width: 100
            height: 100
        }
    }
}

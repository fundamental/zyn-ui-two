import QtQuick 1.0
import MDEPlugins 1.0
import "qml/"



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
        Knob {
            width: 100
            height: 100
        }
    }
}

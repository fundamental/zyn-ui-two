import QtQuick 2.2
import ZynAddSubFX 1.0
import "qrc:/qml/"

Blank{
    ZLayout {
        anchors.fill: parent
        Button {
            label: "1"
            aspect: 1
            zscale: 1
        }
        TextField {
            label: "Kit Name"
            expandable: true
        }
        Div {
            zscale: 0
        }
        HSlider {
            label: "100"
            expandable: true
        }
        Div {
            zscale: 0
        }
        Button {
            label: "M"
            aspect: 1
            zscale: 1
        }
        Button {
            label: "R"
            aspect: 1
            zscale: 1
        }
        Button {
            label: "M"
            aspect: 1
            zscale: 1
        }
        Div {
            zscale: 0
        }
        HSlider {
            label: "100"
            expandable: true
        }
        Div {
            zscale: 0
        }
        PowerButton {}
        Button {
            label: "edit"
            aspect: 0.5 
            zscale: 1
        }
        Div {
            zscale: 0
        }
        PowerButton {}
        Button {
            label: "edit"
            aspect: 0.5 
            zscale: 1
        }
        Div {
            zscale: 0
        }
        PowerButton {}
        Button {
            label: "edit"
            aspect: 0.5 
            zscale: 1
        }
        Div {
            zscale: 0
        }
        DropDown {
            label: "off"
            aspect: 0.5
            zscale: 1.0
        }
    }
}

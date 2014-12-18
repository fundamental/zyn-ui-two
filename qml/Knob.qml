import QtQuick 1.0
import MDEPlugins 1.0


MDE {
    id: knob
    height: 100
    width:  100
    function releaseSwitch()
    {
        console.log("move...", mouse.mouseY)
    }
    function updateValue(norm_dy)
    {
        var tmp = knob.value - norm_dy
        if(tmp > 1) {
            tmp = 1
        } else if(tmp < 0) {
            tmp = 0
        }
        knob.value = tmp
    }
    MouseArea {
        id: mouse
        drag.axis: Drag.YAxis;
        drag.minimumX: 1;
        drag.maximumX: 78
        onClicked: toggle()
        anchors.fill: parent
        property variant previousPosition        
        onPressed: {
            previousPosition = Qt.point(mouseX, mouseY)
        }
        onPositionChanged: {
            if (pressedButtons == Qt.LeftButton)
            {
                var dx = mouseX - previousPosition.x
                var dy = mouseY - previousPosition.y
                previousPosition = Qt.point(mouseX, mouseY)
                console.log(dx, " ",  dy)
                updateValue(dy/200.0)
            }
        }
    }
}


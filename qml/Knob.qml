import QtQuick 2.0
import ZynAddSubFX 1.0


Knob {
    id: knob
    function updateValue(norm_dy)
    {
        console.log("Pos = ", knob.x, " ", knob.y)
        var tmp = knob.t - norm_dy
        if(tmp > 1) {
            tmp = 1
        } else if(tmp < 0) {
            tmp = 0
        }
        knob.t = tmp
    }
    MouseArea {
        id: mouse
        drag.axis: Drag.YAxis;
        drag.minimumX: 1;
        drag.maximumX: 78
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


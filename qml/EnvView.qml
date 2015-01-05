import QtQuick 2.0
import ZynAddSubFX 1.0

EnvView {
    id: env
    MouseArea {
        anchors.fill: parent
        property variant previousPosition
        property variant grabbedElm: -1
        onPressed: {
            var pos = Qt.point(mouseX/width, 1-2*mouseY/height)
            previousPosition = pos
            grabbedElm = env.objNear(pos)
        }
        onPositionChanged: {
            if (pressedButtons == Qt.LeftButton)
            {
                var dx = (mouseX/width - previousPosition.x)
                var dy = ((1-2*mouseY/height) - previousPosition.y)
                previousPosition = Qt.point(mouseX/width, 1-2*mouseY/height)
                if(grabbedElm != -1) {
                    env.objMove(grabbedElm, Qt.point(dx,dy))
                }
            }
        }
    }
}

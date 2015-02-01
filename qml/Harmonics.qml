import QtQuick 2.0
import ZynAddSubFX 1.0

Harmonics {
    id: harmonics
    MouseArea {
        anchors.fill: parent
        property variant previousPosition
        property variant grabbedElm: -1
        property variant lastClick: new Date()
        property bool isDoubleClick: false
        onPressed: {
            var now = new Date()
            var deltaT = now-lastClick
            lastClick = now
            var pos = Qt.point(mouseX/width, 1-mouseY/height)
            previousPosition = pos
            grabbedElm = harmonics.objNear(pos)
            isDoubleClick = deltaT < 300//ms
        }
        onPositionChanged: {
            if(pressedButtons == Qt.LeftButton && !isDoubleClick)
            {
                var dx = (mouseX/width - previousPosition.x)
                var dy = ((1-mouseY/height) - previousPosition.y)
                previousPosition = Qt.point(mouseX/width, 1-mouseY/height)
                if(grabbedElm != -1) {
                    harmonics.objMove(grabbedElm, Qt.point(dx,dy))
                }
            } else if(pressedButtons == Qt.LeftButton) {
                var pos = Qt.point(mouseX/width, 1-mouseY/height)
                grabbedElm = harmonics.objNear(pos)
                if(grabbedElm != -1)
                harmonics.objSet(grabbedElm, 1-mouseY/height)
            }
        }
    }
}

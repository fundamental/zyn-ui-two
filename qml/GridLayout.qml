import QtQuick 2.0

Item {
    id: layout

    onChildrenChanged: performLayout()
    onWidthChanged: performLayout()
    onHeightChanged: performLayout()
    onXChanged: performLayout()
    onYChanged: performLayout()
    property variant cols: -1
    property variant rows: -1

    function performLayout() {
        var currentX = 0
        var N = layout.children.length

        for (var i = 0; i < N; ++i) {
            var obj = layout.children[i]
            obj.x      = currentX
            obj.y      = 0
            obj.width  = layout.width/N
            obj.height = layout.height
            currentX  += layout.width/N
        }
    }
}

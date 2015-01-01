import QtQuick 2.0

Item {
    id: layout

    onChildrenChanged: performLayout()
    onWidthChanged: performLayout()
    onHeightChanged: performLayout()

    function performLayout() {
        var currentY = layout.y
        var N = layout.children.length

        for (var i = 0; i < N; ++i) {
            var obj = layout.children[i]
            obj.y      = currentY
            obj.width  = layout.width
            obj.height = layout.height/N
            obj.x      = layout.x
            currentY  += layout.height/N
        }

    }
}

import QtQuick 2.0

Item {
    id: layout

    onChildrenChanged: performLayout()
    onWidthChanged: performLayout()
    onHeightChanged: performLayout()

    function performLayout() {
        var currentX = layout.x
        var N = layout.children.length

        for (var i = 0; i < N; ++i) {
            var obj = layout.children[i]
            obj.x      = currentX
            obj.width  = layout.width/N
            obj.height = layout.height
            obj.y      = layout.y
            currentX  += layout.width/N
        }
    }
}

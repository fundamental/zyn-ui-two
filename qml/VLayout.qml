import QtQuick 2.0

Item {
    id: layout

    onChildrenChanged: performLayout()
    onWidthChanged: performLayout()
    onHeightChanged: performLayout()
    onXChanged: performLayout()
    onYChanged: performLayout()

    function performLayout() {
        var N = layout.children.length

        for (var i = 0; i < N; ++i) {
            var obj = layout.children[i]
            obj.y      = layout.height*(N-i-1)/N
            obj.x      = 0
            obj.width  = layout.width
            obj.height = layout.height/N
        }

    }
}

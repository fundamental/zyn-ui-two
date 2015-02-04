import QtQuick 2.0

Item {
    id: layout

    onChildrenChanged: performLayout()
    onWidthChanged: performLayout()
    onHeightChanged: performLayout()
    onXChanged: performLayout()
    onYChanged: performLayout()

    property variant weights: [1.0]

    function performLayout() {
        var N = layout.children.length
        var iWeights = weights;
        if(iWeights.length != N) {
            iWeights = []
            for (var i = 0; i < N; ++i) {
                iWeights.push(1.0/N)
            }
        }

        var current = 0
        for (var i = 0; i < N; ++i) {
            var obj = layout.children[i]
            obj.y      = current*layout.height
            obj.x      = 0
            obj.width  = layout.width
            obj.height = layout.height*iWeights[i]
            current   += iWeights[i]
        }

    }
}

import QtQuick 1.1
import com.nokia.symbian 1.1

Item {
    id: picker

    property int itemHeight: height / 3
    property alias model: view.model
    property alias currentIndex: view.currentIndex

    Rectangle {
        anchors.fill: parent
        radius: 5
        smooth: true
        gradient: Gradient {
            GradientStop { position: 0; color: "#202020" }
            GradientStop { position: 0.5; color: "#808080" }
            GradientStop { position: 1; color: "#000000" }
        }
    }

    PathView {
        id: view
        anchors.fill: parent
        clip: true

        onModelChanged: pathItemCount = 5

        delegate: Label {
            opacity: PathView.opacity
            font.pixelSize: 30
            text: index < 10 ? "0" + index : index
        }

        highlight: Rectangle {
            width: view.width
            height: itemHeight
            color: "black"
            opacity: 0.7
        }

        dragMargin: view.width
        preferredHighlightBegin: 0.5
        preferredHighlightEnd: 0.5
        highlightRangeMode: PathView.StrictlyEnforceRange

        path: Path {
            startX: view.width / 2
            startY: -itemHeight

            PathAttribute { name: "opacity"; value: 0 }
            PathLine { x: view.width / 2; y: 0 }
            PathAttribute { name: "opacity"; value: 0.1 }
            PathLine { x: view.width / 2; y: view.height / 2 }
            PathAttribute { name: "opacity"; value: 1 }
            PathLine { x: view.width / 2; y: view.height }
            PathAttribute { name: "opacity"; value: 0.1 }
            PathLine { x: view.width / 2; y: view.height + itemHeight }
            PathAttribute { name: "opacity"; value: 0 }
        }
    }
}

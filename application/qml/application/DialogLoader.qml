import QtQuick 1.1

Loader {
    id: root
    onLoaded: item.open()

    Connections {
        target: root.item
        onAccepted: closeTimer.start()
        onRejected: closeTimer.start()
    }

    Timer {
        id: closeTimer
        interval: 500
        onTriggered: root.source = ""
    }
}

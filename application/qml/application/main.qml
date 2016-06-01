import QtQuick 1.1
import com.nokia.symbian 1.1

PageStackWindow {
    Item {
        id: splash
        anchors.fill: parent

        Rectangle {
            anchors.fill: parent
            color: "black"
        }

        Image {
            anchors.fill: parent
            anchors.margins: 120
            source: "qrc:/gfx/application/situations.svg"
            fillMode: Image.PreserveAspectFit
            sourceSize.width: Math.min(width, height)
            sourceSize.height: Math.min(width, height)
            smooth: true
        }

        MouseArea {
            anchors.fill: parent
            enabled: splash.opacity != 0
        }

        Behavior on opacity { NumberAnimation{ duration: 250 } }
    }

    Connections {
        target: client
        onConnected: client.reqPlugins()
        onPluginsUpdated: client.reqSituations()
        onSituationsUpdated: {
            pageStack.push("qrc:/qml/application/MainPage.qml")
            splashTimer.start()
        }
    }

    Timer {
        interval: 1
        Component.onCompleted: start()
        onTriggered: client.connectToServer()
    }

    Timer {
        id: splashTimer
        interval: 100
        onTriggered: splash.opacity = 0
    }
}


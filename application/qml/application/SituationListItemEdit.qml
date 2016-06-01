import QtQuick 1.1
import com.nokia.symbian 1.1

Item {
    id: editLayer

    Image {
        anchors.fill: parent
        source: "qrc:/gfx/application/EditOverlay.svg"
        fillMode: Image.TileHorizontally
        smooth: true
    }

    MouseArea {
        anchors.fill: parent
        onClicked: editClicked()
    }

    Column {
        id: priorityButtons
        height: parent.height
        anchors.verticalCenter: parent.verticalCenter

        ToolButton {
            width: 80
            height: priorityButtons.height / 2
            enabled: index > 1
            flat: true
            iconSource: enabled ? "qrc:/gfx/application/IconUp.svg" : ""
            onClicked: client.reqMoveSituation(index, index - 1)
        }

        ToolButton {
            width: 80
            height: priorityButtons.height / 2
            enabled: index > 0 && index < (repeater.count - 1)
            flat: true
            iconSource: enabled ? "qrc:/gfx/application/IconDown.svg" : ""
            onClicked: client.reqMoveSituation(index + 1, index)
        }
    }

    Row {
        spacing: 10
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter

        ToolButton {
            width: 50
            height: 50

            visible: application.advancedMode
            iconSource: "toolbar-settings"
            onClicked: settingsDialog.open()
        }

        ToolButton {
            width: 50
            height: 50

            enabled: index > 0
            iconSource: enabled ? "toolbar-delete" : ""
            onClicked: client.reqRmvSituation(index)
        }
    }
}


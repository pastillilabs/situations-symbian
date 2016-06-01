import QtQuick 1.1
import com.nokia.symbian 1.1

Item {
    property int itemIndex: index
    property alias content: contentItem.children
    property alias clickEnabled: mouseArea.enabled

    signal clicked
    signal removeClicked

    height: 70

    onClicked: if(groupEditor) editLoader.source = groupEditor

    Rectangle {
        anchors.fill: parent
        opacity: mouseArea.pressed ? 1 : 0
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#1080DD" }
            GradientStop { position: 1.0; color: "#53A4E7" }
        }
    }

    Rectangle {
        width: parent.width
        height: 1
        anchors.bottom: parent.bottom
        color: "#404040"
    }

    Item {
        id: contentItem
        anchors.fill: parent
        anchors.rightMargin: buttonArea.width
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: {
            parent.clicked()
            privateStyle.play(Symbian.BasicButton)
        }
        onPressed: privateStyle.play(Symbian.BasicButton)
    }

    Row {
        id: buttonArea
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter

        ToolButton {
            width: 50
            height: 50
            anchors.verticalCenter: parent.verticalCenter
            visible: application.advancedMode && (groupSettings ? true : false)

            iconSource: "toolbar-settings"
            onClicked: editLoader.source = groupSettings
        }

        ToolButton {
            width: 50
            height: 50
            anchors.verticalCenter: parent.verticalCenter

            iconSource: "toolbar-delete"
            onClicked: removeClicked()
        }
    }

    DialogLoader {
        id: editLoader
    }
}


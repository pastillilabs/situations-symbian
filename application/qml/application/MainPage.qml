import QtQuick 1.1
import com.nokia.symbian 1.1

Page {
    id: mainPage
    tools: mainTools

    property bool edit: false

    onEditChanged: {
        if(edit) mainPage.pageStack.toolBar.setTools(editTools)
        else mainPage.pageStack.toolBar.setTools(mainTools)
        privateStyle.play(Symbian.BasicButton)
    }

    Flickable {
        id: flickable
        anchors.fill: parent
        contentHeight: mainHeading.height + situationColumn.height

        MouseArea {
            width: flickable.width
            height: mainPage.height
            onClicked: mainPage.edit = false
            onPressAndHold: mainPage.edit = true
        }

        MainHeading {
            id: mainHeading
            width: flickable.width
            height: 100
            edit: mainPage.edit

            onClicked: aboutDialog.open()
            onPressAndHold: mainPage.edit = true
        }

        Column {
            id: situationColumn
            width: flickable.width
            anchors.top: mainHeading.bottom

            move: Transition { NumberAnimation { properties: "y"; duration: 100 } }

            Repeater {
                id: repeater
                width: parent.width
                model: situations

                SituationListItem {
                    width: situationColumn.width
                    edit: mainPage.edit
                    onPressAndHold: mainPage.edit = true
                    onEditClicked: mainPage.edit = false
                }
            }
        }
    }

    ScrollBar {
        anchors.right: flickable.right
        flickableItem: flickable
    }

    Connections {
        target: client
        onAddSituation: repeater.itemAt(repeater.count - 1).clicked()
    }

    ToolBarLayout {
        id: mainTools

        ToolButton {
            iconSource: "toolbar-back"
            onClicked: Qt.quit()
        }
        ToolButton {
            iconSource: "toolbar-add"
            onClicked: newSituationDialog.open()
        }
        ToolButton {
            iconSource: "toolbar-settings"
            onClicked: mainPage.edit = true
        }
    }

    ToolBarLayout {
        id: editTools

        ToolButton {
            iconSource: "toolbar-back"
            onClicked: mainPage.edit = false
        }

        ToolButton {
            iconSource: "qrc:/gfx/application/information_userguide.svg"
            onClicked: aboutDialog.open()
        }

        ToolButton {
            iconSource: "toolbar-view-menu"
            onClicked: mainMenu.open()
        }
    }

    Loader {
        id: mainMenu

        function open() {
            source = "qrc:/qml/application/MainMenu.qml"
            item.open()
        }
    }

    Loader {
        id: newSituationDialog

        function open() {
            source = "qrc:/qml/application/NewSituationDialog.qml"
            item.open()
        }
    }

    Loader {
        id: aboutDialog

        function open() {
            source = "qrc:/qml/application/AboutDialog.qml"
            item.open()
        }
    }
}


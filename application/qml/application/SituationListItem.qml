import QtQuick 1.1
import com.nokia.symbian 1.1

ListItem {
    property bool edit: false
    property int situationIndex: index

    signal editClicked

    onClicked: pageStack.push("qrc:/qml/application/SettingsPage.qml", {situationIndex: index, situation: situation, situationConditions: situationConditions, situationActions: situationActions})
    onEditChanged: edit ? editLayer.show() : editLayer.hide()
    subItemIndicator: !edit

    Item {
        id: activeContainer
        width: parent.height
        height: parent.height
        anchors.left: parent.left

        RadioButton {
            opacity: edit ? 0.3 : 1
            anchors.centerIn: parent
            checked: situations.enabled && situations.current == index
            pressed: radioButtonMouseArea.pressed
        }

        MouseArea {
            id: radioButtonMouseArea
            anchors.fill: parent
            onClicked: if(situations.enabled) client.reqSetCurrent(index)
        }
    }

    ListItemText {
        id: listItemText

        anchors.fill: paddingItem
        anchors.leftMargin: activeContainer.width

        text: situation.name
        verticalAlignment: Text.AlignVCenter
        mode: situation.enabled ? parent.mode : "disabled"
        font.bold: situations.proposal === index
        role: "Title"
    }

    Loader {
        id: editLayer
        anchors.fill: parent
        opacity: 0
        Behavior on opacity { NumberAnimation { duration: 200 } }

        function show() {
            source = "qrc:/qml/application/SituationListItemEdit.qml"
            opacity = 1
        }

        function hide() {
            opacity = 0
        }
    }

    Loader {
        id: settingsDialog

        function open() {
            source = "qrc:/qml/application/SituationSettingsDialog.qml"
            item.open()
        }
    }
}


import QtQuick 1.1
import com.nokia.symbian 1.1

CommonDialog {
    id: dialog

    property alias name: nameField.text
    property alias enabled: enabledSwitch.checked

    titleText: qsTrId("dialog_title_situation_settings")
    buttonTexts: [qsTrId("dialog_softkey_ok"), qsTrId("dialog_softkey_cancel")]

    onStatusChanged: {
        if(status === DialogStatus.Opening) {
            nameField.text = situation.name
            enabledSwitch.checked = situation.enabled
        }
    }

    onButtonClicked: index == 0 ? accept() : reject()
    onAccepted: {
        if(situation.name != dialog.name) client.reqSetSituationName(situationIndex, dialog.name)
        if(situation.enabled != dialog.enabled) client.reqSetSituationEnabled(situationIndex, dialog.enabled)
    }

    content: Flickable {
        id: flickable
        width: parent.width
        height: Math.min(platformContentMaximumHeight, contentHeight + 1)
        contentWidth: width
        contentHeight: content.height + content.y * 2
        flickableDirection: Flickable.VerticalFlick
        interactive: contentHeight > height

        Column {
            id: content
            x: 20
            y: 20
            width: parent.width - 2 * x
            spacing: 20

            Column {
                spacing: 5

                Label {
                    text: qsTrId("situation_setting_name")
                }

                TextField {
                    id: nameField
                    width: content.width
                    maximumLength: 50
                    validator: RegExpValidator { regExp: /[^ ].+|[^ ]/ }
                    onTextChanged: dialog.buttons[0].children[0].children[0].enabled = acceptableInput
                }
            }

            Column {
                spacing: 5
                visible: situationIndex

                Label {
                    text: qsTrId("situation_setting_automation")
                }

                Row {
                    spacing: 10
                    Switch {
                        id: enabledSwitch
                        checked: situation.enabled
                    }

                    Label {
                        anchors.verticalCenter: enabledSwitch.verticalCenter
                        text: enabledSwitch.checked ? qsTrId("setting_on") : qsTrId("setting_off")
                    }
                }
            }
        }
    }

    ScrollBar {
        parent: flickable
        anchors.right: parent.right
        flickableItem: parent
    }
}

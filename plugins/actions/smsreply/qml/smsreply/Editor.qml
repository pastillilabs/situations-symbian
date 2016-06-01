import QtQuick 1.1
import com.nokia.symbian 1.1
import com.pastillilabs.situations.action.smsreply 1.0

CommonDialog {
    id: dialog
    titleText: qsTrId("action_smsreply_name")
    buttonTexts: [qsTrId("dialog_softkey_ok"), qsTrId("dialog_softkey_cancel")]

    Component.onCompleted: if(itemIndex >= 0) {
        action.data = group.getActionData(itemIndex)
    }

    onButtonClicked: index === 0 ? accept() : reject()
    onAccepted: {
        if(itemIndex >= 0) client.reqSetActionData(situationIndex, groupIndex, itemIndex, action.data)
        else client.reqAddAction(situationIndex, groupIndex, action.data)
    }

    SmsReplyAction {
        id: action
    }

    content: Item {
        width: parent.width
        height: Math.min(platformContentMaximumHeight, flickable.contentHeight)

        Flickable {
            id: flickable
            anchors.fill: parent
            contentWidth: content.width + content.x * 2
            contentHeight: content.height + content.y * 2
            flickableDirection: Flickable.VerticalFlick
            interactive: contentHeight > height

            Column {
                id: content
                x: 20
                y: 20
                width: flickable.width - 2 * x
                spacing: 20

                TextArea {
                    id: messageField
                    width: parent.width
                    height: 120
                    text: action.value
                    onTextChanged: {
                        action.value = text
                        dialog.buttons[0].children[0].children[0].enabled = (text !== "")
                    }
                }

                CheckBox {
                    checked: action.onlyContacts
                    text: qsTrId("dialog_sms_reply_only_contacts")
                    onClicked: action.onlyContacts = !action.onlyContacts
                }
            }
        }
    }
}


import QtQuick 1.1
import com.nokia.symbian 1.1
import com.pastillilabs.situations.action.smssend 1.0

CommonDialog {
    id: dialog
    titleText: qsTrId("action_sms_send_name")
    buttonTexts: contactName ? [qsTrId("dialog_softkey_ok"), qsTrId("dialog_softkey_cancel")] : [qsTrId("dialog_softkey_cancel")]

    Component.onCompleted: if(itemIndex >= 0) {
        action.data = group.getActionData(itemIndex)
    }

    onButtonClicked: index === 0 && contactName ? accept() : reject()
    onAccepted: {
        if(itemIndex >= 0) client.reqSetActionData(situationIndex, groupIndex, itemIndex, action.data)
        else client.reqAddAction(situationIndex, groupIndex, action.data)
    }

    property string contactName: action.nameById(action.contactId)

    SmsSendAction {
        id: action
        onContactIdChanged: contactName = action.nameById(action.contactId)
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

                CheckBox {
                    checked: action.situationEnd
                    text: qsTrId("action_at_situation_end")
                    onClicked: action.situationEnd = !action.situationEnd
                }

                ToolButton {
                    onClicked: contactDialog.open()
                    width: parent.width
                    text: contactName ? contactName : qsTrId("action_sms_send_select_recipient")
                    iconSource: "qrc:/gfx/smssend/contacts.svg"

                    Loader {
                        id: contactDialog

                        function open() {
                            source = "qrc:/qml/smssend/ContactDialog.qml"
                            item.open()
                        }
                    }
                }

                TextArea {
                    width: parent.width
                    height: 120

                    text: action.value
                    onTextChanged: action.value = text
                }
            }
        }
    }
}


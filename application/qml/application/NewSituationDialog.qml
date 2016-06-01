import QtQuick 1.1
import com.nokia.symbian 1.1

CommonDialog {
    id: dialog

    property alias name: nameField.text

    titleText: qsTrId("dialog_title_new_situation")
    buttonTexts: [qsTrId("dialog_softkey_ok"), qsTrId("dialog_softkey_cancel")]
    onButtonClicked: index === 0 ? accepted() : rejected()

    onStatusChanged: if(status === DialogStatus.Opening) nameField.text = qsTrId("situation_name_new")
    onAccepted: client.reqAddSituation("", name)

    content: Flickable {
        width: parent.width
        height: Math.min(platformContentMaximumHeight, contentHeight + 1)
        contentWidth: width
        contentHeight: column.height + column.y * 2
        flickableDirection: Flickable.VerticalFlick
        interactive: contentHeight > height

        Column {
            id: column
            x: 20
            y: 20
            width: parent.width - 2 * x
            spacing: 5

            Label {
                width: parent.width
                text: qsTrId("situation_setting_name")
            }

            TextField {
                id: nameField
                width: parent.width
                maximumLength: 50
                validator: RegExpValidator { regExp: /[^ ].+|[^ ]/ }
                onTextChanged: dialog.buttons[0].children[0].children[0].enabled = acceptableInput
            }
        }
    }
}


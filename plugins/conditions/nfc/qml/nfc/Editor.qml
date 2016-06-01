import QtQuick 1.1
import com.nokia.symbian 1.1
import com.pastillilabs.situations.condition.nfc 1.0

CommonDialog {
    id: dialog

    titleText: qsTrId("condition_nfc_name")
    buttonTexts: condition.uid ? [qsTrId("dialog_softkey_ok"), qsTrId("dialog_softkey_cancel")] : [qsTrId("dialog_softkey_cancel")]

    Component.onCompleted: if(itemIndex >= 0) {
        condition.data = group.getConditionData(itemIndex)
    }

    onStatusChanged: if(status == DialogStatus.Open && !scanner.power()) powerDialog.open()

    onButtonClicked: index === 0 && condition.uid ? accept() : reject()
    onAccepted: {
        if(itemIndex >= 0) client.reqSetConditionData(situationIndex, groupIndex, itemIndex, condition.data)
        else client.reqAddCondition(situationIndex, groupIndex, condition.data)
    }

    NfcCondition {
        id: condition
    }

    NfcScanner {
        id: scanner
        onUidChanged: if(uid) condition.uid = uid
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
                y: 20
                width: flickable.width
                spacing: 20

                TextField {
                    width: parent.width - 40
                    anchors.horizontalCenter: parent.horizontalCenter
                    placeholderText: qsTrId("condition_nfc_tag_name")
                    text: condition.name
                    onTextChanged: condition.name = text
                }

                Item {
                    width: 200
                    height: 200
                    anchors.horizontalCenter: parent.horizontalCenter

                    Rectangle {
                        id: indicator
                        anchors.fill: parent
                        anchors.margins: 20
                        radius: width / 2
                        opacity: scanner.uid ? 1 : 0.5
                        color: "transparent"
                        border.color: condition.uid ? "green" : "darkgrey"
                        border.width: width / 8

                        Rectangle {
                            anchors.fill: parent
                            anchors.margins: parent.width / 4
                            radius: width / 2
                            color: parent.border.color
                        }
                    }

                    Label {
                        anchors.fill: parent
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter

                        text: condition.uid ? condition.uidString() : qsTrId("condition_nfc_no_tag")
                    }
                }

                CheckBox {
                    width: parent.width - 40
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: qsTrId("condition_nfc_virtual")
                    checked: condition.virtualTag
                    onCheckedChanged: if(checked != condition.virtualTag) condition.virtualTag = checked
                }
            }
        }
    }

    CommonDialog {
        id: powerDialog

        titleText: qsTrId("condition_nfc_power")
        buttonTexts: [qsTrId("dialog_softkey_settings"), qsTrId("dialog_softkey_cancel")]

        onButtonClicked: index === 0 ? accept() : reject()
        onAccepted: scanner.powerOn()

        content: Label {
            x: 20
            width: platformContentMaximumWidth - 2 * x
            height: 200
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            wrapMode: Text.Wrap

            text: qsTrId("condition_nfc_power_description")
        }
    }
}


import QtQuick 1.1
import com.nokia.symbian 1.1
import com.pastillilabs.situations.condition.connectivity 1.0

CommonDialog {
    id: dialog
    titleText: qsTrId("condition_connectivity_name")
    buttonTexts: nameField.text != "" ? [qsTrId("dialog_softkey_ok"), qsTrId("dialog_softkey_cancel")] : [qsTrId("dialog_softkey_cancel")]

    Component.onCompleted: if(itemIndex >= 0) {
        condition.data = group.getConditionData(itemIndex)
    }

    onButtonClicked: index === 0 && nameField.text != "" ? accept() : reject()
    onAccepted: {
        if(itemIndex >= 0) client.reqSetConditionData(situationIndex, groupIndex, itemIndex, condition.data)
        else client.reqAddCondition(situationIndex, groupIndex, condition.data)
    }

    onStatusChanged: typeSwitch.checkedButton = (condition.isWlan ? wlan : bt)

    ConnectivityCondition {
        id: condition
        property bool isWlan: type == ConnectivityCondition.TypeWlan
    }

    WlanScanner {
        id: wlanScanner
    }

    BtScanner {
        id: btScanner
    }

    content: Flickable {
        id: flickable
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
            spacing: 20

            ButtonRow {
                id: typeSwitch
                width: 200
                anchors.horizontalCenter: parent.horizontalCenter
                checkedButton: condition.isWlan ? wlan : bt

                Button {
                    id: wlan
                    text: qsTrId("connectivity_buttonrow_wlan")
                    onClicked: {
                        if(!condition.isWlan) condition.name = ""
                        condition.type = ConnectivityCondition.TypeWlan
                    }
                }
                Button {
                    id: bt
                    text: qsTrId("connectivity_buttonrow_bluetooth")
                    onClicked: {
                        if(condition.isWlan) condition.name = ""
                        condition.type = ConnectivityCondition.TypeBt
                    }
                }
            }

            ToolButton {
                id: selector
                anchors.left: parent.left
                anchors.right: parent.right
                iconSource: "toolbar-search"
                text: qsTrId("connectivity_button_search")
                onClicked: availableConnections.open()
            }

            TextField {
                id: nameField
                anchors.left: parent.left
                anchors.right: parent.right
                placeholderText: qsTrId("connectivity_textfield_connection_name")
                maximumLength: 50
                validator: RegExpValidator { regExp: /[^ ].+|[^ ]/ }
                text: condition.name
                onTextChanged: if(text != condition.name) condition.name = text
            }

            Connections {
                target: inputContext
                onVisibleChanged: if(inputContext.visible) flickable.contentY = flickable.contentHeight - flickable.height - column.y
            }
        }
    }

    SelectionDialog {
        id: availableConnections
        titleText: qsTrId("dialog_title_available_connections")
        model: condition.isWlan ? wlanScanner.connections : btScanner.connections
        onStatusChanged: {
            selectedIndex = -1
            if(status === DialogStatus.Opening) {
                if(condition.isWlan ? wlanScanner.power() : btScanner.power()) condition.isWlan ? wlanScanner.start() : btScanner.start()
                else powerButton.visible = true
            }
            if(status === DialogStatus.Closing) {
                condition.isWlan ? wlanScanner.stop() : btScanner.stop()
                powerButton.visible = false
            }
        }
        onSelectedIndexChanged: nameField.text = model[selectedIndex]
        onPlatformContentMaximumHeightChanged: availableConnections.content[0].height = 4 * privateStyle.menuItemHeight

        BusyIndicator {
            id: busyIndicator
            parent: availableConnections.content[0]
            anchors.centerIn: parent
            visible: condition.isWlan ? wlanScanner.scanning : btScanner.scanning
            running: visible
        }

        Column {
            parent: availableConnections.content[0]
            anchors.centerIn: parent
            spacing: 20

            Label {
                anchors.horizontalCenter: parent.horizontalCenter
                visible: !busyIndicator.visible && (condition.isWlan ? wlanScanner.count == 0 : btScanner.count == 0)
                text: qsTrId("dialog_text_no_connections")
            }

            Button {
                id: powerButton
                anchors.horizontalCenter: parent.horizontalCenter
                visible: false
                text: condition.isWlan ? qsTrId("dialog_text_turn_on_wlan") : qsTrId("dialog_text_turn_on_bt")
                onClicked: {
                    condition.isWlan ? wlanScanner.powerOn() : btScanner.powerOn()
                    scanTimer.start()
                    visible = false
                }

                Timer {
                    id: scanTimer
                    interval: 1000
                    repeat: false
                    onTriggered: condition.isWlan ? wlanScanner.start() : btScanner.start()
                }
            }
        }
    }
}


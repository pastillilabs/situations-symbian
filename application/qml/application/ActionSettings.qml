import QtQuick 1.1
import com.nokia.symbian 1.1

CommonDialog {
    titleText: qsTrId("dialog_action_settings")
    buttonTexts: [qsTrId("dialog_softkey_ok"), qsTrId("dialog_softkey_cancel")]

    property alias hasDelay: delayItem.visible
    property alias hasRestore: restoreItem.visible

    property int maxDelay: 10000
    property int startDelay: action.startDelay
    property bool restore: action.restore

    onButtonClicked: index === 0 ? accept() : reject()
    onAccepted: {
        action.startDelay = startDelay
        action.restore = restore
        client.reqSetActionData(situationIndex, groupIndex, itemIndex, action.data)
    }

    content: Flickable {
        width: parent.width
        height: contentHeight + 1
        contentWidth: width
        contentHeight: column.height + column.y * 2
        interactive: contentHeight > height

        Column {
            id: column
            x: 20
            y: 20
            width: parent.width - 2 * x
            spacing: 20

            Column {
                id: delayItem
                spacing: 5

                Label {
                    text: qsTrId("action_delay")
                }

                Item {
                    width: column.width
                    height: 70

                    ToolButton {
                        anchors.left: parent.left
                        anchors.verticalCenter: parent.verticalCenter
                        iconSource: "toolbar-previous"

                        MouseArea {
                            anchors.fill: parent
                            Timer {
                                running: parent.pressed
                                repeat: true
                                interval: 100
                                triggeredOnStart: true
                                onTriggered: startDelay > 0 ? startDelay -= 200 : startDelay = 0
                            }
                        }
                    }

                    Label {
                        anchors.centerIn: parent
                        text: qsTrId("action_delay_value").arg(startDelay)
                    }

                    ToolButton {
                        anchors.right: parent.right
                        anchors.verticalCenter: parent.verticalCenter
                        iconSource: "toolbar-next"

                        MouseArea {
                            anchors.fill: parent
                            Timer {
                                running: parent.pressed
                                repeat: true
                                interval: 100
                                triggeredOnStart: true
                                onTriggered: startDelay < maxDelay ? startDelay += 200 : startDelay = maxDelay
                            }
                        }
                    }
                }
            }

            Column {
                id: restoreItem
                spacing: 5

                Label {
                    text: qsTrId("action_restore")
                }

                Row {
                    spacing: 10
                    Switch {
                        id: enabledSwitch
                        checked: restore
                        onCheckedChanged: restore = checked
                    }

                    Label {
                        anchors.verticalCenter: enabledSwitch.verticalCenter
                        text: enabledSwitch.checked ? qsTrId("setting_on") : qsTrId("setting_off")
                    }
                }
            }
        }
    }
}


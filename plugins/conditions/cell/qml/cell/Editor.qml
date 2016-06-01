import QtQuick 1.1
import com.nokia.symbian 1.1
import com.pastillilabs.situations.condition.cell 1.0

CommonDialog {
    id: dialog

    titleText: qsTrId("condition_cell_name")
    buttonTexts: condition.cellCount ? [qsTrId("dialog_softkey_ok"), qsTrId("dialog_softkey_cancel")] : [qsTrId("dialog_softkey_cancel")]

    Component.onCompleted: if(itemIndex >= 0) {
        condition.data = group.getConditionData(itemIndex)
    }

    onButtonClicked: index === 0 && condition.cellCount ? accept() : reject()
    onAccepted: {
        if(itemIndex >= 0) client.reqSetConditionData(situationIndex, groupIndex, itemIndex, condition.data)
        else client.reqAddCondition(situationIndex, groupIndex, condition.data)
    }

    CellCondition {
        id: condition

        onCellCountChanged: if(cellCount == 0 && !adding) stopRecording()
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
            y: 20
            width: parent.width
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 20

            Item {
                width: parent.width - 40
                height: functions.height
                anchors.horizontalCenter: parent.horizontalCenter

                ButtonRow {
                    id: functions
                    width: parent.width
                    exclusive: false
                    opacity: condition.recording ? 0 : 1

                    Behavior on opacity { NumberAnimation { duration: 200 } }

                    Button {
                        text: qsTrId("condition_cell_dialog_record")
                        enabled: !condition.recording
                        onClicked: condition.startRecording(true)
                    }

                    Button {
                        text: qsTrId("condition_cell_dialog_erase")
                        enabled: !condition.recording && condition.cellCount > 0
                        onClicked: condition.startRecording(false)
                    }
                }

                Item {
                    anchors.fill: functions
                    opacity: condition.recording ? 1 : 0

                    Behavior on opacity { NumberAnimation { duration: 200 } }

                    BusyIndicator {
                        id: indicator
                        anchors.left: parent.left
                        anchors.verticalCenter: parent.verticalCenter
                        running: condition.recording
                    }

                    Label {
                        anchors.left: indicator.right
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.margins: 20
                        text: condition.adding ? qsTrId("condition_cell_dialog_recording") : qsTrId("condition_cell_dialog_erasing")
                    }

                    ToolButton {
                        id: stopButton
                        anchors.right: parent.right
                        anchors.verticalCenter: parent.verticalCenter
                        iconSource: "qrc:/gfx/cell/stop.svg"
                        flat: false
                        onClicked: condition.stopRecording()
                    }
                }
            }

            Label {
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTrId("condition_cell_dialog_count").arg(condition.cellCount)
                font.pixelSize: 30
            }

            TextField {
                width: parent.width - 40
                anchors.horizontalCenter: parent.horizontalCenter

                placeholderText: qsTrId("condition_cell_dialog_area_name")
                text: condition.name
                onTextChanged: condition.name = text
            }

            Connections {
                target: inputContext
                onVisibleChanged: if(inputContext.visible) flickable.contentY = flickable.contentHeight - flickable.height - column.y
            }
        }
    }
}


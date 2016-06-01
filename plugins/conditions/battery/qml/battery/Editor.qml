import QtQuick 1.1
import com.nokia.symbian 1.1
import com.pastillilabs.situations.condition.battery 1.0

CommonDialog {
    titleText: qsTrId("condition_battery_name")
    buttonTexts: [qsTrId("dialog_softkey_ok"), qsTrId("dialog_softkey_cancel")]

    Component.onCompleted: if(itemIndex >= 0) {
        condition.data = group.getConditionData(itemIndex)
    }

    onButtonClicked: index === 0 ? accept() : reject()
    onAccepted: {
        if(itemIndex >= 0) client.reqSetConditionData(situationIndex, groupIndex, itemIndex, condition.data)
        else client.reqAddCondition(situationIndex, groupIndex, condition.data)
    }

    onStatusChanged: lessThanSwitch.checkedButton = (condition.lessThan ? lessThanButton : moreThanButton)

    BatteryCondition {
        id: condition
    }

    content: Item {
        width: parent.width
        height: column.height + column.y * 2

        Column {
            id: column
            x: 10
            y: 20
            width: parent.width - 2 * x
            spacing: 20

            ButtonRow {
                id: lessThanSwitch
                width: parent.width

                Button {
                    id: lessThanButton
                    text: qsTrId("dialog_battery_less_than")
                    onClicked: condition.lessThan = true
                }

                Button {
                    id: moreThanButton
                    text: qsTrId("dialog_battery_more_than")
                    onClicked: condition.lessThan = false
                }
            }

            Column {
                width: parent.width

                Label {
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: qsTrId("dialog_battery_remaining").arg(slider.value)
                    font.pixelSize: 30
                }

                Slider {
                    id: slider
                    width: parent.width
                    minimumValue: 0
                    maximumValue: 100
                    stepSize: 10
                    value: condition.value
                    onValueChanged: condition.value = value
                }
            }
        }
    }
}


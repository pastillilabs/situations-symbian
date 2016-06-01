import QtQuick 1.1
import com.nokia.symbian 1.1
import com.pastillilabs.situations.condition.time 1.0

CommonDialog {
    titleText: qsTrId("condition_time_name")
    buttonTexts: [qsTrId("dialog_softkey_ok"), qsTrId("dialog_softkey_cancel")]

    Component.onCompleted: if(itemIndex >= 0) {
        condition.data = group.getConditionData(itemIndex)
    }

    onButtonClicked: index === 0 ? accept() : reject()
    onAccepted: {
        if(itemIndex >= 0) client.reqSetConditionData(situationIndex, groupIndex, itemIndex, condition.data)
        else client.reqAddCondition(situationIndex, groupIndex, condition.data)
    }

    TimeCondition {
        id: condition
    }

    content: Item {
        width: parent.width
        height: column.height + column.y * 2

        Column {
            id: column
            y: 20
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 20

            TimePeriodPicker {
                anchors.horizontalCenter: parent.horizontalCenter
                startHour: condition.startHour
                startMinute: condition.startMinute
                endHour: condition.endHour
                endMinute: condition.endMinute

                onStartHourChanged: condition.startHour = startHour
                onStartMinuteChanged: condition.startMinute = startMinute
                onEndHourChanged: condition.endHour = endHour
                onEndMinuteChanged: condition.endMinute = endMinute
            }

            Row {
                height: 46
                anchors.horizontalCenter: parent.horizontalCenter

                Button {
                    width: parent.height
                    height: parent.height
                    checked: condition.monday
                    text: qsTrId("time_selection_monday")
                    font.pixelSize: 14
                    onClicked: condition.monday = !condition.monday
                }

                Button {
                    width: parent.height
                    height: parent.height
                    checked: condition.tuesday
                    text: qsTrId("time_selection_tuesday")
                    font.pixelSize: 14
                    onClicked: condition.tuesday = !condition.tuesday
                }

                Button {
                    width: parent.height
                    height: parent.height
                    checked: condition.wednesday
                    text: qsTrId("time_selection_wednesday")
                    font.pixelSize: 14
                    onClicked: condition.wednesday = !condition.wednesday
                }

                Button {
                    width: parent.height
                    height: parent.height
                    checked: condition.thursday
                    text: qsTrId("time_selection_thursday")
                    font.pixelSize: 14
                    onClicked: condition.thursday = !condition.thursday
                }

                Button {
                    width: parent.height
                    height: parent.height
                    checked: condition.friday
                    text: qsTrId("time_selection_friday")
                    font.pixelSize: 14
                    onClicked: condition.friday = !condition.friday
                }

                Button {
                    width: parent.height
                    height: parent.height
                    checked: condition.saturday
                    text: qsTrId("time_selection_saturday")
                    font.pixelSize: 14
                    onClicked: condition.saturday = !condition.saturday
                }

                Button {
                    width: parent.height
                    height: parent.height
                    checked: condition.sunday
                    text: qsTrId("time_selection_sunday")
                    font.pixelSize: 14
                    onClicked: condition.sunday = !condition.sunday
                }
            }
        }
    }
}


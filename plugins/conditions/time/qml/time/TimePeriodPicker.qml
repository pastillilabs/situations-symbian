import QtQuick 1.1
import com.nokia.symbian 1.1

Row {
    property alias startHour: startTimePicker.hour
    property alias startMinute: startTimePicker.minute
    property alias endHour: endTimePicker.hour
    property alias endMinute: endTimePicker.minute

    spacing: 20

    TimePicker {
        id: startTimePicker
    }

    Label {
        anchors.verticalCenter: parent.verticalCenter
        font.pixelSize: 30
        text: "-"
    }

    TimePicker {
        id: endTimePicker
    }
}

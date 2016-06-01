import QtQuick 1.1
import com.nokia.symbian 1.1
import "qrc:/qml/application"

PluginItem {
    content: Column {
        anchors.fill: parent
        anchors.margins: 10

        Label {
            width: parent.width
            text: getString()
            elide: Text.ElideRight

            function getString() {
                var startDate = new Date()
                var endDate = new Date()
                startDate.setHours(condition.startHour, condition.startMinute)
                endDate.setHours(condition.endHour, condition.endMinute)
                var startTime = Qt.formatTime(startDate, qsTrId("time_item_time_format"))
                var endTime = Qt.formatTime(endDate, qsTrId("time_item_time_format"))

                return qsTrId("time_item_time_period").arg(startTime.valueOf()).arg(endTime.valueOf())
            }
        }

        Row {
            spacing: 5

            Label {
                color: condition.monday ? "white" : "grey"
                text: qsTrId("time_item_monday")
            }

            Label {
                color: condition.tuesday ? "white" : "grey"
                text: qsTrId("time_item_tuesday")
            }

            Label {
                color: condition.wednesday ? "white" : "grey"
                text: qsTrId("time_item_wednesday")
            }

            Label {
                color: condition.thursday ? "white" : "grey"
                text: qsTrId("time_item_thursday")
            }

            Label {
                color: condition.friday ? "white" : "grey"
                text: qsTrId("time_item_friday")
            }

            Label {
                color: condition.saturday ? "white" : "grey"
                text: qsTrId("time_item_saturday")
            }

            Label {
                color: condition.sunday ? "white" : "grey"
                text: qsTrId("time_item_sunday")
            }
        }
    }
}


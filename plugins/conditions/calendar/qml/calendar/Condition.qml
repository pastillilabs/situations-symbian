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
                var string = ""

                if(condition.alarm) string += qsTrId("calendar_item_events_alarmed")
                if(condition.recurring) string += qsTrId("calendar_item_events_recurring")
                if(condition.noAllDay) string += qsTrId("calendar_item_events_no_allday")

                if(string == "") string = qsTrId("calendar_item_events_all")
                return string
            }
        }

        Label {
            width: parent.width
            text: getString()
            elide: Text.ElideRight

            function getString() {
                var string = ""
                if(condition.keyword) string = qsTrId("calendar_item_keyword").arg(condition.keyword)
                return string
            }
        }
    }
}


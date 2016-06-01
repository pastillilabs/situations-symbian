import QtQuick 1.1
import com.nokia.symbian 1.1
import "qrc:/qml/application"

PluginItem {
    content: Column {
        anchors.fill: parent
        anchors.margins: 10

        Label {
            width: parent.width
            text: qsTrId("location_item_latitude").arg(condition.latitude)
            font.pixelSize: 14
        }

        Label {
            width: parent.width
            text: qsTrId("location_item_longitude").arg(condition.longitude)
            font.pixelSize: 14
        }

        Label {
            width: parent.width
            text: qsTrId("location_item_radius").arg(condition.radius)
            font.pixelSize: 14
        }
    }
}


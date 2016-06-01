import QtQuick 1.1
import com.nokia.symbian 1.1

PluginItem {
    property alias checked: switchItem.checked
    property alias text: labelItem.text

    content: Row {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        Switch {
            id: switchItem
            anchors.verticalCenter: parent.verticalCenter
        }

        Label {
            id: labelItem
            anchors.verticalCenter: parent.verticalCenter
        }
    }
}


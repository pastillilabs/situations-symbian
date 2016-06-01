import QtQuick 1.1
import com.nokia.symbian 1.1
import "qrc:/qml/application"

PluginItem {
    content: Column {
        anchors.fill: parent
        anchors.margins: 10

        Label {
            width: parent.width
            text: condition.name
            elide: Text.ElideRight
            visible: condition.name !== ""
        }

        Label {
            id: uidLabel
            width: parent.width
            text: condition.uidString()
            elide: Text.ElideRight

            Connections {
                target: condition
                onUidChanged: uidLabel.text = condition.uidString()
            }
        }
    }
}


import QtQuick 1.1
import com.nokia.symbian 1.1
import "qrc:/qml/application"

PluginItem {
    content: Column {
        anchors.fill: parent
        anchors.margins: 10

        Label {
            width: parent.width
            text: condition.nameById(condition.type)
            elide: Text.ElideRight
        }

        Label {
            width: parent.width
            text: condition.name
            elide: Text.ElideRight
        }
    }
}


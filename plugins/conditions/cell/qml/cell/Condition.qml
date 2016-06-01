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
            width: parent.width
            text: qsTrId("condition_cell_item_count").arg(condition.cellCount)
            elide: Text.ElideRight
        }
    }
}


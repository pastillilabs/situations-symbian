import QtQuick 1.1
import com.nokia.symbian 1.1

PluginItem {
    property alias text: labelItem.text
    property alias endIconVisible: endIcon.visible

    content: Label {
        id: labelItem
        anchors.fill: parent
        anchors.margins: 10

        elide: Text.ElideRight
        verticalAlignment: Text.AlignVCenter
        wrapMode: Text.Wrap
        maximumLineCount: 2

        Image {
            id: endIcon
            width: 30
            height: 30
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            opacity: 0.3
            visible: false
            source: "qrc:/gfx/application/atend.svg"
            sourceSize.width: width
            sourceSize.height: height
        }
    }
}


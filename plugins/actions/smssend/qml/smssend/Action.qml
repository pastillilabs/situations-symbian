import QtQuick 1.1
import com.nokia.symbian 1.1
import "qrc:/qml/application"

PluginItem {
    content: Item {
        anchors.fill: parent
        anchors.margins: 10

        Column {
            anchors.fill: parent

            Label {
                id: contactLabel
                width: parent.width

                text: action.nameById(action.contactId)
                elide: Text.ElideRight
                clip: true
                verticalAlignment: Text.AlignVCenter
            }

            Label {
                width: parent.width

                text: action.value
                elide: Text.ElideRight
                wrapMode: Text.Wrap
                maximumLineCount: 1
                verticalAlignment: Text.AlignVCenter
            }
        }

        Image {
            width: 30
            height: 30
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            opacity: 0.3
            visible: action.situationEnd
            source: "qrc:/gfx/application/atend.svg"
            sourceSize.width: width
            sourceSize.height: height
        }
    }
}


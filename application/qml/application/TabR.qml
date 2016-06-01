import QtQuick 1.1
import com.nokia.symbian 1.1

BorderImage {
    property bool active: false
    property alias text: textItem.text

    signal clicked

    border.left: 1
    border.right: 30
    border.top: 30
    border.bottom: 30

    source: active || mouseArea.pressed ? "qrc:/gfx/application/TabR_on.svg" : "qrc:/gfx/application/TabR.svg"

    Label {
        id: textItem
        anchors.fill: parent
        anchors.margins: 5
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: {
            parent.clicked()
            privateStyle.play(Symbian.BasicButton)
        }
        onPressed: privateStyle.play(Symbian.BasicButton)
    }
}


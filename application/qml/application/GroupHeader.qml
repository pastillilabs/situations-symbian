import QtQuick 1.1
import com.nokia.symbian 1.1

Item {
    height: childrenRect.height

    property alias enabled: addMouseArea.enabled
    property bool hasNegative: false

    signal add
    signal switchNegative

    BorderImage {
        id: background
        anchors.left: parent.left
        anchors.right: parent.right
        border.left: 10
        border.right: 10
        border.top: 10
        border.bottom: 10
        smooth: true
        source: addMouseArea.pressed ? "qrc:/gfx/application/GroupHeaderPressed.svg" : "qrc:/gfx/application/GroupHeader.svg"
    }

    BorderImage {
        id: iconArea
        width: group.negative ? 100 : 50
        height: background.height
        anchors.left: parent.left
        border.left: 10
        border.right: 10
        border.top: 10
        border.bottom: 10
        smooth: true
        visible: application.advancedMode

        property string iconSource: notMouseArea.pressed ? "qrc:/gfx/application/GroupHeaderIconAreaPressed.svg" : "qrc:/gfx/application/GroupHeaderIconArea.svg"
        property string notIconSource: notMouseArea.pressed ? "qrc:/gfx/application/GroupHeaderNotPressed.svg" : "qrc:/gfx/application/GroupHeaderNot.svg"
        source: hasNegative ? (group.negative ? notIconSource : iconSource) : ""

        Behavior on width { NumberAnimation { duration: 200; easing.type: Easing.OutQuad } }
    }

    Label {
        anchors.right: iconArea.right
        width: 50
        anchors.verticalCenter: iconArea.verticalCenter
        font.pixelSize: 14
        opacity: group.negative ? 1 : 0
        text: qsTrId("group_not")
        horizontalAlignment: Text.AlignHCenter
        maximumLineCount: 2

        Behavior on opacity { NumberAnimation { duration: 200; easing.type: Easing.OutQuad } }
    }

    Image {
        id: headerIcon
        width: 30
        height: 30
        anchors.left: iconArea.left
        anchors.leftMargin: 10
        anchors.verticalCenter: iconArea.verticalCenter
        smooth: true
        source: groupIcon
    }

    Image {
        id: notArrow
        width: 10
        height: background.height
        anchors.left: headerIcon.right
        scale: 0.4
        opacity: 0.5
        smooth: true
        visible: application.advancedMode
        source: hasNegative ? (group.negative ? "qrc:/gfx/application/IconLeft.svg" : "qrc:/gfx/application/IconRight.svg") : ""
        fillMode: Image.PreserveAspectFit
    }

    Label {
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: iconArea.right
        anchors.right: addButton.left
        anchors.margins: 10
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
        text: qsTrId(groupLocId)
    }

    BorderImage {
        id: addButton
        anchors.right: parent.right
        border.left: 10
        border.right: 10
        border.top: 10
        border.bottom: 10
        smooth: true

        property string iconSource: addMouseArea.enabled ? "qrc:/gfx/application/GroupHeaderAdd.svg" : "qrc:/gfx/application/GroupHeaderAddInactive.svg"
        source: addMouseArea.pressed ? "qrc:/gfx/application/GroupHeaderAddPressed.svg" : iconSource

        Image {
            anchors.centerIn: parent
            source: addMouseArea.enabled ? "qrc:/gfx/application/add.svg" : ""
            smooth: true
        }
    }

    MouseArea {
        id: addMouseArea
        anchors.fill: parent
        onClicked: add()
        onPressed: privateStyle.play(Symbian.BasicButton)
        onReleased: privateStyle.play(Symbian.BasicButton)
    }

    MouseArea {
        id: notMouseArea
        anchors.fill: iconArea
        enabled: hasNegative && application.advancedMode
        onClicked: switchNegative()
        onPressed: privateStyle.play(Symbian.BasicButton)
        onReleased: privateStyle.play(Symbian.BasicButton)
    }
}


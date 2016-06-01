import QtQuick 1.1
import com.nokia.symbian 1.1

Rectangle {
    id: mainHeading

    property bool edit: false

    signal clicked
    signal pressAndHold

    gradient: Gradient {
        GradientStop { position: 0; color: "#00808c" }
        GradientStop { position: 1; color: "#023f62" }
    }
    clip: true
    opacity: headingMouseArea.pressed ? 0.8 : 1

    Image {
        anchors.horizontalCenter: parent.right
        anchors.verticalCenter: parent.bottom
        anchors.horizontalCenterOffset: -10
        anchors.verticalCenterOffset: -10
        width: parent.height * 2.5
        height: parent.height * 2.5
        sourceSize.width: width
        sourceSize.height: height

        source: "qrc:/gfx/application/situations.svg"
        smooth: true
        opacity: edit || !situations.enabled ? 0 : 1

        Behavior on opacity { NumberAnimation { duration: 200 } }
    }

    Label {
        anchors.fill: parent
        anchors.margins: 15
        verticalAlignment: Text.AlignBottom
        font.pixelSize: height / 2
        text: qsTrId("application_name")
    }

    MouseArea {
        id: headingMouseArea
        anchors.fill: parent
        enabled: !edit
        onClicked: {
            mainHeading.clicked()
            privateStyle.play(Symbian.BasicButton)
        }
        onPressed: privateStyle.play(Symbian.BasicButton)
        onPressAndHold: mainHeading.pressAndHold()
    }

    Switch {
        id: mainSwitch
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.margins: 10
        opacity: edit || !situations.enabled ? 1 : 0
        Behavior on opacity { NumberAnimation { duration: 200 } }

        checked: situations.enabled
        onClicked: client.reqSetEnabled(!situations.enabled)
    }

    Label {
        anchors.right: parent.right
        anchors.bottom: mainSwitch.top
        anchors.rightMargin: 10
        anchors.bottomMargin: 5
        opacity: mainSwitch.opacity
        text: mainSwitch.checked ? qsTrId("application_enabled") : qsTrId("application_disabled")
    }
}


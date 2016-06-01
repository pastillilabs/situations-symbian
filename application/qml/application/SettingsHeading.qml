import QtQuick 1.1
import com.nokia.symbian 1.1

Rectangle {
    property int index: 0
    clip: true

    gradient: Gradient {
        GradientStop { position: 0; color: "#0D0D0D" }
        GradientStop { position: 0.1; color: "#404040" }
        GradientStop { position: 0.9; color: "#1D1D1D" }
        GradientStop { position: 1; color: "#0D0D0D" }
    }

    Column {
        width: parent.width
        anchors.centerIn: parent
        spacing: 5

        Label {
            width: parent.width
            horizontalAlignment: Text.AlignHCenter
            elide: Text.ElideRight
            text: situation ? situation.name : ""
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter

            TabL {
                width: 160
                text: qsTrId("settings_tab_conditions")
                active: index == 0
                onClicked: index = 0
            }

            TabR {
                width: 160
                text: qsTrId("settings_tab_actions")
                active: index == 1
                onClicked: index = 1
            }
        }
    }
}


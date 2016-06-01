import QtQuick 1.1
import com.nokia.symbian 1.1

Page {
    id: page
    property alias contentHeight: column.height

    Column {
        id: column

        Repeater {
            model: situationIndex ? situationConditions : undefined

            ConditionGroup {
                width: page.width
                groupIndex: index

                Label {
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.top
                    font.pixelSize: 12
                    text: groupIndex ? qsTrId("group_and") : ""
                }
            }
        }
    }

    Label {
        anchors.fill: parent
        anchors.margins: 15
        visible: situationIndex == 0
        text: qsTrId("settings_conditions_general_text")
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        wrapMode: Text.WordWrap
    }
}


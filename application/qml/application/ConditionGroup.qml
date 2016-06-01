import QtQuick 1.1
import com.nokia.symbian 1.1

Item {
    property int groupIndex: index

    height: column.height + (2 * column.y)

    BorderImage {
        anchors.fill: column
        border.left: 10
        border.right: 10
        border.top: 10
        border.bottom: 10
        smooth: true
        source: "qrc:/gfx/application/GroupBackground.svg"
        visible: repeater.count
    }

    Column {
        id: column
        x: 5
        y: 10
        width: parent.width - (2 * x)

        GroupHeader {
            width: parent.width
            enabled: repeater.count < groupMaxCount
            hasNegative: true
            onAdd: {
                if(groupEditor) newLoader.source = groupEditor
                else client.reqAddCondition(situationIndex, groupIndex, undefined)
            }
            onSwitchNegative: client.reqSetConditionGroupNegative(situationIndex, groupIndex, !group.negative)

            DialogLoader {
                id: newLoader
                property int itemIndex: -1
            }
        }

        Column {
            move: Transition { NumberAnimation { properties: "y"; duration: 200; easing.type: Easing.OutQuad } }
            add: Transition { NumberAnimation { properties: "y"; duration: 200; easing.type: Easing.OutQuad } }

            Repeater {
                id: repeater
                model: group

                Loader {
                    id: loader
                    x: 1
                    width: column.width - (2 * x)
                    source: groupItem

                    Connections {
                        target: loader.item
                        onRemoveClicked: client.reqRmvCondition(situationIndex, groupIndex, index)
                    }

                    Label {
                        z: parent.z + 1
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.top
                        font.pixelSize: 12
                        text: index ? qsTrId("group_or") : ""
                    }
                }
            }
        }

        Item {
            width: column.width
            height: 10
            visible: repeater.count
        }
    }
}


import QtQuick 1.1
import com.nokia.symbian 1.1
import "qrc:/qml/application"

PluginSwitchItem {
    checked: action.value
    text: action.value ? qsTrId("setting_turn_on") : qsTrId("setting_turn_off")

    onClicked: {
        action.value = !action.value
        client.reqSetActionData(situationIndex, groupIndex, itemIndex, action.data)
    }
}


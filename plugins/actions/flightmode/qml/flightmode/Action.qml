import "qrc:/qml/application"

PluginSwitchItem {
    checked: action.value ? action.value : false
    text: checked ? qsTrId("setting_turn_on") : qsTrId("setting_turn_off")

    onClicked: {
        action.value = !action.value
        client.reqSetActionData(situationIndex, groupIndex, itemIndex, action.data)
    }
}


import "qrc:/qml/application"

PluginTextItem {
    text: condition.lessThan ? qsTrId("condition_battery_less_than_value").arg(condition.value) : qsTrId("condition_battery_more_than_value").arg(condition.value)
}


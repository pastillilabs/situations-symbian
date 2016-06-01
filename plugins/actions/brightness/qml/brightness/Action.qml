import QtQuick 1.1
import com.nokia.symbian 1.1
import "qrc:/qml/application"

PluginItem {
    clickEnabled: false

    content: Slider {
        id: slider
        anchors.fill: parent

        maximumValue: action.maxBrightness()
        minimumValue: action.minBrightness()
        value: action.value
        onValueChanged: if(value != action.value) {
            action.value = slider.value
            reqTimer.restart()
        }
        stepSize: 25

        Timer {
            id: reqTimer
            interval: 200
            onTriggered: client.reqSetActionData(situationIndex, groupIndex, itemIndex, action.data)
        }
    }
}


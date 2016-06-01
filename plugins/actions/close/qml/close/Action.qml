import "qrc:/qml/application"

PluginTextItem {
    text: action.nameById(action.value)
    endIconVisible: action.situationEnd
}


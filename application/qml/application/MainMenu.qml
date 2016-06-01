import QtQuick 1.1
import com.nokia.symbian 1.1

Menu {
    MenuLayout {
        MenuItem {
            text: client.notifications ? qsTrId("disable_notifications") : qsTrId("enable_notifications")
            onClicked: client.reqSetNotifications(!client.notifications)
        }
        MenuItem {
            text: application.advancedMode ? qsTrId("set_advanced_mode_off") : qsTrId("set_advanced_mode_on")
            onClicked: application.advancedMode = !application.advancedMode
        }
        MenuItem {
            text: qsTrId("update_localisation")
            onClicked: translationUpdateDialog.open()
        }
    }

    Loader {
        id: translationUpdateDialog

        function open() {
            source = "qrc:/qml/application/TranslationUpdateDialog.qml"
            item.open()
            translationUpdater.update()
        }
    }
}


import QtQuick 1.1
import com.nokia.symbian 1.1
import com.pastillilabs.situations.action.theme 1.0

SelectionDialog {
    titleText: qsTrId("action_theme_name")

    Component.onCompleted: if(itemIndex >= 0) {
        action.data = group.getActionData(itemIndex)
    }

    onAccepted: {
        if(itemIndex >= 0) client.reqSetActionData(situationIndex, groupIndex, itemIndex, action.data)
        else client.reqAddAction(situationIndex, groupIndex, action.data)
    }

    selectedIndex: themeList.index(action.value)

    ThemeAction {
        id: action
    }

    ThemeListModel {
        id: themeList
    }

    model: themeList
    delegate: MenuItem {
        text: themeName
        onClicked: {
            selectedIndex = index
            action.value = themeList.id(selectedIndex)
            root.accept()
        }

        Keys.onPressed: {
            if (event.key == Qt.Key_Up || event.key == Qt.Key_Down) scrollBar.flash()
        }
    }
}


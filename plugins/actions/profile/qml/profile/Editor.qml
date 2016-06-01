import QtQuick 1.1
import com.nokia.symbian 1.1
import com.pastillilabs.situations.action.profile 1.0

SelectionDialog {
    titleText: qsTrId("action_profile")

    Component.onCompleted: if(itemIndex >= 0) {
        action.data = group.getActionData(itemIndex)
        selectedIndex = action.value
    }

    onAccepted: {
        if(itemIndex >= 0) client.reqSetActionData(situationIndex, groupIndex, itemIndex, action.data)
        else client.reqAddAction(situationIndex, groupIndex, action.data)
    }

    ProfileAction {
        id: action
        value: selectedIndex
    }

    ProfileListModel {
        id: profileListModel
    }

    model: profileListModel
    delegate: MenuItem {
        text: profileName
        onClicked: {
            selectedIndex = index
            action.value = profileListModel.id(selectedIndex)
            accept()
        }

        Keys.onPressed: {
            if (event.key == Qt.Key_Up || event.key == Qt.Key_Down) scrollBar.flash()
        }
    }
}


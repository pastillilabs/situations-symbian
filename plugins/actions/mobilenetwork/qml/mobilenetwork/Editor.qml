import QtQuick 1.1
import com.nokia.symbian 1.1
import com.pastillilabs.situations.action.mobilenetwork 1.0

SelectionDialog {
    titleText: qsTrId("action_mobilenetwork_name")

    Component.onCompleted: if(itemIndex >= 0) {
        action.data = group.getActionData(itemIndex)
        selectedIndex = action.value
    }

    onAccepted: {
        if(itemIndex >= 0) client.reqSetActionData(situationIndex, groupIndex, itemIndex, action.data)
        else client.reqAddAction(situationIndex, groupIndex, action.data)
    }

    MobileNetworkAction {
        id: action
        value: selectedIndex
    }

    ListModel {
        id: mobileNetworkListModel
        Component.onCompleted: {
            for(var i=0; i < 3; ++i) {
                append({"name":action.nameById(i)})
            }
        }
    }

    model: mobileNetworkListModel
    delegate: MenuItem {
        text: name
        onClicked: {
            selectedIndex = index
            action.value = index
            accept()
        }

        Keys.onPressed: {
            if (event.key == Qt.Key_Up || event.key == Qt.Key_Down) scrollBar.flash()
        }
    }
}


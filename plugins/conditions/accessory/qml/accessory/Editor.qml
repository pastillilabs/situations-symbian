import QtQuick 1.1
import com.nokia.symbian 1.1
import com.pastillilabs.situations.condition.accessory 1.0

SelectionDialog {
    titleText: qsTrId("condition_accessory_name")

    Component.onCompleted: if(itemIndex >= 0) {
        condition.data = group.getConditionData(itemIndex)
        selectedIndex = condition.value
    }

    onAccepted: {
        if(itemIndex >= 0) client.reqSetConditionData(situationIndex, groupIndex, itemIndex, condition.data)
        else client.reqAddCondition(situationIndex, groupIndex, condition.data)
    }

    AccessoryCondition {
        id: condition
    }

    model: 4

    delegate: MenuItem {
        text: condition.nameById(index)
        onClicked: {
            selectedIndex = index
            condition.value = index
            root.accept()
        }

        Keys.onPressed: {
            if (event.key == Qt.Key_Up || event.key == Qt.Key_Down) scrollBar.flash()
        }
    }
}


import QtQuick 1.1
import com.nokia.symbian 1.1
import com.pastillilabs.situations.condition.calendar 1.0

CommonDialog {
    titleText: qsTrId("condition_calendar_name")
    buttonTexts: [qsTrId("dialog_softkey_ok"), qsTrId("dialog_softkey_cancel")]

    Component.onCompleted: if(itemIndex >= 0) {
        condition.data = group.getConditionData(itemIndex)
    }

    onButtonClicked: index === 0 ? accept() : reject()
    onAccepted: {
        if(itemIndex >= 0) client.reqSetConditionData(situationIndex, groupIndex, itemIndex, condition.data)
        else client.reqAddCondition(situationIndex, groupIndex, condition.data)
    }

    CalendarCondition {
        id: condition
    }

    CalendarCollections {
        id: collections
    }

    content: Flickable {
        id: flickable
        width: parent.width
        height: Math.min(platformContentMaximumHeight, contentHeight + 1)
        contentWidth: width
        contentHeight: column.height + column.y * 2
        flickableDirection: Flickable.VerticalFlick
        interactive: contentHeight > height

        Column {
            id: column
            x: 20
            y: 20
            width: parent.width - 2 * x
            spacing: 20

            CheckBox {
                id: alarmCheck
                width: parent.width
                text: qsTrId("checkbox_calendar_alarm")
                checked: condition.alarm
                onCheckedChanged: condition.alarm = checked
            }

            CheckBox {
                id: recurringCheck
                width: parent.width
                text: qsTrId("checkbox_calendar_recurring")
                checked: condition.recurring
                onCheckedChanged: condition.recurring = checked
            }

            CheckBox {
                id: noAllDayCheck
                width: parent.width
                text: qsTrId("checkbox_calendar_no_allday")
                checked: condition.noAllDay
                onCheckedChanged: condition.noAllDay = checked
            }

            Button {
                id: collectionButton
                width: parent.width
                text: collections.nameById(condition.collectionId)
                onClicked: if(collections.count() > 1) calendarSelection.open()
            }

            TextField {
                id: keywordField
                width: parent.width
                placeholderText: qsTrId("textfield_calendar_keyword_placholder")
                maximumLength: 100
                text: condition.keyword
                onTextChanged: condition.keyword = text
            }

            Connections {
                target: inputContext
                onVisibleChanged: if(inputContext.visible) flickable.contentY = flickable.contentHeight - flickable.height - column.y
            }
        }
    }

    ScrollBar {
        parent: flickable
        anchors.right: parent.right
        flickableItem: parent
    }

    SelectionDialog {
        id: calendarSelection
        titleText: qsTrId("calendar_select")
        model: collections.names()
        delegate: MenuItem {
            text: modelData
            onClicked: {
                condition.collectionId = collections.idByIndex(index)
                accept()
            }
        }
    }
}


import QtQuick 1.1
import com.nokia.symbian 1.1

Page {
    id: page
    property alias contentHeight: column.height

    Column {
        id: column

        Repeater {
            model: situationActions

            ActionGroup {
                width: page.width
            }
        }
    }
}


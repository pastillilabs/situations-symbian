import QtQuick 1.1
import com.nokia.symbian 1.1

Row {
    property alias hour: hourPicker.currentIndex
    property alias minute: minutePicker.currentIndex
    spacing: 2

    Picker {
        id: hourPicker
        width: 60
        height: 120
        model: 24
    }

    Picker {
        id: minutePicker
        width: 60
        height: 120
        model: 60
    }
}


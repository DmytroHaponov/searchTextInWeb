import QtQuick 2.7

Item {
    id: root
    objectName: "results"

    height: _resultsRow.height + _titleRect.height

    Rectangle {
        id: _titleRect
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        height: 12
        color: "#1a1818"
        Text {
            id: _resultTitle
            anchors.centerIn: parent
            text: qsTr("results:")
        }
    }

    Column {
        id: _resultsRow
        spacing: 5
        anchors.centerIn: parent

        Repeater {
            id: _resultItem
            model: search_engine.results

            Rectangle {
                id: _resultWrapper
                height: 28
                color: "#83d3ca"
                border.color: "#c6dd2c"
                width: _columnsRow.width

                Rectangle {
                    id: _lineRect
                    height: parent.height/2
                    width: parent.width
                    color: "blue"
                    border.color: "yellow"

                    Text {
                        id: lineText
                        anchors.top: parent.top
                        anchors.topMargin: 2
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: qsTr(modelData.line)
                        font.pixelSize: 12
                    }
                }

                Row {
                    id: _columnsRow
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 2
                    height: parent.height
                    spacing: 5
                    Repeater {
                        id: _column
                        model: modelData.columns

                        Text {
                            id: name
                            text: qsTr(modelData)
                            font.pixelSize: 12
                        }
                    }
                }
            }
        }
    }
}


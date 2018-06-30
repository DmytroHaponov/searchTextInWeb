import QtQuick 2.7
import QtQuick.Controls 2.2

Item {
    id: root
    property alias text: _URLlabel.text
    property alias input: _url.text
    property alias inputWidth: _editRect.width

    height: _editRect.height + _URLlabel.height
    width: _editRect.width > _URLlabel.width
           ? _editRect.width
           : _URLlabel.width

    Label {
        id: _URLlabel

        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter

        background: Rectangle {
            id: _URLlabelBack
            gradient: Gradient {
                GradientStop { position: 0.4; color: "purple" }
                GradientStop { position: 1.0; color: "green" }
            }
        }
        color: "white"
        height: 30
        width: contentWidth
    }

    Rectangle {
        id: _editRect
        anchors.top: _URLlabel.bottom
        anchors.topMargin: 1
        anchors.horizontalCenter: parent.horizontalCenter
        border.color: "red"
        height: 30
        width: 100

        TextEdit {
            id: _url
            anchors.fill: parent
            color: "purple"
            padding: 5
        }
    }
}

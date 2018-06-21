import QtQuick 2.6
import QtQuick.Controls 2.2

Item {
    id: root
    property alias text: _URLlabel.text
    property alias input: _url.text

    height: _editRect.height + _URLlabel.height
    width: _editRect.width > _URLlabel.width
           ? _editRect.width
           : _URLlabel.width

    Rectangle {
        id: _editRect
        border.color: "red"
        height: 50
        width: 100

        TextEdit {
            id: _url
            anchors.fill: parent
            color: "purple"
            padding: 5
        }
    }

    Label {
        id: _URLlabel

        anchors.bottom: _editRect.top
        anchors.bottomMargin: 3
        anchors.horizontalCenter: _editRect.horizontalCenter

        background: Rectangle {
            id: _URLlabelBack
            gradient: Gradient {
                GradientStop { position: 0.4; color: "white" }
                GradientStop { position: 1.0; color: "green" }
            }
        }

        height: 30
        width: contentWidth
    }
}

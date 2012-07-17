// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Rectangle {
    id: root
    width: 800
    height: 600

    property int basicMargin: 10

    Image {
        id: background
        source: "clouds.jpg"
        smooth: true
        width: parent.width
        height: parent.height
        fillMode: Image.Stretch
    }


    ListView {
        id: object_list

        model: objects

        anchors.top: title.bottom
        anchors.topMargin: basicMargin

        anchors.left: parent.left
        anchors.leftMargin: basicMargin

        anchors.right: parent.right
        anchors.rightMargin: basicMargin

        anchors.bottom: root.bottom
        anchors.bottomMargin: basicMargin

        ScrollBar {
            flickable: object_list
            vertical: true
        }

        // ListView.onAdd: ListView.positionViewAtEnd()

        delegate: ObjectDelegate{}
    }

    Rectangle {
        id: title
        height: childrenRect.height + basicMargin
        width: parent.width
        opacity: 0.8

        gradient: Gradient {
            GradientStop { position: 0.0; color: "white" }
            GradientStop { position: 0.867; color: "white" }
            GradientStop { position: 1.0; color: "lightblue" }
        }

        Text {
            text: "Object Connoisseur™"
            font.pointSize: 30

            anchors.left: parent.left
            anchors.leftMargin: basicMargin

            anchors.right: parent.right
            anchors.rightMargin: basicMargin

            anchors.top: parent.top
            anchors.topMargin: basicMargin / 2
        }
    }


}

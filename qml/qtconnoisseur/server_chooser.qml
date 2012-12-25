import QtQuick 2.0

Rectangle {
    id: root
    width: 800
    height: 600

    signal okButtonClicked(string host, string port)

    Image {
        id: background
        source: "clouds.jpg"
        smooth: true
        width: parent.width
        height: parent.height
        fillMode: Image.Stretch
    }

    Grid {
        columns: 2
        spacing: 10

        anchors.centerIn: parent

        Text { text: "Server"
               font.bold: true
               id: server_label }
        Rectangle {
            color: "lightsteelblue"
            width: server_label.width * 2
            height: server_label.height
            TextInput { id: serverInput; font.capitalization: Font.AllLowercase;
                        width: parent.width; height: parent.height }
        }

        Text { text: "Port"
               font.bold: true }
        Rectangle {
            color: "lightsteelblue"
            width: server_label.width * 2
            height: server_label.height
            TextInput { id: portInput; font.capitalization: Font.AllLowercase
                        validator: IntValidator{ bottom: 1025; top: 65500; }
                        width: parent.width; height: parent.height }
        }

        Text { text: " " }
        MouseArea {
            width: server_label.width * 2
            height: server_label.height

            onClicked: {
                buttonRectangle.color = "gray"

                // flow.startFlow(serverInput.text, portInput.text)
                root.okButtonClicked(serverInput.text, portInput.text)
                // connectionProperties["host"] = serverInput.text
                // connectionProperties["port"] = portInput.text
            }

            Rectangle {
                id: buttonRectangle
                height: parent.height
                width: parent.width
                color: "lightsteelblue"
                Text { anchors.centerIn: parent; text: "OK" }
            }
        }
    }
}

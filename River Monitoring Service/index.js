const express = require("express");
const mqtt = require("mqtt");
const topic = "waterlevel";

//mqtt
const mqttclient = mqtt.connect("localhost", {
  protocol: "mqtt",
  clientId: "waterlevel-js",
});

mqttclient.on("connect", () => {
  mqttclient.subscribe(topic, (err) => {
    if (err) {
      console.log(err);
    }
  });
});

mqttclient.on("message", (_, message) => {
  console.log(message.toString());
});

//http
var app = express();
app.get("/", function (request, response) {
  response.send("Hello World!");
});
app.listen(10000, function () {
  console.log("Started application on port %d", 10000);
});

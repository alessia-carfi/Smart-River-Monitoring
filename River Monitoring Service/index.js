const express = require("express");
const mqtt = require("mqtt");
const { SerialPort } = require("serialport");
const { ReadlineParser } = require("@serialport/parser-readline");
const TOPIC = "water-level";

const MACHINE_STATE = {
  ALARM_TOO_LOW: "ALARM_TOO_LOW",
  NORMAL: "NORMAL",
  PRE_ALARM_TOO_HIGH: "PRE_ALARM_TOO_HIGH",
  ALARM_TOO_HIGH: "ALARM_TOO_HIGH",
  ALARM_TOO_HIGH_CRITIC: "ALARM_TOO_HIGH_CRITIC",
};

const VALVE_INPUT = {
  AUTOMATIC: "AUTOMATIC",
  MANUAL: "MANUAL",
  ADMIN: "ADMIN",
};

const WL1 = 10;
const WL2 = 20;
const WL3 = 30;
const WL4 = 40;
const F1 = 5000;
const F2 = 2000;

let currentWaterLevel = 0;
let monitoringFrequency = F1;
let valveOpeningLevel = 25;
let valveInput = VALVE_INPUT.AUTOMATIC;
let valveState = MACHINE_STATE.NORMAL;

//mqtt
const mqttclient = mqtt.connect("localhost", {
  protocol: "mqtt",
  clientId: "waterlevel-js",
});

mqttclient.on("connect", () => {
  mqttclient.subscribe(TOPIC, (err) => {
    if (err) {
      console.log(err);
    }
  });
});

mqttclient.on("message", (topic, message) => {
  // console.log(message.toString());
  if (topic === TOPIC) {
    currentWaterLevel = parseFloat(message.toString());
    updateSystemState();
  }
});

const serialPort = new SerialPort({
  // alexa: path: "/dev/tty0",
  path: "COM5",
  baudRate: 115200,
});

const parser = serialPort.pipe(new ReadlineParser({ delimiter: "\n" }));

parser.on("data", (data) => {
  console.log(`Received from Arduino: ${data}`);
});

//http
var app = express();
app.get("/", function (req, res) {
  // response.send("Hello World!");
  res.json({
    currentWaterLevel,
    valveOpeningLevel,
    monitoringFrequency,
  });
});

app.listen(10000, function () {
  console.log("Started application on port %d", 10000);
});

function updateSystemState() {
  if (currentWaterLevel < WL1) {
    valveOpeningLevel = 0;
    monitoringFrequency = F1;
    valveState = MACHINE_STATE.ALARM_TOO_LOW;
    console.log("State: ALARM-TOO-LOW");
  } else if (currentWaterLevel >= WL1 && currentWaterLevel <= WL2) {
    valveOpeningLevel = 25;
    monitoringFrequency = F1;
    valveState = MACHINE_STATE.NORMAL;
    console.log("State: NORMAL");
  } else if (currentWaterLevel > WL2 && currentWaterLevel <= WL3) {
    valveOpeningLevel = 0.5 * 100;
    monitoringFrequency = F2;
    valveState = MACHINE_STATE.PRE_ALARM_TOO_HIGH;
    console.log("State: PRE-ALARM-TOO-HIGH");
  } else if (currentWaterLevel > WL3 && currentWaterLevel <= WL4) {
    valveOpeningLevel = 50;
    monitoringFrequency = F2;
    valveState = MACHINE_STATE.ALARM_TOO_HIGH;
    console.log("State: ALARM-TOO-HIGH");
  } else {
    valveOpeningLevel = 100;
    monitoringFrequency = F2;
    valveState = MACHINE_STATE.ALARM_TOO_HIGH_CRITIC;
    console.log("State: ALARM-TOO-HIGH-CRITIC");
  }

  sendDataToArduino(valveInput, valveOpeningLevel);
}

function mapValue(value, inputMin, inputMax, outputMin, outputMax) {
  return (
    ((value - inputMin) / (inputMax - inputMin)) * (outputMax - outputMin) +
    outputMin
  );
}

function sendDataToArduino(valveInput, valveOpeningLevel) {
  serialPort.write(
    `${valveInput}_${mapValue(valveOpeningLevel, 0, 100, 0, 180)}\n`,
    (err) => {
      if (err) {
        return console.error("Error on write: ", err.message);
      }
      console.log(
        `${valveInput}_${mapValue(valveOpeningLevel, 0, 100, 0, 180)}`
      );
    }
  );
}

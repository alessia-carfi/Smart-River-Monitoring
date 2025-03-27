const express = require("express");
const mqtt = require("mqtt");
const cors = require("cors");
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

const WL1 = 30;
const WL2 = 20;
const WL3 = 10;
const WL4 = 5;
const WATER_LEVEL_MAX = 50;
const F1 = 5000;
const F2 = 1000;

let currentWaterLevel = 0;
let effectiveWaterLevel = 0;
let monitoringFrequency = F1;
let oldFrequency = F1;
let serverValveOpeningLevel = 25;
let serverValveInput = VALVE_INPUT.AUTOMATIC;
let valveState = MACHINE_STATE.NORMAL;

//mqtt
const mqttclient = mqtt.connect("mqtt://192.168.72.48:1883", {
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
  path: "/dev/ttyACM0",
  // path: "/dev/tty0",
  // path: "COM4",
  baudRate: 115200,
});

const parser = serialPort.pipe(new ReadlineParser({ delimiter: "\n" }));

parser.on("data", (data) => {
  console.log(`Received from Arduino: ${data}`);
  const [serialValveInput, serialValveOpeningLevel] = data.split("-");
  serverValveInput = serialValveInput;
  serverValveOpeningLevel = parseInt(
    mapValue(serialValveOpeningLevel, 0, 180, 0, 100)
  );
});

//http
var app = express();
app.use(cors());
app.use(express.json());

app.get("/", function (req, res) {
  // response.send("Hello World!");
  res.json({
    effectiveWaterLevel,
    valveOpeningLevel: serverValveOpeningLevel,
    valveState,
    time: new Date().toLocaleTimeString(),
  });
});

app.post("/data", (req, res) => {
  const { valveInput, valveOpeningLevel } = req.body;
  console.log("Received data:", { valveInput, valveOpeningLevel });

  serverValveInput = valveInput;
  serverValveOpeningLevel = parseInt(valveOpeningLevel);

  res.json({
    message: "Data received successfully",
    receivedData: { valveInput, valveOpeningLevel },
  });
});

app.listen(10000, function () {
  console.log("Started application on port %d", 10000);
});

function updateSystemState() {
  effectiveWaterLevel = WATER_LEVEL_MAX - currentWaterLevel;
  if (currentWaterLevel > WL1) {
    if (serverValveInput === VALVE_INPUT.AUTOMATIC) {
      serverValveOpeningLevel = 0;
    }
    monitoringFrequency = F1;
    valveState = MACHINE_STATE.ALARM_TOO_LOW;
    console.log("State: ALARM-TOO-LOW");
    console.log("currentWaterLevel", currentWaterLevel);
  } else if (currentWaterLevel >= WL2 && currentWaterLevel <= WL1) {
    if (serverValveInput === VALVE_INPUT.AUTOMATIC) {
      serverValveOpeningLevel = 25;
    }
    monitoringFrequency = F1;
    valveState = MACHINE_STATE.NORMAL;
    console.log("State: NORMAL");
    console.log("currentWaterLevel", currentWaterLevel);
  } else if (currentWaterLevel > WL3 && currentWaterLevel <= WL2) {
    if (serverValveInput === VALVE_INPUT.AUTOMATIC) {
      serverValveOpeningLevel = 0.5 * 100;
    }
    monitoringFrequency = F2;
    valveState = MACHINE_STATE.PRE_ALARM_TOO_HIGH;
    console.log("State: PRE-ALARM-TOO-HIGH");
    console.log("currentWaterLevel", currentWaterLevel);
  } else if (currentWaterLevel > WL4 && currentWaterLevel <= WL3) {
    if (serverValveInput === VALVE_INPUT.AUTOMATIC) {
      serverValveOpeningLevel = 50;
    }
    monitoringFrequency = F2;
    valveState = MACHINE_STATE.ALARM_TOO_HIGH;
    console.log("State: ALARM-TOO-HIGH");
    console.log("currentWaterLevel", currentWaterLevel);
  } else {
    if (serverValveInput === VALVE_INPUT.AUTOMATIC) {
      serverValveOpeningLevel = 100;
    }
    monitoringFrequency = F2;
    valveState = MACHINE_STATE.ALARM_TOO_HIGH_CRITIC;
    console.log("State: ALARM-TOO-HIGH-CRITIC");
    console.log("currentWaterLevel", currentWaterLevel);
  }

  if (oldFrequency !== monitoringFrequency) {
    oldFrequency = monitoringFrequency;
    mqttclient.publish("water-frequency", String(monitoringFrequency));
  }
  sendDataToArduino(serverValveInput, serverValveOpeningLevel);
}

function mapValue(value, inputMin, inputMax, outputMin, outputMax) {
  return (
    ((value - inputMin) / (inputMax - inputMin)) * (outputMax - outputMin) +
    outputMin
  );
}

function sendDataToArduino(valveInput, valveOpeningLevel) {
  serialPort.write(
    `${valveInput}-${mapValue(valveOpeningLevel, 0, 100, 0, 180)}\n`,
    (err) => {
      if (err) {
        return console.error("Error on write: ", err.message);
      }
    }
  );
}

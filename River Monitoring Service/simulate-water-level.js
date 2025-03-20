const mqtt = require("mqtt");

const mqttClient = mqtt.connect("mqtt://localhost:1883");

mqttClient.on("connect", () => {
  console.log("Connected to MQTT broker");

  const waterLevels = [5, 15, 25, 35, 45];
  let index = 0;

  setInterval(() => {
    if (index < waterLevels.length) {
      const level = waterLevels[index];
      mqttClient.publish("water-level", level.toString());
      console.log(`Published water level: ${level}`);
      index++;
    } else {
      index = 0;
    }
  }, 5000);
});

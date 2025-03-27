import { useData } from "../hooks/UseData";
import { useState } from "react";

function ValveController() {
  const { sendData } = useData();
  const [valveOpeningLevel, setValveOpeningLevel] = useState("");
  const [valveInput, setValveInput] = useState("");

  const handleSubmit = (e) => {
    e.preventDefault();
    setValveInput("ADMIN");
    sendData(valveInput, valveOpeningLevel);
  };

  const handleCancel = (e) => {
    e.preventDefault();
    setValveInput("AUTOMATIC");
    sendData(valveInput, valveOpeningLevel);
  };

  return (
    <div className="valve-controller-container">
      <h2>ADMIN VALVE CONTROLLER</h2>
      <form onSubmit={handleSubmit}>
        <input
          type="range"
          min="0"
          max="100"
          value={valveOpeningLevel}
          onChange={(e) => setValveOpeningLevel(e.target.value)}
        />
        <p>Valve is open at: {valveOpeningLevel || 50}%</p>
        <button type="submit">Submit</button>
      </form>
      <form onSubmit={handleCancel}>
        <button type="submit">Cancel</button>
      </form>
    </div>
  );
}
export default ValveController;

import { useData } from "../hooks/UseData";
import { useState } from "react";
const valveInput = "ADMIN";

function ValveController() {
  const { sendData } = useData();
  const [valveOpeningLevel, setValveOpeningLevel] = useState("");

  const handleSubmit = (e) => {
    e.preventDefault();
    const data = { valveOpeningLevel, valveInput };
    sendData(data);
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
    </div>
  );
}
export default ValveController;

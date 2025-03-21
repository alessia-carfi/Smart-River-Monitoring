import { useData } from "../hooks/UseData";
import { useState } from "react";
const MODE = "ADMIN";

function ValveController() {
  const { sendData, response, error } = useData();
  const [num, setNum] = useState("");

  const handleSubmit = (e) => {
    e.preventDefault();
    const data = { num, MODE };
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
          value={num}
          onChange={(e) => setNum(e.target.value)}
        />
        <p>Valve is open at: {num}%</p>
        <button type="submit">Submit</button>
      </form>
    </div>
  );
}
export default ValveController;

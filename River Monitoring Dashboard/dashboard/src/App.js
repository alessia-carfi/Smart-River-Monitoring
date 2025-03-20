import "./App.css";
import Chart from "chart.js/auto";
import { CategoryScale } from "chart.js";
import { useState } from "react";
import Linechart from "./components/Linechart";
import { Data } from "./components/Data";
import ValveState from "./components/ValveState";
import ValveController from "./components/ValveController";
import State from "./components/State";

Chart.register(CategoryScale);

function App() {
  const [chartData, setChartData] = useState({
    labels: Data.map((data) => data.year),
    datasets: [
      {
        label: "Users Gained ",
        data: Data.map((data) => data.userGain),
        backgroundColor: [
          "rgba(75,192,192,1)",
          "#ecf0f1",
          "#50AF95",
          "#f3ba2f",
          "#2a71d0",
        ],
        borderColor: "black",
        borderWidth: 2,
      },
    ],
  });
  return (
    <div className="App">
      <Linechart chartData={chartData} />
      <State mode="MANUAL" />
      <ValveState num={50} />
      <ValveController num={50} />
    </div>
  );
}

export default App;

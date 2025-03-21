import "./App.css";
import Chart from "chart.js/auto";
import { CategoryScale } from "chart.js";
import { useState } from "react";
import Linechart from "./components/Linechart";
import ValveState from "./components/ValveState";
import ValveController from "./components/ValveController";
import State from "./components/State";
import { DataProvider } from "./context/DataContext";
import { useData } from "./hooks/UseData";

Chart.register(CategoryScale);

export default function App() {
  return (
    <DataProvider>
      <AppComponents />
    </DataProvider>
  );
}

function AppComponents() {
  const { espData, error } = useData();
  const [chartData, setChartData] = useState({
    datasets: [
      {
        label: "Water Level: ",
        data: espData ? espData.map((data) => data.currentWaterLevel) : null,
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
      <ValveController />
      error && <p style={{ color: "red" }}>{error}</p>}
    </div>
  );
}

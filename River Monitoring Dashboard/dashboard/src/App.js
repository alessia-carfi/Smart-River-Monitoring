import "./App.css";
import Chart from "chart.js/auto";
import { CategoryScale } from "chart.js";
import { useEffect, useState } from "react";
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
  const [chartData, setChartData] = useState({ datasets: [] });

  useEffect(() => {
    setChartData({
      labels: espData ? espData.map((item) => item["time"]) : [],
      datasets: [
        {
          label: "Water Level",
          data: espData ? espData.map((item) => item["currentWaterLevel"]) : [],
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
  }, [espData]);

  return (
    <div className="App">
      <Linechart chartData={chartData} />
      <State
        mode={
          espData.length > 0 ? espData[espData.length - 1]["valveInput"] : ""
        }
      />
      <ValveState
        num={
          espData.length > 0
            ? espData[espData.length - 1]["valveOpeningLevel"]
            : ""
        }
      />
      <ValveController />
      {error && <p style={{ color: "red" }}>{error}</p>}
    </div>
  );
}

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
      labels: espData ? espData.slice(-20).map((item) => item["time"]) : [],
      datasets: [
        {
          label: "Water Level",
          data: espData
            ? espData.slice(-20).map((item) => item["effectiveWaterLevel"])
            : [],
          backgroundColor: ["rgba(75,192,192,1)"],
          borderColor: "black",
          borderWidth: 2,
          fill: true,
        },
      ],
      options: {
        scales: {
          y: {
            min: 0,
            max: 100,
            ticks: {
              stepSize: 15,
            },
          },
        },
      },
    });
  }, [espData]);

  return (
    <div className="App">
      <Linechart chartData={chartData} />
      <State
        mode={
          espData.length > 0 ? espData[espData.length - 1]["valveState"] : ""
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

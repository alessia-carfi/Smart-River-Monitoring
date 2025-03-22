import React from "react";
import { Line } from "react-chartjs-2";
function Linechart({ chartData }) {
  return (
    <div className="chart-container">
      <h2 style={{ textAlign: "center" }}>Line Chart</h2>
      <Line
        data={chartData}
        options={{
          plugins: {
            title: {
              display: true,
              text: "WATER LEVEL",
            },
            legend: {
              display: false,
            },
          },
        }}
      />
    </div>
  );
}
export default Linechart;

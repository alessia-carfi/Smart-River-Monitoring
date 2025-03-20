function ValveController({ num }) {
  return (
    <div className="valve-controller-container">
      <h2>ADMIN VALVE CONTROLLER</h2>
      <input type="range" min="0" max="100" value={num} />
      <p>Valve is open at: {num}%</p>
    </div>
  );
}
export default ValveController;

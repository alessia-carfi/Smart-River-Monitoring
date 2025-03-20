function ValveState({ num }) {
  return (
    <div className="valve-state-container">
      <h2>VALVE STATE</h2>
      <p>Valve is open at: {num}%</p>
    </div>
  );
}
export default ValveState;

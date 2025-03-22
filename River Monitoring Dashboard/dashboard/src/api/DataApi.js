class DataApi {
  static async sendData(data) {
    try {
      const response = await fetch("http://localhost:10000/data", {
        method: "POST",
        headers: {
          "Content-Type": "application/json",
        },
        body: JSON.stringify(data),
      });

      if (!response.ok) {
        throw new Error("Network response was not ok");
      }

      return await response.json();
    } catch (error) {
      console.error("Error sending data:", error);
      throw error;
    }
  }

  static async getData() {
    try {
      const response = await fetch("http://localhost:10000");
      if (!response.ok) {
        throw new Error("Network response was not ok");
      }
      return await response.json();
    } catch (error) {
      console.error("Error fetching data:", error);
      throw error;
    }
  }
}

export default DataApi;

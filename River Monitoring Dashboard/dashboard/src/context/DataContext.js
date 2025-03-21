import { createContext, useState, useEffect } from "react";
import DataApi from "../api/DataApi";

export const DataContext = createContext({});

export const DataProvider = ({ children }) => {
  const [response, setResponse] = useState(null);
  const [espData, setEspData] = useState(null);
  const [error, setError] = useState(null);

  const sendData = async (data) => {
    try {
      const result = await DataApi.sendData(data);
      setResponse(result);
      setError(null);
    } catch (err) {
      setError(err.message);
      setResponse(null);
    }
  };

  const fetchData = async () => {
    try {
      const result = await DataApi.getData();
      setEspData(result);
      setError(null);
    } catch (err) {
      setError(err.message);
      setEspData(null);
    }
  };

  useEffect(() => {
    fetchData();

    const intervalId = setInterval(() => {
      fetchData();
    }, 10000);

    return () => clearInterval(intervalId);
  }, []);

  return (
    <DataContext.Provider value={{ sendData, response, error, espData }}>
      {children}
    </DataContext.Provider>
  );
};

/*setInterval(requestData, 5000);
console.log('Client avviato. Richieste ogni 10 sec');*/


function chartLogic(ctx) {
    const dataPoints = [];
    const labels = [];
    const url = 'http://localhost:3000/dati';

    // Initialize Chart.js chart
    const chart = new Chart(ctx, {
        type: 'line',
        data: {
            labels: labels,
            datasets: [{
                label: 'Temperature (°C)',
                data: dataPoints,
                borderColor: 'rgba(75,192,192,1)',
                tension: 0.1,
            }]
        },
        options: {
            scales: {
                x: { title: { display: true, text: 'Time' } },
                y: { title: { display: true, text: 'Temperature (°C)' } }
            }
        }
    });
}

async function requestData() {
    try{
        const response = await fetch(url);
        if(!response.ok) {
            throw new Error(`Errore HTTP! status: ${response.status}`);
        }
        const dati =  await response.json();
        console.log(dati);
        labels.push(dati.timestamp);
        dataPoints.push(dati.value);
        if(labels.length > 10) {
            labels.shift();
            dataPoints.shift();
        }
        chart.update();
    } catch (error) {
        console.log('Errore:',error.message);
    }
    }

// Firebase paths sesuai dengan direktori baru
const dbPathTemp = '/Suhu';          // Path untuk Suhu
const dbPathHum = '/Kelembapan';     // Path untuk Kelembapan
const dbPathAirQuality = '/Gas';     // Path untuk Kualitas Udara (Gas)
const dbPathFire = '/Api';           // Path untuk Deteksi Api
const dbPathLED = '/LED';            // Path untuk LED
const dbPathBuzzer = '/Buzzer';      // Path untuk Buzzer
const dbPathMotor = '/Motor';        // Path untuk Motor
const dbPathMessage = '/Pesan';      // Path untuk Pesan

// Firebase references
const dbRefTemp = firebase.database().ref(dbPathTemp);
const dbRefHum = firebase.database().ref(dbPathHum);
const dbRefAirQuality = firebase.database().ref(dbPathAirQuality);
const dbRefFire = firebase.database().ref(dbPathFire);
const dbRefLED = firebase.database().ref(dbPathLED);
const dbRefBuzzer = firebase.database().ref(dbPathBuzzer);
const dbRefMotor = firebase.database().ref(dbPathMotor);
const dbRefMessage = firebase.database().ref(dbPathMessage);

// Elemen HTML untuk menampilkan data sensor dan status
const tempElement = document.getElementById('temp');
const humElement = document.getElementById('hum');
const airQualityElement = document.getElementById('air-quality');
const fireStatusElement = document.getElementById('fire-status');
const ledStatusElement = document.getElementById('led-status');
const buzzerStatusElement = document.getElementById('buzzer-status');
const motorStatusElement = document.getElementById('motor-status');
const messageElement = document.getElementById('message');

// Membuat instance chart
const chartT = createTemperatureChart();
const chartH = createHumidityChart();
const chartA = createAirQualityChart();

// Update data Suhu
dbRefTemp.on('value', (snapshot) => {
    const temperature = snapshot.val();
    if (temperature !== null) {
        const formattedTemp = parseFloat(temperature).toFixed(2);
        tempElement.innerText = `${formattedTemp} °C`;

        const currentTime = (new Date()).getTime();
        chartT.series[0].addPoint([currentTime, formattedTemp], true, chartT.series[0].data.length > 40);
    } else {
        console.error("Temperature data is null.");
        tempElement.innerText = "N/A";
    }
});

// Update data Kelembapan
dbRefHum.on('value', (snapshot) => {
    const humidity = snapshot.val();
    if (humidity !== null) {
        const formattedHum = parseFloat(humidity).toFixed(2);
        humElement.innerText = `${formattedHum} %`;

        const currentTime = (new Date()).getTime();
        chartH.series[0].addPoint([currentTime, formattedHum], true, chartH.series[0].data.length > 40);
    } else {
        console.error("Humidity data is null.");
        humElement.innerText = "N/A";
    }
});

// Update data Kualitas Udara (Gas)
dbRefAirQuality.on('value', (snapshot) => {
    const airQuality = snapshot.val();
    if (airQuality !== null) {
        airQualityElement.innerText = `${airQuality} ppm`;

        const currentTime = (new Date()).getTime();
        chartA.series[0].addPoint([currentTime, airQuality], true, chartA.series[0].data.length > 40);
    } else {
        console.error("Air Quality data is null.");
        airQualityElement.innerText = "N/A";
    }
});

// Update status Deteksi Api
dbRefFire.on('value', (snapshot) => {
    const fireDetected = snapshot.val();
    if (fireDetected !== null) {
        fireStatusElement.innerText = fireDetected ? "Fire Detected!" : "No Fire Detected";
        fireStatusElement.style.color = fireDetected ? "red" : "green";
    } else {
        console.error("Fire detection data is null.");
        fireStatusElement.innerText = "N/A";
    }
});

// Update LED status
dbRefLED.on('value', (snapshot) => {
    const ledStatus = snapshot.val();
    if (ledStatus !== null) {
        ledStatusElement.innerText = `LED Status: ${ledStatus}`;
    } else {
        console.error("LED data is null.");
        ledStatusElement.innerText = "LED Status: N/A";
    }
});

// Update Buzzer status
dbRefBuzzer.on('value', (snapshot) => {
    const buzzerStatus = snapshot.val();
    if (buzzerStatus !== null) {
        buzzerStatusElement.innerText = `Buzzer Status: ${buzzerStatus}`;
    } else {
        console.error("Buzzer data is null.");
        buzzerStatusElement.innerText = "Buzzer Status: N/A";
    }
});

// Update Motor status
dbRefMotor.on('value', (snapshot) => {
    const motorStatus = snapshot.val();
    if (motorStatus !== null) {
        motorStatusElement.innerText = `Motor Status: ${motorStatus}`;
    } else {
        console.error("Motor data is null.");
        motorStatusElement.innerText = "Motor Status: N/A";
    }
});

// Update Pesan
dbRefMessage.on('value', (snapshot) => {
    const message = snapshot.val();
    if (message !== null) {
        messageElement.innerText = `Message: ${message}`;
    } else {
        console.error("Message data is null.");
        messageElement.innerText = "Message: N/A";
    }
});

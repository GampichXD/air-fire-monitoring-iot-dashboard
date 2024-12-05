let chartTemperature, chartHumidity, chartAirQuality;

// Listen for Firebase database changes and update charts
const firebaseDataRef = firebase.database().ref(); // Referensi ke root

// Load charts on window load
window.addEventListener('load', () => {
    chartTemperature = createTemperatureChart(); // Chart for Temperature
    chartHumidity = createHumidityChart();       // Chart for Humidity
    chartAirQuality = createAirQualityChart();   // Chart for Air Quality

    // Listen for changes in Firebase database
    firebaseDataRef.on('value', (snapshot) => {
        const data = snapshot.val();
        if (data) {
            const temperature = data.Suhu;     // Reading from "/Suhu"
            const humidity = data.Kelembapan;  // Reading from "/Kelembapan"
            const airQuality = data.Gas;       // Reading from "/Gas"

            // Check if the data is valid
            if (temperature !== null && humidity !== null && airQuality !== null) {
                updateChartsWithData(temperature, humidity, airQuality);
            } else {
                console.error("Received null values from Firebase.");
            }
        } else {
            console.error("No data received from Firebase.");
        }
    });
});

// Create Temperature Chart
function createTemperatureChart() {
    return Highcharts.chart('chart-temperature', {
        chart: { type: 'spline' },
        title: { text: 'Temperature Monitoring' },
        time: {
            useUTC: false,
            timezoneOffset: -7 * 60 // Adjust to your local timezone (WIB: GMT+7)
        },
        xAxis: { type: 'datetime', dateTimeLabelFormats: { second: '%H:%M:%S' } },
        yAxis: { title: { text: 'Temperature (°C)' } },
        series: [{ name: 'Temperature', data: [] }],
        plotOptions: {
            spline: { animation: false, dataLabels: { enabled: true } }
        },
        credits: { enabled: false }
    });
}

// Create Humidity Chart
function createHumidityChart() {
    return Highcharts.chart('chart-humidity', {
        chart: { type: 'spline' },
        title: { text: 'Humidity Monitoring' },
        time: {
            useUTC: false,
            timezoneOffset: -7 * 60
        },
        xAxis: { type: 'datetime', dateTimeLabelFormats: { second: '%H:%M:%S' } },
        yAxis: { title: { text: 'Humidity (%)' } },
        series: [{ name: 'Humidity', data: [] }],
        plotOptions: {
            spline: { animation: false, dataLabels: { enabled: true } },
            series: { color: '#50b8b4' }
        },
        credits: { enabled: false }
    });
}

// Create Air Quality Chart
function createAirQualityChart() {
    return Highcharts.chart('chart-air-quality', {
        chart: { type: 'spline' },
        title: { text: 'Air Quality Monitoring' },
        time: {
            useUTC: false,
            timezoneOffset: -7 * 60
        },
        xAxis: { type: 'datetime', dateTimeLabelFormats: { second: '%H:%M:%S' } },
        yAxis: { title: { text: 'Air Quality (ppm)' } },
        series: [{ name: 'Air Quality', data: [] }],
        plotOptions: {
            spline: { animation: false, dataLabels: { enabled: true } },
            series: { color: '#f45b5b' }
        },
        credits: { enabled: false }
    });
}

// Function to update charts with data from sensors
function updateChartsWithData(temperatureData, humidityData, airQualityData) {
    const currentTime = (new Date()).getTime(); // Get current time

    // Update Temperature Chart
    chartTemperature.series[0].addPoint([currentTime, temperatureData], true, chartTemperature.series[0].data.length > 20);

    // Update Humidity Chart
    chartHumidity.series[0].addPoint([currentTime, humidityData], true, chartHumidity.series[0].data.length > 20);

    // Update Air Quality Chart
    chartAirQuality.series[0].addPoint([currentTime, airQualityData], true, chartAirQuality.series[0].data.length > 20);
}

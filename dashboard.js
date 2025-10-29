const firebaseConfig = {
  apiKey: "AIzaSyAyL52mHEL1-M8e_ZMEdUKlXjBMO3JcULI",
  authDomain: "sky-pulse-fed3e.firebaseapp.com",
  projectId: "sky-pulse-fed3e",
  storageBucket: "sky-pulse-fed3e.firebasestorage.app",
  messagingSenderId: "636853506255",
  appId: "1:636853506255:web:67ff97b4421d314845e7ba",
  measurementId: "G-7QYBJQ3XFP"
};

firebase.initializeApp({
  apiKey: "AIzaSyCK3dfVqo8oR-PlivnLSB1nWgf83o9nXDg",
  authDomain: "sky-pulse-69da2.firebaseapp.com",
  projectId: "sky-pulse-69da2",
  storageBucket: "sky-pulse-69da2.firebasestorage.app",
  messagingSenderId: "302343060501",
  appId: "1:302343060501:web:cf819777b3b77552ffa21c",
  measurementId: "G-V0M4KYZ5K7"
} );
const db = firebase.firestore();

function updateStats(stats) {
  document.getElementById('humidity').textContent = stats.humidity !== undefined ? stats.humidity + '%' : '--';
  document.getElementById('temperature').textContent = stats.temperature !== undefined ? stats.temperature + '°C' : '--';
  document.getElementById('co2').textContent = stats.co2 !== undefined ? stats.co2 + ' ppm' : '--';
  document.getElementById('airQuality').textContent = stats.airQuality !== undefined ? stats.airQuality : '--';
  document.getElementById('highTemp').textContent = stats.highTemp !== undefined ? stats.highTemp + '°C' : '--';
  document.getElementById('avgAirQuality').textContent = stats.avgAirQuality !== undefined ? stats.avgAirQuality : '--';
  document.getElementById('avgHumidity').textContent = stats.avgHumidity !== undefined ? stats.avgHumidity + '%' : '--';
  document.getElementById('avgCO2').textContent = stats.avgCO2 !== undefined ? stats.avgCO2 + ' ppm' : '--';
}

db.collection("SensorData")
  .orderBy("timestamp", "desc")
  .limit(1)
  .onSnapshot(snapshot => {
    snapshot.forEach(doc => {
      const stats = doc.data();
      updateStats(stats);
    });
  });


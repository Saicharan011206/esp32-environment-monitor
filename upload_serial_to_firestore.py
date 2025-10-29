import serial
import json
import time
import firebase_admin
from firebase_admin import credentials, firestore

SERIAL_PORT = 'COM7'  # Your ESP32 serial port
BAUDRATE = 115200
SERVICE_ACCOUNT_PATH = r"C:\Users\Admin\hackathoniith\sky-pulse-69da2-firebase-adminsdk-fbsvc-396133f97a.json"
COLLECTION = 'SensorData'

cred = credentials.Certificate(SERVICE_ACCOUNT_PATH)
firebase_admin.initialize_app(cred)
db = firestore.client()

ser = serial.Serial(SERIAL_PORT, BAUDRATE, timeout=2)

print(f"Listening on serial {SERIAL_PORT}")

while True:
    line = ser.readline().decode(errors='ignore').strip()
    if not line.startswith('{'):
        continue
    try:
        data = json.loads(line)
        data['timestamp'] = firestore.SERVER_TIMESTAMP
        db.collection(COLLECTION).add(data)
        print("Uploaded:", data)
    except Exception as e:
        print(f"Failed to upload line: {line} Error: {e}")
    time.sleep(0.05)

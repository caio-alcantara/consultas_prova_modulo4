import json
import paho.mqtt.client as mqtt
from flask import Flask, jsonify
from flask_socketio import SocketIO, emit

app = Flask(__name__)
socketio = SocketIO(app)

# Configurações MQTT
BROKER_MQTT = "broker.emqx.io"
BROKER_PORT = 1883
TOPIC = "emqx/esp32docaionoshare"

temp_value = None
hum_value = None
press_value = None
light_value = None

def on_connect(client, userdata, flags, rc):
    print("Connected with result code " + str(rc))
    client.subscribe(TOPIC)

def on_message(client, userdata, msg):
    global temp_value, hum_value, press_value, light_value
    try:
        data = json.loads(msg.payload.decode())
        temp_value = data.get('Temp')
        hum_value = data.get('Hum')
        press_value = data.get('Press')
        light_value = data.get('Light')

        print(f"Temperatura: {temp_value}, Umidade: {hum_value}, Pressão: {press_value}, Luz: {light_value}")

        socketio.emit('sensor_data', {'Temperatura': temp_value, 'Umidade': hum_value, 'Pressão': press_value, 'Luz': light_value})

    except json.JSONDecodeError:
        print("Falha ao decodificar JSON")

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
client.connect(BROKER_MQTT, BROKER_PORT, 60)

client.loop_start()

from flask import render_template

@app.route('/')
def index():
    return render_template('index.html')


@app.route('/sensor', methods=['GET'])
def get_sensor_data():
    if temp_value is None or hum_value is None:
        return jsonify({"error": "Dados do sensor ainda não recebidos"}), 404
    return jsonify({
        "Temperatura": temp_value,
        "Umidade": hum_value,
        "Pressao": press_value,
        "Luz": light_value
    })

if __name__ == '__main__':
    socketio.run(app, debug=True, host='0.0.0.0', port=5000)

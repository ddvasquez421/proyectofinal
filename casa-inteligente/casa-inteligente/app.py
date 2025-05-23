import streamlit as st
import paho.mqtt.client as mqtt
import threading
import time

# Variables globales para datos
temperatura_actual = 0
humedad_actual = 0

# MQTT broker público
MQTT_BROKER = "test.mosquitto.org"
MQTT_PORT = 1883

client = mqtt.Client()

# Callbacks MQTT
def on_connect(client, userdata, flags, rc):
    client.subscribe("casa/sala/temperatura")
    client.subscribe("casa/jardin/humedad")

def on_message(client, userdata, msg):
    global temperatura_actual, humedad_actual
    topic = msg.topic
    payload = msg.payload.decode()
    if topic == "casa/sala/temperatura":
        temperatura_actual = int(payload)
    elif topic == "casa/jardin/humedad":
        humedad_actual = int(payload)

# Conectar MQTT en hilo separado
def mqtt_loop():
    client.loop_forever()

client.on_connect = on_connect
client.on_message = on_message
client.connect(MQTT_BROKER, MQTT_PORT, 60)

mqtt_thread = threading.Thread(target=mqtt_loop)
mqtt_thread.daemon = True
mqtt_thread.start()

st.title("Casa Inteligente Multimodal")

page = st.sidebar.radio("Selecciona ambiente", ["Sala de estar", "Jardín"])

if page == "Sala de estar":
    st.header("Sala de estar")

    # Control luces
    luz = st.radio("Luz de la sala", ("Apagada", "Encendida"))
    if luz == "Encendida":
        client.publish("casa/sala/luz", "ON")
    else:
        client.publish("casa/sala/luz", "OFF")

    # Control temperatura
    temp_input = st.number_input("Ajusta la temperatura (°C)", min_value=10, max_value=40, value=temperatura_actual)
    if st.button("Enviar temperatura"):
        client.publish("casa/sala/temperatura_set", str(temp_input))

    st.write(f"Temperatura actual: {temperatura_actual} °C")

elif page == "Jardín":
    st.header("Jardín")

    # Botón para activar riego
    if st.button("Activar riego"):
        client.publish("casa/jardin/bomba", "ON")
        st.success("Riego activado por 5 segundos")
        time.sleep(5)
        client.publish("casa/jardin/bomba", "OFF")

    st.write(f"Humedad actual: {humedad_actual} %")

    # Entrada de voz (modo básico con text_input simulando reconocimiento de voz)
    st.write("Simulación de entrada de voz:")
    comando_voz = st.text_input("Di el comando para activar riego (ejemplo: 'regar')")

    if comando_voz.lower() == "regar":
        client.publish("casa/jardin/bomba", "ON")
        st.success("Riego activado por comando de voz")
        time.sleep(5)
        client.publish("casa/jardin/bomba", "OFF")

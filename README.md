# Casa Inteligente Multimodal

Proyecto de Casa Inteligente con dos ambientes y control multimodal usando Streamlit y MQTT.

## Descripción

- Simulación con dos ESP32 en WOKWI (Sala de estar y Jardín)
- Control de luces, temperatura y riego
- Interacción con botones, texto y entrada de voz simulada

## Instalación

1. Clonar repositorio
2. Instalar dependencias:
   ```
   pip install -r requirements.txt
   ```
3. Ejecutar la app:
   ```
   streamlit run app.py
   ```

## Simulación WOKWI

Abrir los archivos `.ino` en WOKWI, cargar en dos proyectos separados y ejecutar.

---

Broker MQTT público usado: test.mosquitto.org

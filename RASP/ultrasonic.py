#                                                           LABORATORIO 6
#eJERCICIO ULTRASONICO
### Detect the distance of an object using the HC-SR04 ultrasonic
###sensor in Raspberry Pi. Then send the value of the distance to the
###Tiva with serial communication using UART.

import RPi.GPIO as GPIO
import serial
import time

# Configuración de pines para el sensor ultrasónico
TRIG_PIN = 18  # Pin del trigger
ECHO_PIN = 17  # Pin del echo

# Configuración del puerto serie
ser = serial.Serial("/dev/ttyACM0", 9600)
ser.reset_input_buffer()

# Configuración de los pines GPIO en la Raspberry Pi
GPIO.setwarnings(False)
GPIO.setmode(GPIO.BCM)
GPIO.setup(TRIG_PIN, GPIO.OUT)
GPIO.setup(ECHO_PIN, GPIO.IN)

# Función para medir la distancia con el sensor ultrasónico
def medida_distancia():
    GPIO.output(TRIG_PIN, False)
    time.sleep(0.5)

    GPIO.output(TRIG_PIN, True)
    time.sleep(0.00001)
    GPIO.output(TRIG_PIN, False)

    while GPIO.input(ECHO_PIN) == 0:
        pulse_start = time.time()

    while GPIO.input(ECHO_PIN) == 1:
        pulse_end = time.time()

    pulse_duration = pulse_end - pulse_start

    distance = int(pulse_duration * 17150)  # Convertir a distancia en centímetros
    if distance >= 400:  # Limitar la distancia máxima a 400 cm
        distance = 400

    return distance

try:
    while True:
        distancia = medida_distancia()  # Medir la distancia
        print("Distancia:", distancia, "cm")

        # Enviar la distancia medida a la Tiva a través del puerto serie
        ser.write((str(distancia) + "\n").encode())

        time.sleep(0.3)

except KeyboardInterrupt:
    GPIO.cleanup()

import random
import tkinter as tk
import logging
from connector.mqtt import MQTTClient
from gui.control_panel import ControlPanel

# Logging config
logging.basicConfig(encoding='utf-8',
                    level=logging.DEBUG,
                    format='[%(asctime)s] [%(levelname)s] %(message)s',
                    datefmt='%H:%M:%S')

# MQTT config
MQTT_BROKER = "duijsens.dev"
MQTT_PORT = 1883
MQTT_KEEPALIVE = 60

# Generate random client ID
CLIENT_ID = random.randint(100000, 999999)


def main():
    """
    Main function of the client application. Initializes the MQTT client and the GUI.
    :return: None
    """
    logging.info("Starting Client Application..")

    # Initialize MQTT client
    mqtt_connector = MQTTClient(MQTT_BROKER, MQTT_PORT, MQTT_KEEPALIVE)
    mqtt_connector.connect()

    # Initialize GUI
    root = tk.Tk()
    control_panel = ControlPanel(root, CLIENT_ID, mqtt_connector)

    # Set handlers
    mqtt_connector.set_register_handler(control_panel.register_ball)
    mqtt_connector.set_deregister_handler(control_panel.deregister_ball)
    mqtt_connector.set_state_handler(control_panel.update_state)

    # Start MQTT client
    root.mainloop()
    

if __name__ == '__main__':
    main()

from machine import UART, Pin
from tb600b import TB600B_CO as TB600B
import utime

# # Initialize UART for co sensor
uart = UART(1, 9600, tx=11, rx=14)
uart.init(9600, bits=8, parity=None, stop=1)
sensor = TB600B(uart)


while True:
    # turn on the led 
    sensor.led_on()
    # change the sensor mode to qna
    if sensor.qna == False:
        sensor.changeMode()
    # get readings from the sensor
    print(sensor.read_all())
    # turn the led off
    sensor.led_off()
    # wait for the next cycle
    utime.sleep(5)
    sensor.led_on()

from machine import UART, Pin
from tb600b import TB600B_CO as TB600B
import utime
uart = UART(1, 9600, tx=11, rx=14)
uart.init(9600, bits=8, parity=None, stop=1)
sensor = TB600B(uart)
"""
Expected Behaviour:
    REPEAT FOREVER:
        Turn LED ON
        Samples all factors
        Turn LED off
        Wait 5 seconds
"""
while True:
    sensor.led_on()
    if sensor.qna == False:
        sensor.changeMode()
    print(sensor.read_all())
    sensor.led_off()
    utime.sleep(5)
    sensor.led_on()

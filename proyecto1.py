import spidev
import time

import random
from Adafruit_IO import Client, Feed

ADAFRUIT_IO_USERNAME = "Lop17065"
ADAFRUIT_IO_KEY = "aio_IlMq696GHJJJePrh34H3TXDdeK9f"
aio = Client(ADAFRUIT_IO_USERNAME,ADAFRUIT_IO_KEY)

spi = spidev.SpiDev()
spi.open(0,0)
spi.mode = 3
spi.max_speed_hz = 125000
try:
    while True :
        
        
       
            spi = spidev.SpiDev()
            spi.open(0,0)
            spi.mode = 3
            spi.max_speed_hz = 125000
            TEMPERATURA = spi.xfer([0x51])
            TE = int("".join(map(str, TEMPERATURA)))
            TE = (TE/255)*100 
            spi.close()
            
            spi = spidev.SpiDev()
            spi.open(0,0)
            spi.mode = 3
            spi.max_speed_hz = 125000
            LUZ = spi.xfer([0x50])
            L = int("".join(map(str, LUZ)))
            L = (L/255)*100
            
            spi.close()
            
            spi = spidev.SpiDev()
            spi.open(0,0)
            spi.mode = 3
            spi.max_speed_hz = 125000
            HUMEDAD = spi.xfer([0x52])
            H = int("".join(map(str, HUMEDAD)))
            H = (H/255)*100
            
            spi.close()
            
            
            spi = spidev.SpiDev()
            spi.open(0,0)
            spi.mode = 3
            spi.max_speed_hz = 125000
            LLUVIA = spi.xfer([0x53])
            LL = int("".join(map(str, LLUVIA)))
            LL = (LL/255)*100
            
            spi.close()
            
            
            spi = spidev.SpiDev()
            spi.open(0,0)
            spi.mode = 3
            spi.max_speed_hz = 125000
            NIVEL = spi.xfer([0x54])
            N = int("".join(map(str, NIVEL)))
            N = (N/255)*100
            
            spi.close()
            
            print(TE)
            print(L)
            print(H)
            print(LL)
            print(N)
            temperatura = aio.feeds('temperatura-interior-del-invernadero')
            aio.send_data(temperatura.key,L)
            luz = aio.feeds('intensidad-luz')
            aio.send_data(luz.key,H)
            humedad = aio.feeds('humedad-v')
            aio.send_data(humedad.key,LL)
            lluvia = aio.feeds('lluvia')
            aio.send_data(lluvia.key,N)
            nivel = aio.feeds('nivel-del-tanque-de-agua')
            aio.send_data(nivel.key,TE)

        
        
           

     
        
      
        
finally:
    spi.close()  

import network
import urequests
import machine
import ssd1306
import time
import dht
import framebuf

ssid = 'WIFISSID'
password = 'Password'

wlan = network.WLAN(network.STA_IF)
wlan.active(True)
wlan.connect(ssid, password)


i2c = machine.I2C(0,sda=machine.Pin(0), scl=machine.Pin(1))
display = ssd1306.SSD1306_I2C(128, 64, i2c)

d = dht.DHT11(machine.Pin(15))

button = machine.Pin(2, machine.Pin.IN, machine.Pin.PULL_DOWN)
buttonInput = machine.Pin(3, machine.Pin.IN, machine.Pin.PULL_DOWN)

max_wait = 10

buttonState = True
buttonInputState = 0

while True:
    precipitation = 0
    if button.value()==1 and buttonState is False:
        buttonState=True
        time.sleep(1)
    
    while buttonState:
        display.fill(0)
        
        if buttonInput.value()==1:
            buttonInputState = buttonInputState + 1
            time.sleep(1)
            
        if buttonInputState==1:
            loaded = False
            
        
            while loaded is False:
                
                for attempt in range(10):
                    try:
                        max_wait=10
                        while max_wait > 0:
                            if wlan.status() < 0 or wlan.status() >= 3:
                                break
                            max_wait -= 1
                            time.sleep(10)
                    except:
                        time.sleep(10)


                for attempt in range(10):
                    try:
                        if wlan.status()==3:
                            
                            status = wlan.ifconfig()
                            
                            url3 = "https://worldtimeapi.org/api/timezone/Location"
                            
                            for attem in range(10):
                                
                                try:
                                    request = urequests.get(url3)
                                    if request.status_code ==200:
                                        data = request.json()
                                        timeCurrent = data['datetime']

                                        request.close()
                                        loaded = True
                                        break
                                    
                                except:
                                    time.sleep(10)
                            
                            
                    
                    
                        display.fill(0)
                        display.invert(0)
                        display.poweron()
                        
                        
                        date = ""
                        netTime = ""
                        for index, char in enumerate(timeCurrent):
                            
                            if index<=9:
                                date = date + char
                            if index>10 and index<=15:
                                netTime = netTime + char
                        
                        numSlash=0
                        numString=""
                        totString = ""
                        day=""
                        month= ""
                        year =""
                        newDate =""
                        for char in date:
                            if char !='-':
                                if numSlash ==0:
                                    year = year+char
                        
                                
                                if numSlash==1:
                                    month = month + char
                                
                                if numSlash ==2:
                                    day = day + char                
                            else:
                                numSlash = numSlash +1
                        
                        newDate = day + '/' + month + '/' + year
                        display.text(newDate,28,0,1)
                        display.text(netTime,40,30,1)
                        display.show()
                        
                        timeDuration = 60
                        startingTime = time.ticks_ms()/1000
                                
                        while (time.ticks_ms()/1000)-startingTime<timeDuration:
                            if button.value()==1 and buttonState is True:
                                buttonState = False
                                display.fill(0)
                                display.poweroff()
                                time.sleep(1)
                                break
                            if buttonInput.value()==1:
                                display.fill(0)                    
                                buttonInputState=0
                                time.sleep(1)
                                break
                                    
                            time.sleep(0.1)
                    except:
                        time.sleep(10)
                        
                

        else:
            
            
            loaded = False
            
            while loaded is False:
                
                for attempt in range(10):
                    try:
                        max_wait=10
                        while max_wait > 0:
                            if wlan.status() < 0 or wlan.status() >= 3:
                                break
                            max_wait -= 1
                            time.sleep(10)
                        
                        if wlan.status() == 3:
                            
                            status = wlan.ifconfig()
                            
                            url = "https://api.open-meteo.com/v1/forecast?latitude=-latitude&longitude=longitude&current_weather=true&temperature_unit=celsius&windspeed_unit=kmh&precipitation_unit=mm&daily=temperature_2m_max,temperature_2m_min,precipitation_sum"
                            
                            for attem in range(10):
                                
                                try:
                                        
                                    request = urequests.get(url)
                                        
                                    if request.status_code ==200:
                                        data = request.json()
                                        temperature = data['current_weather']['temperature']
                                        windspeed = data['current_weather']['windspeed']
                                        winddirection = data['current_weather']['winddirection']
                                        tempLow = data['daily']['temperature_2m_min'][0]
                                        tempHigh = data['daily']['temperature_2m_max'][0]
                                        precipitationList=[]
                                        precipitation = data['daily']['precipitation_sum'][0]
                                        num=0
                                        while num<7:
                                            precipitationCur = data['daily']['precipitation_sum'][num]
                                            precipitationList.append(precipitationCur)
                                            num = num+1
                                        
                                        
                                
                                        loaded = True
                                    
                                        request.close()
                                        
                                        display.fill(0)
                                        display.invert(0)
                                            
                                        timeDuration = 2
                                        startingTime = time.ticks_ms()/1000
                                            
                                        while (time.ticks_ms()/1000)-startingTime<timeDuration:
                                            if button.value()==1 and buttonState is True:
                                                buttonState = False
                                                display.fill(0)
                                                display.poweroff()
                                                time.sleep(1)
                                                break
                                            if buttonInput.value()==1:
                                                break
                                                
                                            time.sleep(0.1)
                                            
                                        d.measure()
                                        display.poweron()
                                        
                                        apparentTemperature = 0.5 * (d.temperature() + 61.0 + (d.temperature()-68.0)/1.2) + (d.humidity() * 0.094)
                                        
                                        display.text(("C:" + str(d.temperature()) +"  F:" + str(round(apparentTemperature,1))),30,5,1)
                                        
                                        if(precipitation >= 1.5 and precipitation<7.5):
                                            with open('showers.pbm', 'rb') as f:
                                                f.readline()
                                                f.readline()
                                                f.readline()
                                                data = bytearray(f.read())
                                            fbuf = framebuf.FrameBuffer(data,20,24,framebuf.MONO_HLSB)
                                            display.blit(fbuf,-2,0)
                                            
                                        elif (precipitation >= 7.5):
                                            with open('heavy.pbm', 'rb') as f:
                                                f.readline()
                                                f.readline()
                                                f.readline()
                                                data = bytearray(f.read())
                                            fbuf = framebuf.FrameBuffer(data,20,21,framebuf.MONO_HLSB)
                                            display.blit(fbuf,-2,0)
                                        
                                        else:
                                            with open('sun.pbm', 'rb') as f:
                                                f.readline()
                                                f.readline()
                                                f.readline()
                                                data = bytearray(f.read())
                                            fbuf = framebuf.FrameBuffer(data,20,21,framebuf.MONO_HLSB)
                                            display.blit(fbuf,-2,-2)
                                        
                                        windStrength="Little"
                            
                                        if windspeed > 10 and windspeed < 30:
                                            with open('light.pbm', 'rb') as f:
                                                f.readline()
                                                f.readline()
                                                f.readline()
                                                data = bytearray(f.read())
                                            fbuf = framebuf.FrameBuffer(data,30,16,framebuf.MONO_HLSB)
                                            display.blit(fbuf,-2,35)
                                            windStrength = "Mild"
                                            
                                        elif windspeed > 30 and windspeed <40:
                                            with open('medium.pbm', 'rb') as f:
                                                f.readline()
                                                f.readline()
                                                f.readline()
                                                data = bytearray(f.read())
                                            fbuf = framebuf.FrameBuffer(data,30,19,framebuf.MONO_HLSB)
                                            display.blit(fbuf,-2,35)
                                            windStrength = "Moderate"
                                        elif windspeed>40:
                                            with open('heavyWind.pbm', 'rb') as f:
                                                f.readline()
                                                f.readline()
                                                f.readline()
                                                data = bytearray(f.read())
                                            fbuf = framebuf.FrameBuffer(data,30,24,framebuf.MONO_HLSB)
                                            display.blit(fbuf,-2,35)
                                            windStrength = "Strong"
                                        
                                        else:
                                            with open('sun.pbm', 'rb') as f:
                                                f.readline()
                                                f.readline()
                                                f.readline()
                                                data = bytearray(f.read())
                                            fbuf = framebuf.FrameBuffer(data,20,21,framebuf.MONO_HLSB)
                                            display.blit(fbuf,-2,35)
                                        
                                        if winddirection <90:
                                            display.text("N",0,30,1)
                                            
                                        elif winddirection < 180:
                                            display.text("E",0,30,1)
                                        elif winddirection < 180:
                                            display.text("S",0,30,1)
                                        else:
                                            display.text("W",0,25,1)
                                        
                                        display.text(("L:"+ str(tempLow) ),70,25,1)
                                        display.text(("H:"+ str(tempHigh)),16,25,1)
                                        
                                        display.text(("O:" + str(temperature)),40,40)
                                        
                                        n=0
                                        weeklyPrecipitation = ""
                                        
                                        while n<7:
                                            currentPrecipitation = precipitationList[n]
                                                                
                                            if currentPrecipitation >1.5:
                                                
                                                loaded = False
                                                
                                                while loaded is False:
                                                    max_wait=10
                                                    max_wait=10
                                                    while max_wait > 0:
                                                        if wlan.status() < 0 or wlan.status() >= 3:
                                                            break
                                                        max_wait -= 1
                                                        time.sleep(10)


                                                    if wlan.status()==3:
                                                        
                                                        status = wlan.ifconfig()
                                                        
                                                        url2="https://worldtimeapi.org/api/timezone/Location"
                                                        
                                                        for att in range(10):
                                                            
                                                            try:
                                                                request = urequests.get(url2)
                                                                
                                                                
                                                                
                                                                if request.status_code ==200:
                                                                    data = request.json()
                                                                    dayWeek = data['day_of_week'] +n
                                                                    
                                                                    request.close()
                                                                    
                                                                    loaded = True
                                                                    break
                                                                    
                                                            except:
                                                                time.sleep(10)
            
                                                            
                                                dayOfWeek = ""
                                                
                                                if dayWeek ==1:
                                                    dayOfWeek = "Mon"
                                                elif dayWeek == 2:
                                                    dayOfWeek="Tue"
                                                elif dayWeek == 3:
                                                    dayOfWeek="Wed"
                                                elif dayWeek == 4:
                                                    dayOfWeek="Thu"
                                                elif dayWeek == 5:
                                                    dayOfWeek="Fri"
                                                elif dayWeek == 6:
                                                    dayOfWeek="Sat"
                                                elif dayWeek == 7:
                                                    dayOfWeek="Sun"
                                                
                                                if dayOfWeek !="":
                                                    weeklyPrecipitation = "  Rain:" + dayOfWeek
                                                break
                                            n = n+1
                                        
                                        display.text((windStrength + weeklyPrecipitation), 0, 55)
                                        
                                        display.show()
                                            
                                        timeDuration = 60
                                        startingTime = time.ticks_ms()/1000
                                            
                                        while (time.ticks_ms()/1000)-startingTime<timeDuration:
                                            if button.value()==1 and buttonState is True:
                                                buttonState = False
                                                display.fill(0)
                                                display.poweroff()
                                                time.sleep(1)
                                                break
                                            if buttonInput.value()==1:
                                                break
                                                
                                                
                                            time.sleep(0.1)
                                        
                                except:
                                    time.sleep(10)                    
                            
                                
                        
                        
                    except:
                        time.sleep(10)

                




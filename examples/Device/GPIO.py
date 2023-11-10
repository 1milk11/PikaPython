import PikaStdLib
import PikaStdDevice

mem = PikaStdLib.MemChecker()
io1 = PikaStdDevice.GPIO()
time = PikaStdDevice.Time()

io1.setPin('PA8')
io1.setMode('out')
io1.enable()
io1.low()

print('hello pikascript')
print('mem.max :')
mem.max()
print('mem.now :')
mem.now()

while True:
    mem.now()
    io1.low()
    time.sleep_ms(500)
    io1.high()
    time.sleep_ms(500)
    

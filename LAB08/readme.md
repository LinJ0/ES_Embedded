LAB08
===

See [Lecture 08](http://www.nc.es.ncku.edu.tw/course/embedded/08) for more details.

---
## Circuit Schematic
![image](https://user-images.githubusercontent.com/99814021/184300036-aebafed4-fc6e-4b47-835c-eca22a2045e0.png)

## Serial Console
1. Install screen:
```
sudo apt-get install screen
```
2. Connect the USB to TTL module to the host computer and find the device name:
```
ls /dev/ttyUSB*
```
3. Connect to the serial console: (replace ttyUSBx with yours)
```
sudo screen /dev/ttyUSBx 115200
```

4. Press the reset button on your STM32F4DISCOVERY board. You should see the result. And then try type something in the console.

5. To leave screen, press `CTRL+A` and then press `k`.

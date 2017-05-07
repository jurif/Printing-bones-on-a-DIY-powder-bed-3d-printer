def printObject():
    ser = serial.Serial('COM3', 250000, timeout = 0.1) # Establish the connection on a specific port
    sleep(1)
    global yNozzles
    global psizeX
    global xmlLen
    for lNum in range(xmlLen):
        yPos = 0;
        while (yPos <90):
           
            hasData = False
            for yNR in range(128):
                boolCurrentState=False
                boolPreviousState=False
                xCount=0
                xRLE=[]
                yCoord=yNR*0.136+yPos
                for xPos in range(0,psizeX):
                    boolCurrentState = isPointInPolygon(xPos*0.136,yCoord)
                    if (boolCurrentState == boolPreviousState):
                        xCount+=1
                    else:
                        xRLE.append(xCount)
                        xCount=0  
                    boolPreviousState = boolCurrentState
                #xRLE.append(xCount)
                if(len(xRLE) >1):
                    hasData = True
                    
                    ser.write(str('N%s%s\r\n'%('{0:03d}'.format(yNR),xRLE)).encode('ascii'))
                    print(str('N%s%s'%('{0:03d}'.format(yNR),xRLE)))
                    sleep(0.01)
            
            if (hasData):
                ser.write(str('Y%s\r\n'%(yPos*100)).encode('ascii'))
                while True:
                    if(ser.readline()==b'A\r\n'):
                        break;
                ser.write(b'S\r\n')
                while True:
                    if(ser.readline()==b'A\r\n'):
                        break;
            yPos += 7.5
        ser.write(b'L\r\n')
        while True:
            if(ser.readline()==b'A\r\n'):
                break;
            
        layerUp();

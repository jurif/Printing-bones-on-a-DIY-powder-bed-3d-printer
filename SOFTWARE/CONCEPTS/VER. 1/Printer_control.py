import tkinter
import matplotlib.pyplot as plt
import matplotlib.path as mplPath
import xml.etree.ElementTree as ET
import numpy as np
import serial
from time import sleep
polygons= []
tempPolyC = []
tempPolyP = []
currLayer = 0
psizeX=735
psizeY=735
xNozzles=0
yNozzles=128
yOffset = 10
xOffset = 10
svgLoc = "" #THIS IS THE SLIC3R SVG FILE LOCATION
svgXml = ET.parse(svgLoc)
root = svgXml.getroot()
xmlLen = len(root);
print(xmlLen)
top = tkinter.Tk()

C = tkinter.Canvas(top, bg="white", height=500, width=500)




def changeLayer():
    polygons.clear();
    C.delete("all")
    for idx,entry in enumerate(root[currLayer]):
        pType = entry.get('{http://slic3r.org/namespaces/slic3r}type')
 
        for point in entry.get('points').split():
            tempPolyC.append([(float(point.split(',')[0])+yOffset)*5,(float(point.split(',')[1])+xOffset)*5])
            tempPolyP.append([(float(point.split(',')[0])+yOffset),(float(point.split(',')[1])+xOffset)])
        if (pType == "contour"):
            C.create_polygon(tempPolyC,fill = 'black')
        else:
            C.create_polygon(tempPolyC,fill = 'white')
        print(idx,tempPolyP)
        polygons.append(mplPath.Path(tempPolyP))
        tempPolyC.clear()
        tempPolyP.clear()
        C.pack()
    return;
changeLayer()
def motion(event):
    boolCP = False
    x, y = event.x, event.y
    for child in polygons:
        if (child.contains_point((x/5,y/5))):
            boolCP = not boolCP
    #print(boolCP)
def layerUp():
    global currLayer
    if (currLayer<xmlLen-1):
        currLayer +=1
        changeLayer()
        print(currLayer)
    return;
def layerDown():
    global currLayer
    if (currLayer>0):
        currLayer -=1
        changeLayer()
        print(currLayer)
    return;


def isPointInPolygon(x,y):
    boolCP=False
    for child in polygons:
        if (child.contains_point((x,y))):
            boolCP = not boolCP
    return boolCP;
def openCom():
    ser = serial.Serial('COM3', 250000) # Establish the connection on a specific port
    
    return;
def printLayer():
    ser = serial.Serial('COM3', 250000, timeout = 0) # Establish the connection on a specific port
    sleep(1)
    global yNozzles
    global psizeX
    for yNR in range(128):
        boolCurrentState=False
        boolPreviousState=False
        xCount=0
        xRLE=[]
        for xPos in range(0,psizeX):
            boolCurrentState = isPointInPolygon(xPos*0.136,yNR*0.136)
            if (boolCurrentState == boolPreviousState):
                xCount+=1
            else:
                xRLE.append(xCount)
                xCount=0  
            boolPreviousState = boolCurrentState
        #xRLE.append(xCount)
        if(len(xRLE) >1):
            sleep(0.02)
            ser.write(str('N%s%s\r\n'%('{0:03d}'.format(yNR),xRLE)).encode('ascii'))
            print(str('N%s%s'%('{0:03d}'.format(yNR),xRLE)))
    sleep(1)
    ser.write(b'S\r\n')
    sleep(1)
    print(ser.read_all())

    return;
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


def debug():
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
                    
                    
                    print(str('N%s%s'%('{0:03d}'.format(yNR),xRLE)))
                    sleep(0.01)
            
            if (hasData):
                print(str('Y%s\r\n'%(yPos*100)).encode('ascii'))           
                print(b'S\r\n')
            yPos += 7.5
        print(b'L\r\n')

            


bDown = tkinter.Button(top, text = "Layer Down",command=layerDown)
bUp = tkinter.Button(top, text = "Layer up", command=layerUp)
bPrintLayer = tkinter.Button(top, text = "Print layer", command=printLayer)
bOpenSerial = tkinter.Button(top, text= "Open Comm", command=openCom)
bDebug = tkinter.Button(top,text = 'Debug', command =debug)
bPrint = tkinter.Button(top,text = 'Print', command =printObject)
bUp.pack()
bDown.pack()
bPrintLayer.pack()
bOpenSerial.pack()
bDebug.pack()
bPrint.pack()


top.bind('<Motion>',motion)
top.mainloop()

## Application main code using Arduino ide and serial, the code is written in c language
##### The code contain two parts
****
    Hand movement

Follow hand movement by checking hand and wrest position and direction using sensors like distance and accelerometer

Collect these data and send it via wifi to the module that control the robotic arm to simulate the arm movement

    Robotic arm controlling

Using the data received from the module mounted on the hand this module control the arm to follow the hand movement

The module moves the arm by rotating several servo motors according to specific angles
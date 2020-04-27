Atari Joystick USB Duo Adapter

Connect 2 Atari joysticks with this device to your USB port.  Your system will recognize your Atari joysticks as standard gamepads ready for you to use with your emulators and games.  No drivers needed! In addition to standard single button Atari Joystick, this device also supports Flashback Atari joysticks that have 5 buttons.

Tested on Leanardo Pro Micro mini board. To get the USB device name recognized correctly, you need to modify your Arduino boards.txt file.  Open it and look for the leonardo section and add these 3 lines to the end of the section:  

leonardo.build.vid=0x8282
leonardo.build.pid=0x3201
leonardo.build.usb_product="Retro Joystick Adapter"

You will also need to download the generic Joystick.h library used in this project.

Good luck and see blog at https://www.icode.com/atari-joystick-usb-adapter-design/ for more details.




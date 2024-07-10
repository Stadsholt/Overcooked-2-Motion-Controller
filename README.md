# <p align="center">Overcooked! 2 Controller</p>

This project README was collaboratively created by my group member eske4, with contributions from myself in the form of images and videos.

This repository contains the code for a custom controller designed for the game "Overcooked! 2". The controller is split into three distinct parts, each serving a specific function:
<div align="center">
<img src="https://github.com/Stadsholt/Overcooked-2-Motion-Controller/blob/main/Images/AllComponents.jpg" width="600">
</div>

## <p align="center">Parts:</p>

1. **Knife with Buttons:**
   - Equipped with buttons for user interaction.
   - Enables unique actions triggered by doing a chopping motion with the knife.
   <div align="center">
     <img src="https://github.com/Stadsholt/Overcooked-2-Motion-Controller/blob/main/Images/knife.gif" width="500">
   </div>
2. **Plate Controller:**
   - Responsible for character movement and control within the game.
   - Utilises an accelerometer to track the tilt of the plate, adding an immersive dimension to the gameplay.
   <div align="center">
     <img src="https://github.com/Stadsholt/Overcooked-2-Motion-Controller/blob/main/Images/plate.gif" width="500">
   </div>
3. **Receiver:**
   - Serves as a central hub that receives information from both the knife and plate controllers.
   - Enables Bluetooth communication between the controller components and the gaming system.
   <div align="center">
     <img src="https://github.com/Stadsholt/Overcooked-2-Motion-Controller/blob/main/Images/Sugar.jpg" width="500">
   </div>
   
## <p align="center">Controllers showcase:</p>
Below the interaction between the controllers and their hardware can be seen:
<div align="center">
<img src="https://github.com/Stadsholt/Overcooked-2-Motion-Controller/blob/main/Images/Info.jpg" width="500">
</div>

## <p align="center">Code Organization:</p>

### Receiver:
   - Code Location: [Receiver/](Receiver/)
   - Description: Code that takes input from the knife and plate controllers.

### Plate Controller:
   - Code Location: [Plate_Wireless/](Plate_Wireless/)
   - Description: Sends thumb stick x-y coordinates through the accelerometer to the receiver. Triggers vibration if tilted too much, signalling that in-game items may be dropped.
### Knife Controller:
   - Code Location: [Knife_Wireless/](Knife_Wireless/)
   - Description: Sends input from the knife controller to the receiver. Includes a pressure sensor for in-game grabbing, two buttons for interactions, and an accelerometer to detect shaking for specific interactions.

## <p align="center">Components and libraries Utilized:</p>

- **Vibration Module:** Enhances user feedback by incorporating vibration effects into the gaming experience.
- **Bluetooth Module:** Facilitates wireless communication between the controller components and the gaming system.
- **Accelerometer:** Enables precise tracking of plate tilt, contributing to a more dynamic and engaging gameplay.
- **Buttons:** Used for initiating various in-game actions.
- **Pressure sensor:** Used for grab actions.
- **[xInput Library](https://github.com/dmadison/ArduinoXInput):** Employed to emulate an Xbox controller, ensuring compatibility with the game "Overcooked! 2".

You can explore the codebase to gain insights into each component's intricacies and interactions.

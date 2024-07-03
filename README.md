# 🛠️ Component Search Automation System

## 🌟 General Description
Our system automates the search for components in a cabinet for technicians. The system allows technicians to enter the SAP code of the component via a keyboard, and in real-time, the corresponding address of the component is displayed on an LCD screen from an Excel file that contains all the SAP addresses of the components. This eliminates the need for manual searches and reduces the risk of errors.

![Image1](https://github.com/mohamedaminenajjar/smart-components-exchange/assets/152156763/158d3beb-2577-42db-8324-6064de492f61)

## 📁 Project Structure
The project is divided into several main parts:

### 1. 📋 Input Handling
Technicians enter the SAP code of the component using a keyboard.

### 2. 📡 Real-Time Address Display
The system fetches the corresponding address of the component from an Excel file and displays it on an LCD screen in real-time.

### 3. 📊 Data Management
The Excel file contains all the addresses of SAP components, ensuring accurate and up-to-date information is always available.

### 4. 🔍📉 Synoptic Diagram

![Capture d'écran 2024-07-03 153034](https://github.com/mohamedaminenajjar/smart-components-exchange/assets/152156763/b58eda13-67ab-4724-8528-bb5c63681a32)

## 📂 Code Files

### `main.c`
This file contains the main logic of the system, including the initialization of the hardware components and the main loop that handles user input and displays the corresponding address on the LCD screen.

### `lcd.c`
This file contains the functions for interfacing with the LCD screen. It includes functions for initializing the LCD, displaying text, and clearing the screen.

### `data.csv`
This file contains the SAP codes and their corresponding addresses. The system reads this file to find and display the correct address based on the input SAP code.

### `fatfs_sd.c`
This file contains the implementation for interfacing with the SD card using the FAT file system. It includes functions for SPI communication, power management, and read/write operations on the SD card. Below are some key functions defined in this file:

- `SELECT` and `DESELECT`: Functions to control the chip select pin for SPI communication.
- `SPI_TxByte`, `SPI_RxByte`, and related functions: Functions for SPI data transmission and reception.
- `SD_PowerOn` and `SD_PowerOff`: Functions to manage the power state of the SD card.
- `SD_SendCmd`: Function to send commands to the SD card.
- `SD_disk_initialize`: Function to initialize the SD card.
- `SD_disk_read` and `SD_disk_write`: Functions to read from and write to the SD card.

## 📝 How to Use
1. **Set Up the Hardware:**
   - Connect the keyboard to the input port.
   - Connect the LCD screen to the display port.
   - Ensure the STM32 microcontroller is properly powered and connected.

2. **Load the Data:**
   - Ensure the `data.csv` file is correctly formatted and contains all necessary SAP codes and addresses.
   - Load the `data.csv` file onto the system.

3. **Compile and Upload the Code:**
   - Compile `main.c` and `lcd.c` using your preferred IDE or compiler.
   - Upload the compiled code to the STM32 microcontroller.

4. **Operation:**
   - Enter the SAP code using the keyboard.
   - The system will display the corresponding address on the LCD screen in real-time.

## 📷 Project Overview Image
![Project Overview](path/to/your/image.png)

## 🛠️ Hardware and Software Requirements
- **Hardware:**
  - [STM32F407Discovery board](https://www.st.com/en/evaluation-tools/stm32f4discovery.html)
  - [Keyboard](https://www.didactico.tn/produit/clavier-16-touches-77x70x08mm/)
  - [LCD screen](https://www.didactico.tn/produit/ecran-lcd-1602-avec-module-i2c/)
  - [SD module](https://2btrading.tn/arduino/966-micro-sd-module-pour-arduino.html)

- **Software:**
  - STM32CubeIDE or any compatible IDE

## 🧩 Dependencies
- Ensure all necessary libraries for STM32 and LCD interfacing are included in the project.

## 🔧 Configuration
- Adjust the configurations in `main.c` and `lcd.c` as per your specific hardware setup and requirements.

## 🖥️ Running the Application
- Power on the system and ensure all connections are secure.
- Follow the operation steps to start using the component search automation system.

## Contact 📧

For any questions or inquiries, please contact:
- Email: [mohamedamine.najjar@isimg.tn](https://mail.google.com/mail/u/0/?fs=1&tf=cm&source=mailto&to=mohamedamine.najjar@isimg.tn)
- LinkedIn: [Mohamed Amine Najjar](https://www.linkedin.com/in/mohamed-amine-najjar-2808a726b/)

---

Happy coding! 😊


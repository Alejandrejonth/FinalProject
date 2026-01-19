# 🕒 SmartAttendance: AI Vision & RFID Attendance System
> **Final Project - Interface Development**

SmartAttendance is a robust personnel management system developed in **C++ (Qt)** that integrates **RFID** hardware, **OpenCV** image processing, **MySQL** databases, and intelligent notifications via a **Telegram Bot**.

---

## 📸 Project Overview
The system automates check-in/check-out records. Unlike traditional systems, it captures a real-time photo of the user when they scan their tag, ensuring record authenticity and sending an instant notification to the user's mobile device.



## 🛠️ Tech Stack & Libraries

* **Language & Framework:** C++ / Qt 5.x (Core, Widgets, Network, Sql, SerialPort).
* **Computer Vision:** OpenCV 4.x (Video capture, `imwrite` for evidence, frame processing).
* **Database:** MySQL (User management, privileges, and attendance logs).
* **Hardware:** Arduino/ESP32 with RFID reader (Serial communication at 115200 baud).
* **API:** Telegram Bot API (Asynchronous integration for mobile communication).

## 🚀 Key Features

### 1. User & Privilege Management
The system distinguishes between **Users (U)** and **Administrators (A)**:
* **User Mode:** Upon detecting a valid TAG, it records attendance and captures a photo.
* **Admin Mode:** When an admin TAG is scanned, the Qt UI dynamically expands, revealing hidden forms to register new employees.

### 2. Real-Time Computer Vision
Using a `QTimer` set at 50ms, the application processes the camera stream:
* Converts `cv::Mat` format to `QImage` for UI rendering.
* Automatic photo evidence capture during check-in (`cv::imwrite`).

### 3. Smart Chatbot (Telegram)
The Telegram bot allows for **Self-Registration**:
* If a new user messages the bot, it requests their TAG code.
* Upon validating the code (using a `#` suffix), the system automatically links the Telegram `ChatID` with the MySQL database without manual admin intervention.

## 🔌 Software Architecture

* **Serial Communication:** Implements `recepcionSerialAsincrona()` to handle incoming JSON data buffers from the Arduino.
* **Database Management:** Uses Prepared SQL Queries (`QSqlQuery::prepare`) to prevent SQL injection and manage timestamp storage.
* **Concurrency:** Leverages signals and slots to keep the UI responsive while processing images and network requests.

---

## ⚙️ Installation & Requirements

1.  **Database:** Create a MySQL database named `2021B2`.
2.  **Qt Configuration:** * Ensure `QT += sql network serialport` is included in your `.pro` file.
    * Link OpenCV libraries correctly.
3.  **Hardware:** Connect the microcontroller. The system automatically detects ports with Arduino (UNO/MEGA) Vendor IDs.

## 📊 Database Schema
* **`usuarios` Table:** `nombre`, `privilegios`, `codigoTAG`, `chatID`, `imagen`.
* **`asistenciasMejorado` Table:** `nombre`, `timestamp`, `codigoTAG`, `imagen`.

---
**Developed by:** Jonathan Alejandre  
**Academic Institution:** Universidad de Guadalajara
**Year:** 2021

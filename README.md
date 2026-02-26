## **Heart Rate Monitor**

The Heart Rate Monitor is a real-time medical system that captures human heart activity through **Photoplethysmography**, measuring heart activity through changes in infrared light absorption and allowing for non-invasive analysis of the human body.

This project focuses on using an EFM8LB1 Lazer Bee microcontroller connected to a finger-clip to measure real-time heart rate in C and amplifying this output for BPM and heart pulse visualization using a GUI.


# Features

**Signal Processing & Hardware**

* **Custom Optical Transducer:** Infrared LED and phototransistor housed in a clothespin and heat shrink to detect blood volume displacement in fingers.
* **Signal Conditioning:** Cascaded op-amps for high-gain amplification of input signal, becoming visible to human eye.
* **Schmitt Trigger (Hysteresis Comparator)** Converting noisy pulses into clean digital square waves, eliminating noise.
* **Visual & Audio Feedback:** A buzzer and LED synchronized to every detected heartbeat for real-time, tangible monitoring.

**Embedded Logic**

* **16-Sample Decimator:** Implemented a rolling average of 16 recorded samples to smooth out input noise and provide a stable BPM reading.
* **Low-Latency Interrupts:** Used timer interrupts to capture pulse intervals while recording rising and falling edges of signals with precision.
* **LCD Integration:** Real-time local display of instantaneous BPM readings and averaged BPM readings from 16-sample decimator.

**GUI & Visualization**

* **Dynamic Waveform Plotting:** Real-time "heartbeat pulse" visualization using UART from EFM8 MCU.
* **Selectable Display:** Ability to toggle between average BPM readings and heartbeat pulse with the push of a button integrated into GUI.
* **Heart Beat Animation:** Pulsing heart beat animation integrated into GUI, matching input from finger clip.

# Demo

https://github.com/user-attachments/assets/78f7c3ea-6402-4b14-a268-56e3dbcc556e

![overhead of circuit2](https://github.com/user-attachments/assets/bd63b28b-255d-4e07-a071-10e8c213952c)

![clip display](https://github.com/user-attachments/assets/32c7ad5f-7207-4136-a00b-01a7da08ade1)

![rhbgraph](https://github.com/user-attachments/assets/562cefe5-d02b-44b1-a116-ba98f92f9ea8)

![hb graph](https://github.com/user-attachments/assets/52b7f0d6-cbf2-44d6-8a24-2b71f36bef13)

# Tech Stack

| Component | Technology |
| --- | --- |
| **Microcontroller** | EFM8LB1 "Laser Bee" - 72 MHz) |
| **Backend/Embedded Language** | C |
| **Frontend/GUI** | Python (PySerial, Tkinter, Matplotlib) |
| **Analog Components** | LM358 Op-Amp, IR/Phototransistor Pair, Schmitt Trigger/Hyterisis Comparator|
| **Physical Peripherals** | LCD, Buzzer, LED|


# Sequence of Operation

1. **Initial Pulse Reading:** The IR LED shines through the finger. As the heart pumps, blood density changes in the finger and varies the light hitting the phototransistor.
2. **Amplification:** A multi-stage op-amp circuit brings the millivolt readings up to logic level range.
3. **Digitization:** The **Schmitt Trigger**  adds hysteresis and turns the signal into a square wave (either 0V or 5V), ensuring that a significant pulse triggers the EFM8 interrupt for period readings.
4. **Processing:** The EFM8 measures the time between pulses. It calculates BPM = 60/T and applies the **16-sample decimation filter** to prevent stabilize readings.

# Why I Built This

I developed this heart rate monitor to learn:

* **Signal Amplification:** Manipulating a small signal into a readable signal using theory taught in class, allowing for data manipulation into more useful forms.
* **Data Filtering:** Implementing decimation filters in C to provide "human-readable" data and improving accuracy for clinical applications.
* **Frontend - Backend Configuration:** Feeding data from embedded C code in the MCU to a Python script, allowing for data visualization.
* **User Interfacing:** Creating a user friendly interface with features that would actually be helpful to a user and improve experience.
* **Interaction with Human Anatomy Using Wearable Electronics:** How to appropriately use electronics to analyze human bodily functions using Biomedical engineering applications.

# Future Improvements

* **SpO2 Integration:** Adding a second Red LED to calculate blood oxygen saturation levels.
* **Wireless Connectivity:** Replacing the USB-Serial link with a Bluetooth module for mobile monitoring.

# Contact
If you're a recruiter, developer, or game enthusiast interested in this project, feel free to reach out via LinkedIn or email (listed on my GitHub profile).


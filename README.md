## **Heart Rate Monitor**

The Heart Rate Monitor is a real-time medical system that captures human heart activity through **Photoplethysmography**, measuring heart activity through changes in infrared light absorption and allowing for non-invasive analysis of the human body.

This project focuses on using an EFM8LB1 Lazer Bee microcontroller connected to a finger-clip to measure real-time heart rate in C and amplifying this output for BPM and heart pulse visualization using a GUI.


### **Features**

**Signal Processing & Hardware**

* **Custom Optical Transducer:** Infrared LED and phototransistor housed in a clothespin and heat shrink to detect blood volume displacement in fingers.
* **Signal Conditioning:** Cascaded op-amps for high-gain amplification of input signal, becoming visible to human eye.
* **Schmitt Trigger (Hysteresis Comparator)** Converting noisy pulses into clean digital square waves, eliminating noise.
* **Visual & Audio Feedback:** A buzzer and LED synchronized to every detected heartbeat for real-time, tangible monitoring.

**Embedded Logic**

* **16-Sample Decimator:** Implements a rolling average to smooth out noise and provide a stable Beats Per Minute (BPM) reading.
* **Low-Latency Interrupts:** Uses the EFM8’s Configurable Logic Units (CLU) or Timer interrupts to capture pulse intervals with microsecond precision.
* **LCD Integration:** Real-time local display of instantaneous pulse status and calculated averages.

#### **GUI & Visualization (Python)**

* **Dynamic Waveform Plotting:** Real-time "heartbeat pulse" visualization using Matplotlib or PyQtGraph.
* **Data Logging:** Tracks BPM averages over time to identify trends or resting heart rate stability.
* **Serial Communication:** Robust UART protocol to stream processed data from the MCU to the PC.

### **Tech Stack**

| Component | Technology |
| --- | --- |
| **Microcontroller** | EFM8LB1 "Laser Bee" (8051-based, 72 MHz) |
| **Embedded Language** | C (Silicon Labs Simplicity Studio / Keil) |
| **Frontend/GUI** | Python (PySerial, Tkinter/PyQt, Matplotlib) |
| **Analog Components** | Op-Amps (LM358/TL072), IR Pair, Schmitt Trigger |
| **Peripherals** | 16x2 LCD, Piezo Buzzer, UART-to-USB Bridge |


### **Theory of Operation**

1. **Transduction:** The IR LED shines through the finger. As the heart pumps, blood density changes, varying the light hitting the phototransistor.
2. **Amplification:** The signal is incredibly faint. A multi-stage Op-Amp circuit brings the millivolt-level fluctuations up to a logic-level range.
3. **Digitization:** The **Schmitt Trigger** prevents "double-counting" by adding hysteresis, ensuring only a significant pulse triggers the EFM8 interrupt.
4. **Processing:** The EFM8 measures the time between pulses ($T$). It calculates $BPM = 60/T$ and applies a **16-sample decimation filter** to prevent erratic readings.

---

### **Why I Built This**

I developed this monitor to master the challenges of **Analog-to-Digital interfacing**:

* **Noise Mitigation:** Learning to filter out 60Hz hum and motion artifacts from sensitive analog sensors.
* **Data Smoothing:** Implementing decimation filters in C to provide "human-readable" data without sacrificing responsiveness.
* **Cross-Language Integration:** Coordinating real-time C-based hardware control with a flexible, high-level Python user interface.

---

### **Future Improvements**

* **SpO2 Integration:** Adding a second Red LED to calculate blood oxygen saturation levels.
* **Wireless Connectivity:** Replacing the USB-Serial link with a Bluetooth (HC-05) module for mobile monitoring.
* **Peak Detection Algorithm:** Moving the pulse detection from hardware (Schmitt Trigger) to software (ADC + Digital Signal Processing) for more granular control.


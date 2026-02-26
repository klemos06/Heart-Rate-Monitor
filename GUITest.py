import tkinter as tk
from matplotlib.figure import Figure
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import collections
import serial
import numpy as np

# desired port 
ser = serial.Serial('COM15', 115200, timeout=0.01)

X_SIZE = 50
wave_data = collections.deque([0]*X_SIZE, maxlen=X_SIZE)
bpm_data = collections.deque([0]*X_SIZE, maxlen=X_SIZE)

current_mode = "LIVE" 

def toggle_view():
    global current_mode
    if current_mode == "LIVE":
        current_mode = "BPM"
        btn_toggle.config(text="Heartbeat Average")
        lbl_status.config(text="Current View: Average BPM")
    else:
        current_mode = "LIVE"
        btn_toggle.config(text="Show Average BPM")
        lbl_status.config(text="Current View: Live Digital Heartbeat")

# -gui main layout
root = tk.Tk()
root.title("Heart Rate Monitor")
root.geometry("600x550")
root.configure(bg="#2E2E2E")

top_frame = tk.Frame(root, bg="#2E2E2E")
top_frame.pack(fill=tk.X, pady=10)

# heart beating animation
lbl_heart = tk.Label(top_frame, text="❤", font=("Helvetica", 60), fg="#404040", bg="#2E2E2E")
lbl_heart.pack(side=tk.LEFT, padx=30)

control_frame = tk.Frame(top_frame, bg="#2E2E2E")
control_frame.pack(side=tk.LEFT, fill=tk.X, expand=True)

lbl_status = tk.Label(control_frame, text="Current View: Live Heartbeat", font=("Arial", 14), fg="white", bg="#2E2E2E")
lbl_status.pack(pady=5)

btn_toggle = tk.Button(control_frame, text="Show Average BPM", command=toggle_view, font=("Arial", 12), bg="teal", fg="white")
btn_toggle.pack()

# matplot lib init
fig = Figure(figsize=(6, 4), dpi=100)
fig.patch.set_facecolor('#2E2E2E')
ax = fig.add_subplot(111)
ax.set_facecolor('#1E1E1E')
line, = ax.plot([], [], lw=2, color='red')

# tkinter gui stuff
canvas = FigureCanvasTkAgg(fig, master=root)
canvas_widget = canvas.get_tk_widget()
canvas_widget.pack(fill=tk.BOTH, expand=True)

# loop logic
def update_data():
    lines_read = 0
    new_data_received = False 
    
    # read serial buffer for transmitted info 
    while ser.in_waiting > 0 and lines_read < 20: 
        try:
            raw_line = ser.readline().decode('utf-8').strip()
            if not raw_line: continue
            
            # debug feature - prints the stuff coming from MCU 
            print(f"Debug Mode - Data from MCU:{raw_line}") 
            
            if raw_line.startswith("HB:"):
                val = int(raw_line[3:]) # checks for HB --> knows its hb info
                wave_data.append(val)
                new_data_received = True
                
                # if the hb is read as 1 from MCU --> turn the heart icon to red
                lbl_heart.config(fg="red" if val == 1 else "#404040")
                
            elif raw_line.startswith("Period:"):
                period = float(raw_line[7:]) # checks for Period --> if yes this is period info
                if period > 0:
                    bpm = 60.0 / period
                    if 40 <= bpm <= 150: # filters outliers (oddly far off readings)
                        bpm_data.append(bpm)
                        new_data_received = True
                        
        except Exception as e:
             lines_read += 1
        
    # redraw if something has changed since last time
    if new_data_received:
        ax.clear()
        ax.set_facecolor('#1E1E1E')
        ax.grid(color='gray', linestyle='--', alpha=0.3)
        ax.tick_params(colors='white')
        
        if current_mode == "LIVE":
            ax.plot(list(wave_data), color='red', lw=2) # visuals for hb
            ax.set_ylim(-0.2, 1.2)
            ax.set_title("Live Heartbeat", color="white")
        else:
            if len(bpm_data) > 0:
                avg_bpm = np.mean(list(bpm_data)[-16:]) # visuals for bpm
                ax.plot(list(bpm_data), color='cyan', lw=2)
                ax.set_title(f"BPM (Avg: {avg_bpm:.1f})", color="white")
            ax.set_ylim(40, 160)

        canvas.draw()
    
    # next check in 20 ms to check data 
    root.after(20, update_data)

# begin loop
root.after(100, update_data)
root.mainloop()
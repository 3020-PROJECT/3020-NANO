# 🔌 3020-NANO

A lightweight **Arduino Nano** firmware that listens for **serial commands** from the TL-MR3020 (as part of the *WhyNot* hardware hacking series).  
It allows remote execution of I/O operations such as reading/writing digital or analog pins and controlling an LCD display.

---

## ✨ Features

- 🔌 **Serial command protocol** for controlling the Arduino Nano  
- 🧠 **Digital & analog I/O support** (`read`, `write`, `aread`, `awrite`, `pinmode`)  
- 💬 **LCD support** using I²C (address configurable via commands)  
- 🧱 **Simple text-based protocol** — perfect for integration with routers or scripts  
- 💡 **Human-readable responses** (`OK`, `KO`, with optional messages)

---

## 📡 Protocol Overview

Commands are sent as **newline-terminated strings (`\n`)** over serial (9600 baud).  
Each command is space-separated and can include parameters.

### Supported Commands

| Command | Description |
|----------|--------------|
| `exec` | Execute higher-level operations (e.g., LCD control) |
| `read <pin>` | Read digital value from a pin |
| `write <pin> <low/high>` | Write digital value |
| `aread <apin>` | Read analog value |
| `awrite <apin> <value>` | Write analog value |
| `pinmode <pin> <input/output>` | Configure a pin mode |

---

## 💬 Example Usage

Using any serial terminal (e.g., `minicom`, `screen`, or via your TL-MR3020):

```
pinmode 13 output
write 13 high
read 13
```

Response:
```
OK [pinmode set]
OK
OK [Value = 1]
```

---

## 🧱 Hardware Setup

- **Board:** Arduino Nano  
- **LCD:** I²C LCD (optional, address configurable via commands)
- **Connection:** Serial link with TL-MR3020 running custom firmware from the *WhyNot* series, or any serial-capable host.
- **Baud rate:** `9600`

---

## ⚙️ Building & Uploading

Using **PlatformIO** or the **Arduino IDE**:

```bash
# PlatformIO
pio run
pio run -t upload

# Or Arduino IDE
Select board: Arduino Nano
Select port: /dev/ttyUSB0
Upload
```

---

## 🔭 Future Plans

- 🧩 Add full LCD command support (`exec setcursor`, `exec print`, etc.)  
- 🧠 Introduce error codes for better debugging
- 📡 Implement advanced I/O features (PWM, interrupts)

---

## 🤝 Contributing

Pull requests and suggestions are welcome!
Please open issues for bugs or feature requests.

---

## 🧩 License
MIT License — free to use, modify, and share.

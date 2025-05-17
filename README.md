# c-web-sample

A minimal web sample project using a **C backend** (raw sockets, no libraries) and a **Vanilla TypeScript frontend**.
Designed to demonstrate how to serve static files and handle API routes with C, while using TypeScript for the client.

---

## 📁 Project Structure
```csharp
c-web-sample/
├── backend/        # C HTTP server (raw sockets)
│   └── main.c
├── frontend/       # TypeScript frontend
│   ├── src/
│   │   └──  main.ts
│   ├── static/  
│   │   └── index.html
│   └── tsconfig.json
├── static/         # Compiled HTML/JS output
├── Makefile        # Build & run scripts
└── README.md
```

---

## ✅ How to Run

1. Install TypeScript

```bash
cd frontend
npm install typescript --save-dev 
```

2. Build frontend

```bash
make build 
```

3. Run server

```bash
make run
```

---

## 🌐 Accessing the Server

### If you're on WSL (Windows Subsystem for Linux)
The server will automatically detect and print your current WSL IP:

```nginx
Server started on http://172.xx.xx.xx:8080
```

→ Open this IP + port in your Windows browser.

### If you're on native Linux or other environments
You will see output like:

```nginx
Server started on http://127.0.0.1:8080
Server started on http://192.168.x.x:8080
```

→ Access using either address.

---

## 🧠 Features
* `GET /api/hello` returns a plain text response from C

* Serves static files (`index.html`, `main.js`, etc.)

* Vanilla TypeScript compiled to JS and served by the C backend

* Fully manual HTTP handling in C using raw sockets (no external libraries)

* WSL-aware: automatically detects and displays proper access IP

---

## 🛠️ Makefile Commands
* `make build`: Compile TypeScript and copy HTML to static/

* `make run`: Compile and run the C server

* `make clear`: Remove generated JS, HTML, and server binary

---

## ⚠️ Notes
* Only basic GET requests are supported (no `POST`, `PUT`, etc.)

* Static file MIME type detection is minimal (`.html`, `.js`)

* If you're using WSL and can't connect, run `hostname -I` to find your IP
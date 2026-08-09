<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>Absorbance Colorimeter</title>
<link rel="preconnect" href="https://fonts.googleapis.com">
<link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
<link href="https://fonts.googleapis.com/css2?family=Plus+Jakarta+Sans:wght@400;500;600;700;800&display=swap" rel="stylesheet">
<style>
  :root{
    --navy:#141b2e; --cream:#edebe6; --red:#c8102e; --red-dark:#9e0c24;
    --white:#fff; --charcoal:#2a2a2a; --mid-grey:#6b7280; --border:#d6d2cc;
    --green:#0f9d58; --amber:#e8a020; --font:'Plus Jakarta Sans','Segoe UI',Arial,sans-serif;
  }
  *{box-sizing:border-box}
  body{margin:0;background:var(--cream);color:var(--charcoal);font-family:var(--font)}
  header{background:var(--navy);color:#fff;padding:1em 1.4em;display:flex;align-items:center;justify-content:space-between;gap:1em}
  header .brand{font-weight:800;font-size:1.1rem;display:flex;flex-direction:column;line-height:1.2}
  header .brand small{font-weight:500;font-size:.72rem;opacity:.7}
  header .status-chip{font-size:.75rem;font-weight:700;padding:.4em .8em;border-radius:10px;
    background:rgba(255,255,255,.14)}
  header .status-chip.on{background:#dff3e6;color:#0b7a41}

  .wrap{max-width:1600px;margin:0 auto;padding:2em 1.4em 3em}
  h1{color:var(--navy);margin:0 0 .1em}
  .sub{color:var(--mid-grey);margin:0 0 1.6em}
  .eyebrow{color:var(--red);font-weight:800;font-size:.75rem;text-transform:uppercase;letter-spacing:.06em;margin:1.4em 0 .6em}

  .layout{display:flex;gap:1.2em;align-items:stretch}
  .column{background:var(--white);border:1px solid var(--border);border-radius:14px;
    padding:1.4em 1.5em;flex:1;display:flex;flex-direction:column;min-height:760px}
  .column h2{color:var(--navy);margin:0 0 .2em;font-size:1.05rem}

  .control-grid{display:grid;grid-template-columns:repeat(3,1fr);gap:.7em}
  .wavelength-grid{display:grid;grid-template-columns:repeat(3,1fr);gap:.7em;margin-top:.4em}

  button.act{font-family:var(--font);font-weight:700;font-size:.9rem;width:100%;
    padding:.75em .5em;background:var(--red);color:#fff;border:none;border-radius:9px;cursor:pointer}
  button.act:hover{background:var(--red-dark)}
  button.act.secondary{background:var(--navy)}
  button.act.secondary:hover{background:#20293f}

  .wl-btn{display:flex;flex-direction:column;align-items:center;gap:.35em;
    background:var(--white);border:1px solid var(--border);border-radius:10px;
    padding:.7em .3em;cursor:pointer;font-family:var(--font);font-weight:700;
    font-size:.82rem;color:var(--navy)}
  .wl-btn:hover{background:#f4f2ee}
  .color-box{width:24px;height:24px;border-radius:6px;border:1px solid rgba(0,0,0,.15)}

  input[type="text"]{width:100%;font-size:.95rem;padding:.6em .8em;margin:1em 0;
    border:1px solid var(--border);border-radius:8px;font-family:var(--font)}

  .scroll-area{flex:1;overflow-y:auto;margin-top:.6em;border:1px solid var(--border);border-radius:10px}
  pre{margin:0;padding:1em;font-size:.82rem;line-height:1.5;font-family:'Plus Jakarta Sans',monospace;
    color:var(--charcoal);white-space:pre-wrap;word-break:break-word}

  table{width:100%;border-collapse:collapse;font-size:.9rem}
  th,td{text-align:center;padding:.55em .5em;border-bottom:1px solid var(--border)}
  th{font-size:.72rem;text-transform:uppercase;letter-spacing:.04em;color:var(--mid-grey)}

  .pill{display:inline-block;font-size:.68rem;font-weight:700;padding:.15em .55em;border-radius:10px;margin-left:.4em}
  .pill.sat{background:#fde2e2;color:var(--red)}
  .pill.low{background:#fdf0d8;color:#a8730a}

  #chart{width:100%;flex:1;border-radius:12px;border:1px solid var(--border)}

  .modal-backdrop{position:fixed;inset:0;background:rgba(20,27,46,.55);
    display:none;align-items:center;justify-content:center;padding:1.4em;z-index:50}
  .modal-backdrop.show{display:flex}
  .modal{background:var(--white);border-radius:16px;max-width:560px;width:100%;
    max-height:85vh;overflow-y:auto;box-shadow:0 24px 70px rgba(20,27,46,.35)}
  .modal-head{background:var(--navy);color:#fff;padding:1.1em 1.4em;
    border-radius:16px 16px 0 0;display:flex;align-items:center;justify-content:space-between}
  .modal-head h2{margin:0;font-size:1.1rem}
  .modal-head button{background:rgba(255,255,255,.14);border:none;color:#fff;
    font-family:var(--font);font-weight:700;font-size:1rem;width:2em;height:2em;
    border-radius:8px;cursor:pointer;line-height:1}
  .modal-head button:hover{background:rgba(255,255,255,.26)}
  .modal-body{padding:1.4em 1.6em 1.8em}
  .modal-body ol{margin:0;padding-left:1.3em}
  .modal-body li{margin:.6em 0;line-height:1.5;font-size:.95rem}
  .modal-body li b{color:var(--navy)}
  .modal-body .note{margin-top:1.2em;padding:.9em 1em;background:#fdf6ee;
    border:1px solid #f0c9a0;border-radius:10px;font-size:.88rem;line-height:1.5}
  .modal-body .note b{color:#a8730a}
</style>
</head>
<body>

<header>
  <div class="brand">Absorbance Colorimeter</div>
  <div class="status-chip" id="connChip">Disconnected</div>
</header>

<div class="wrap">
  <h1>Absorbance Colorimeter</h1>

  <div class="layout">

    <!-- LEFT: CONTROLS -->
    <div class="column">
      <h2>Controls</h2>

      <div class="control-grid">
        <button class="act secondary" id="connectBtn">Connect</button>
        <button class="act" onclick="confirmCalibration()">Calibrate</button>
        <button class="act secondary" onclick="exportCSV()">Export</button>
        <button class="act secondary" onclick="clearTableConfirm()">Clear Data</button>
        <button class="act" onclick="startSweep()">Sweep</button>
        <button class="act secondary" onclick="openInstructions()">Instructions</button>
      </div>

      <input id="testName" type="text" placeholder="Enter trial name…">

      <div class="eyebrow">Read Wavelength</div>
      <div class="wavelength-grid">
        <button class="wl-btn" onclick="manualRead(665)"><div class="color-box" style="background:#ff0000;"></div>665 nm</button>
        <button class="wl-btn" onclick="manualRead(630)"><div class="color-box" style="background:#ff6600;"></div>630 nm</button>
        <button class="wl-btn" onclick="manualRead(600)"><div class="color-box" style="background:#ffcc00;"></div>600 nm</button>
        <button class="wl-btn" onclick="manualRead(550)"><div class="color-box" style="background:#00cc44;"></div>550 nm</button>
        <button class="wl-btn" onclick="manualRead(470)"><div class="color-box" style="background:#0066ff;"></div>470 nm</button>
        <button class="wl-btn" onclick="manualRead(400)"><div class="color-box" style="background:#6633cc;"></div>400 nm</button>
      </div>

      <div class="eyebrow">Status</div>
      <div class="scroll-area">
        <pre id="output"></pre>
      </div>
    </div>

    <!-- CENTER: CHART -->
    <div class="column">
      <h2>Spectrum</h2>
      <canvas id="chart"></canvas>
    </div>

    <!-- RIGHT: TABLE -->
    <div class="column">
      <h2>Absorbance Table</h2>
      <div class="scroll-area">
        <table id="absTable">
          <thead>
            <tr>
              <th>Colour</th>
              <th>Wavelength (nm)</th>
              <th>Absorbance</th>
            </tr>
          </thead>
          <tbody></tbody>
        </table>
      </div>
    </div>

  </div>
</div>

<div class="modal-backdrop" id="instrModal" onclick="if(event.target===this)closeInstructions()">
  <div class="modal">
    <div class="modal-head">
      <h2>How to use the colorimeter</h2>
      <button onclick="closeInstructions()" aria-label="Close">&times;</button>
    </div>
    <div class="modal-body">
      <ol>
        <li><b>Connect.</b> Plug the colorimeter into a USB port, click <b>Connect</b>, and choose the device in the pop-up. The chip at the top right turns green when it's connected.</li>
        <li><b>Prepare the blank.</b> Fill a clean cuvette with your blank solution (solvent only — no sample), wipe the outside, and place it in the holder.</li>
        <li><b>Block all light.</b> Stand the device on a flat surface and completely cover the top of the cuvette with the palm of your hand so no light enters the chamber. Keep it covered throughout calibration.</li>
        <li><b>Calibrate.</b> Click <b>Calibrate</b> and confirm. Each LED lights in turn while the device records the blank. Hold your palm over the cuvette until calibration finishes.</li>
        <li><b>Load your sample.</b> Swap in the cuvette holding the solution you want to measure. Close the lid again.</li>
        <li><b>Read.</b> Click a single <b>wavelength</b> button to measure one colour, or click <b>Sweep</b> to measure all six in order and plot the spectrum.</li>
        <li><b>Record.</b> Enter a trial name, then click <b>Export</b> to save the table as a CSV. Use <b>Clear Data</b> to start a fresh set of readings.</li>
      </ol>
    </div>
  </div>
</div>

<script>
let port, reader, writer;

/* Sweep data: only the latest sweep */
let latestSweep = [];
let isSweeping = false;

const sweepOrder = [400, 470, 550, 600, 630, 665];

const colourMap = {
  665: "#ff0000",
  630: "#ff6600",
  600: "#ffcc00",
  550: "#00cc44",
  470: "#0066ff",
  400: "#6633cc"
};

/* ===================== Instructions modal ======================== */

function openInstructions() {
  document.getElementById("instrModal").classList.add("show");
}
function closeInstructions() {
  document.getElementById("instrModal").classList.remove("show");
}
document.addEventListener("keydown", e => {
  if (e.key === "Escape") closeInstructions();
});

/* ===================== Confirm windows =========================== */

function confirmCalibration() {
  if (confirm("Start calibration?\n\nPlace the device on a flat surface, insert the cuvette, and completely cover the top of the cuvette with the palm of your hand so no light enters the chamber. Hold it covered until calibration finishes.")) {
    sendCommand("CALIBRATE");
  }
}

function clearTableConfirm() {
  if (confirm("Clear ALL recorded absorbance data?")) {
    clearTable();
  }
}

/* ======================= Serial setup ============================ */

async function connectSerial() {
  port = await navigator.serial.requestPort();
  await port.open({ baudRate: 9600 });

  writer = port.writable.getWriter();
  reader = port.readable.pipeThrough(new TextDecoderStream()).getReader();

  const chip = document.getElementById("connChip");
  chip.textContent = "Connected";
  chip.classList.add("on");

  readLoop();
}
document.getElementById("connectBtn").addEventListener("click", connectSerial);

/* Manual wavelength read */
function manualRead(wl) {
  sendCommand("READ_" + wl);
}

/* ========================== Sweep ================================= */

async function startSweep() {
  isSweeping = true;
  latestSweep = [];    // CLEAR CHART
  drawChart();

  for (let wl of sweepOrder) {
    sendCommand("READ_" + wl);
    await new Promise(r => setTimeout(r, 3500));
  }

  isSweeping = false;
  drawChart();
}

/* ======================= Serial parsing ============================ */

async function readLoop() {
  let buffer = "";

  while (true) {
    const { value, done } = await reader.read();
    if (done) break;

    buffer += value;
    let lines = buffer.split("\n");
    buffer = lines.pop();

    for (let line of lines) {
      line = line.trim();
      document.getElementById("output").textContent += line + "\n";

      if (line.startsWith("CAL_")) continue;
      if (line === "DONE") continue;

      if (line.includes(",") && !isNaN(line.split(",")[0])) {
        const [wl, abs] = line.split(",");

        addRow(wl, abs);

        // ONLY add to sweep if sweep is active
        if (isSweeping && sweepOrder.includes(Number(wl))) {
          latestSweep.push({ wl: Number(wl), abs: Number(abs) });
          drawChart();
        }
      }
    }
  }
}

/* ====================== Table handling ============================= */

function addRow(wavelength, absorbance) {
  const tbody = document.getElementById("absTable").querySelector("tbody");
  const colour = colourMap[wavelength] || "#000";

  const row = document.createElement("tr");
  row.innerHTML = `
    <td><div class="color-box" style="background:${colour}; margin:auto;"></div></td>
    <td>${wavelength}</td>
    <td>${absorbance}</td>
  `;
  tbody.insertBefore(row, tbody.firstChild);
}

function clearTable() {
  latestSweep = [];
  isSweeping = false;
  document.getElementById("absTable").querySelector("tbody").innerHTML = "";
  drawChart();
}

/* ========================= Send commands =========================== */

async function sendCommand(cmd) {
  if (!writer) return;
  await writer.write(new TextEncoder().encode(cmd + "\n"));
}

/* ========================== Spectrum chart ========================= */

function drawChart() {
  const canvas = document.getElementById("chart");
  const ctx = canvas.getContext("2d");

  canvas.width = canvas.clientWidth;
  canvas.height = canvas.clientHeight;

  const w = canvas.width;
  const h = canvas.height;

  /* ===== rainbow background ===== */
  const grad = ctx.createLinearGradient(0, 0, w, 0);
  grad.addColorStop(0.000, "#6633cc"); // 400 violet
  grad.addColorStop(0.264, "#0066ff"); // 470 blue
  grad.addColorStop(0.566, "#00cc44"); // 550 green
  grad.addColorStop(0.755, "#ffcc00"); // 600 yellow
  grad.addColorStop(0.868, "#ff6600"); // 630 orange
  grad.addColorStop(1.000, "#ff0000"); // 665 red

  ctx.fillStyle = grad;
  ctx.fillRect(0, 0, w, h);

  if (latestSweep.length === 0) return;

  const minAbs = Math.min(...latestSweep.map(p => p.abs));
  const maxAbs = Math.max(...latestSweep.map(p => p.abs));
  const span = maxAbs - minAbs || 1;

  const yMin = minAbs - span * 0.10;
  const yMax = maxAbs + span * 0.10;

  const pts = [...latestSweep]
    .sort((a, b) => a.wl - b.wl)
    .map(p => ({
      x: ((p.wl - 400) / (665 - 400)) * w,
      y: h - ((p.abs - yMin) / (yMax - yMin)) * h
    }));

  if (pts.length < 2) return;

  function CRtoBez(p0, p1, p2, p3) {
    return [
      p1.x + (p2.x - p0.x) / 6,
      p1.y + (p2.y - p0.y) / 6,
      p2.x - (p3.x - p1.x) / 6,
      p2.y - (p3.y - p1.y) / 6
    ];
  }

  ctx.lineWidth = 4;
  ctx.strokeStyle = "#141b2e";
  ctx.beginPath();
  ctx.moveTo(pts[0].x, pts[0].y);

  for (let i = 0; i < pts.length - 1; i++) {
    const p0 = pts[i - 1] || pts[i];
    const p1 = pts[i];
    const p2 = pts[i + 1];
    const p3 = pts[i + 2] || p2;

    const [cx1, cy1, cx2, cy2] = CRtoBez(p0, p1, p2, p3);
    ctx.bezierCurveTo(cx1, cy1, cx2, cy2, p2.x, p2.y);
  }
  ctx.stroke();

  ctx.fillStyle = "#141b2e";
  for (const pt of pts) {
    ctx.beginPath();
    ctx.arc(pt.x, pt.y, 6, 0, Math.PI * 2);
    ctx.fill();
  }
}

/* ========================= Export to CSV ========================== */

function exportCSV() {
  const name = document.getElementById("testName").value || "AbsorbanceTest";
  const rows = [["Wavelength", "Absorbance"]];

  const tbody = document.getElementById("absTable").querySelector("tbody");
  for (let r of tbody.rows)
    rows.push([r.cells[1].innerText, r.cells[2].innerText]);

  const csv = rows.map(r => r.join(",")).join("\n");
  const blob = new Blob([csv], { type: "text/csv" });

  const link = document.createElement("a");
  link.href = URL.createObjectURL(blob);
  link.download = name + ".csv";
  link.click();
}

window.addEventListener("resize", drawChart);
</script>

</body>
</html>

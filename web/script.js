function pingServer() {
    document.getElementById("status").textContent = "Pinging local Atlas server...";
    fetch("/").then(() => {
        document.getElementById("status").textContent = "✅ Atlas server is online!";
    }).catch(() => {
        document.getElementById("status").textContent = "❌ Cannot connect to server!";
    });
}

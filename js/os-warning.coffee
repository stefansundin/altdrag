---
---

if navigator.platform.indexOf("Win") == -1
  el = document.getElementById("os-warning")
  el.className = el.className.replace("hidden", "")

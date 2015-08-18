---
---

for a in document.querySelectorAll(".expander")
  a.style.display = "inline-block"
  a.addEventListener "click", ->
    id = this.getAttribute "expand"
    document.getElementById(id).style.display = "block"
    this.style.display = "none"
  id = a.getAttribute "expand"
  document.getElementById(id).style.display = "none"

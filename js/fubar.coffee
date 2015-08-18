---
---

for a in document.querySelectorAll("a[fubar]")
  continue if a.href != ""
  a.textContent = a.textContent.replace /[A-Z]{2}/, (c) ->
    (c[0]+"@"+c[1]).toLowerCase()
  .replace /[A-Z]/g, (c) ->
    "."+c.toLowerCase()
  a.href = "mailto:"+a.textContent

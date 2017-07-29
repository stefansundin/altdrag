
var links = document.querySelectorAll("a[fubar]");
for (var i=0; i < links.length; i++) {
  var a = links[i];
  if (a.href != "") {
    continue;
  }
  a.textContent = a.textContent.replace(/[A-Z]{2}/, function(c) {
    return (c[0] + "@" + c[1]).toLowerCase();
  }).replace(/[A-Z]/g, function(c) {
    return "." + c.toLowerCase();
  });
  a.href = "mailto:" + a.textContent;
}

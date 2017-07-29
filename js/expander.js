
var links = document.querySelectorAll(".expander");
for (var i=0; i < links.length; i++) {
  var a = links[i];
  a.style.display = "inline-block";
  a.addEventListener("click", function() {
    var id = this.getAttribute("expand");
    document.getElementById(id).style.display = "block";
    this.style.display = "none";
  });
  var id = a.getAttribute("expand");
  document.getElementById(id).style.display = "none";
}

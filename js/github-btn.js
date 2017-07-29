---

document.addEventListener("DOMContentLoaded", function() {
  function addCommas(n) {
    return n.toString().replace(/(\d)(?=(\d{3})+($|,|\.))/g, "$1,");
  }

  var btns = document.querySelectorAll("[data-github-btn]");
  for (var i=0; i < btns.length; i++) {
    var btn = btns[i];
    var repo = btn.getAttribute("data-github-btn")  || "{{ site.github.repository_nwo }}";
    var button = document.createElement("a");
    button.href = `https://github.com/${repo}`;
    button.target = "_blank";
    button.appendChild(document.createElement("span"));
    button.appendChild(document.createTextNode("Star"));
    btn.classList.add("github-btn");
    btn.appendChild(button);
    var xhr = new XMLHttpRequest();
    xhr.addEventListener("load", function() {
      var json = JSON.parse(this.responseText);
      var counter = document.createElement("a");
      counter.href = `https://github.com/${repo}/stargazers`;
      counter.target = "_blank";
      counter.appendChild(document.createTextNode(addCommas(json.stargazers_count)));
      btn.appendChild(counter);
    });
    xhr.open("get", `https://api.github.com/repos/${repo}`, true);
    xhr.send();
  }
});

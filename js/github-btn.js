---
---

document.addEventListener("DOMContentLoaded", () => {
  function addCommas(n) {
    return n.toString().replace(/(\d)(?=(\d{3})+($|,|\.))/g, "$1,");
  }

  document.querySelectorAll("[data-github-btn]").forEach(async (btn) => {
    const repo = btn.getAttribute("data-github-btn") || "{{ site.github.repository_nwo }}";
    const button = document.createElement("a");
    button.href = `https://github.com/${repo}`;
    button.target = "_blank";
    button.appendChild(document.createElement("span"));
    button.appendChild(document.createTextNode("Star"));
    btn.classList.add("github-btn");
    btn.appendChild(button);
    const response = await fetch(`https://api.github.com/repos/${repo}`);
    if (!response.ok) {
      console.error(response);
      return;
    }
    const data = await response.json();
    const counter = document.createElement("a");
    counter.href = `https://github.com/${repo}/stargazers`;
    counter.target = "_blank";
    counter.appendChild(document.createTextNode(addCommas(data.stargazers_count)));
    btn.appendChild(counter);
  });
});

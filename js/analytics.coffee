---
---

if window.location.protocol != "file:"
  ((i,s,o,g,r,a,m) ->
    i['GoogleAnalyticsObject']=r;
    i[r]=i[r] || ->
      (i[r].q = i[r].q || []).push(arguments)
    i[r].l = 1*new Date()
    a = s.createElement(o)
    m = s.getElementsByTagName(o)[0]
    a.async = 1
    a.src = g
    m.parentNode.insertBefore(a,m)
  )(window,document,'script','//www.google-analytics.com/analytics.js','ga')

gaid = document.body.getAttribute('ga') || 'UA-6797859-18'

ga('create', gaid, 'auto')
ga('send', 'pageview')


trackLink = (e) ->
  category = this.getAttribute('track')
  if !category
    if this.href.indexOf('mailto:') == 0
      category = 'mailto'
    else
      category = 'link'
  ga('send', 'event', category, 'click', this.href)

for link in document.getElementsByTagName('a')
  link.addEventListener('click', trackLink)

trackForm = (e) ->
  category = this.getAttribute('track')
  if !category
    category = 'form'
  label = this.action
  for input in this.getElementsByTagName('input')
    if input[i].name && input[i].type != 'hidden'
      label += ' '+inputs[i].name+'='+inputs[i].value
  ga('send', 'event', category, 'click', label)

for form in document.getElementsByTagName('form')
  form.addEventListener('submit', trackForm)

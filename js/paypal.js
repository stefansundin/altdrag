
var forms = document.querySelectorAll('form[action="https://www.paypal.com/cgi-bin/webscr"]');
for (var i=0; i < forms.length; i++) {
  var form = forms[i];
  form.addEventListener('submit', function(e) {
    if (parseInt(this.amount.value) < 1) {
      e.preventDefault();
      alert("The minimum donation amount is one dollar. Anything less than one dollar and you're just giving PayPal everything because of their fees.\n\nIf you can't afford one dollar, then please donate to a local charity instead.");
      return false;
    }
  });
}

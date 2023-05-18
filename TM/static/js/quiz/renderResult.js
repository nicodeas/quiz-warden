const returnHomeButton = document.getElementById("home-button");

returnHomeButton.addEventListener("click", function (e) {
  e.preventDefault();
  window.location.href = "/";
});

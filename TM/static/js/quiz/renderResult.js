import { getState } from "../utils/getState.js";

const state = await getState();
console.log(state);
if (state.completed) {
  document.getElementById("total-marks").innerHTML = state.totalMarks;
} else {
  window.location.href = "/quiz";
}

const returnHomeButton = document.getElementById("home-button");

returnHomeButton.addEventListener("click", function (e) {
  e.preventDefault();
  window.location.href = "/";
});
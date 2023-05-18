import { getState } from "../utils/getState.js";

const returnHomeButton = document.getElementById("home-button");
const markElement = document.getElementById("mark");
const totalMarkElement = document.getElementById("total-mark");

const state = await getState();
markElement.innerHTML = state.totalScore;
totalMarkElement.innerHTML = state.maxQuestions * 3;

returnHomeButton.addEventListener("click", function (e) {
  e.preventDefault();
  window.location.href = "/";
});

import { getState } from "../utils/getState.js";

const state = await getState();

if (state.completed) {
  document.getElementById("total-marks").innerText = state.totalMarks;
  document.getElementById("max-marks").innerText = state.maxQuestions * 3;
} else {
  window.location.href = "/quiz";
}

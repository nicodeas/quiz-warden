import { getState } from "../utils/getState.js";

const state = await getState();

// If the quiz is completed, show the total marks. Otherwise, redirect to the quiz page.
if (state.completed) {
  document.getElementById("total-marks").innerText = state.totalMarks;
  document.getElementById("max-marks").innerText = state.maxQuestions * 3;
} else {
  window.location.href = "/quiz";
}

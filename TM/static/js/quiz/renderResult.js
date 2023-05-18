import { getState } from "../utils/getState.js";

const state = await getState();
console.log(state);
if (state.completed) {
  document.getElementById("total-marks").innerText = state.totalMarks;
} else {
  window.location.href = "/quiz";
}

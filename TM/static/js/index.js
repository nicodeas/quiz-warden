import { renderState } from "./quiz/renderState.js";
import { getState } from "./utils/getState.js";
import { host } from "./config.js";

const state = await getState();
console.log(state);
const startQuiz = async () => {
  try {
    await fetch(`http://${host}/api/generatequiz`);
    window.location.href = `http://${host}/quiz`;
  } catch (err) {
    console.log(err);
  }
};

const continueQuiz = async () => {
  window.location.href = `http://${host}/quiz?question=${state.currentQuestion}`;
};

renderState(false, state);

const startButton = document.getElementById("start-quiz");

if (startButton) {
  if (!state.completed) {
    startButton.innerText = "Continue Quiz";
  }

  startButton.addEventListener("click", async (e) => {
    e.preventDefault();
    if (state.activeQBs.length > 0) {
      if (!state.completed) {
        continueQuiz();
      } else {
        startQuiz();
      }
    }
  });
}

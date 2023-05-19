import { renderState } from "./quiz/renderState.js";
import { getState } from "./utils/getState.js";

const state = await getState();
console.log(state);
const startQuiz = async () => {
  try {
    await fetch("http://localhost:8000/api/generatequiz");
    window.location.href = "http://localhost:8000/quiz";
  } catch (err) {
    console.log(err);
  }
};

const continueQuiz = async () => {
  window.location.href = `http://localhost:8000/quiz?question=${state.currentQuestion}`;
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

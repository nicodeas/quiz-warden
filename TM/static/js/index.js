import { renderState } from "./quiz/renderState.js";
import { getState } from "./utils/getState.js";

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

const state = await getState();

renderState(false, state);

const startButton = document.getElementById("start-quiz");

if (startButton) {
  if (!state.completed) {
    startButton.innerText = "Continue Quiz";
  }

  startButton.addEventListener("click", async (e) => {
    e.preventDefault();
    if (!state.completed) {
      continueQuiz();
    } else {
      startQuiz();
    }
  });
}

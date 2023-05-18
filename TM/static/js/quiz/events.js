import { state, question } from "./index.js";
import { markQuestion } from "./markQuestion.js";

const checkButton = document.getElementById("check-button");

checkButton.addEventListener("click", async function (e) {
  e.preventDefault();
  let answer = null;
  if (question.type == "CHOICE") {
    const options = document.getElementsByName("option");
    for (let i = 0; i < options.length; i++) {
      if (options[i].checked) {
        answer = options[i].value;
        break;
      }
    }

    if (answer === -1) {
      alert("Please select an answer!");
      return;
    }
  } else if (question.type == "CODE") {
    answer = document.getElementById("code-answer").value;
  }
  await markQuestion(answer);
});

document.getElementById("next-button").addEventListener("click", function (e) {
  e.preventDefault();
  const questionAttemptsText = document.getElementById("question-attempts");
  questionAttemptsText.innerHTML = "";
  checkButton.disabled = false;

  window.location.href = "/quiz?question=" + (state.currentQuestion + 1);
});

document.getElementById("back-button").addEventListener("click", function (e) {
  e.preventDefault();
  const questionAttemptsText = document.getElementById("question-attempts");
  questionAttemptsText.innerHTML = "";
  checkButton.disabled = false;

  window.location.href = "/quiz?question=" + (state.currentQuestion - 1);
});

document.getElementById("home-button").addEventListener("click", function (e) {
  e.preventDefault();
  window.location.href = "/";
});

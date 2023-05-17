import { state, question } from "./index.js";

const checkButton = document.getElementById("check-button");

checkButton.addEventListener("click", function (e) {
  e.preventDefault();
  if (question.type == "CHOICE") {
    const options = document.getElementsByName("option");
    let checkedQuestionIndex = -1;
    for (let i = 0; i < options.length; i++) {
      if (options[i].checked) {
        checkedQuestionIndex = i;
        break;
      }
    }
    if (checkedQuestionIndex === -1) {
      alert("Please select an answer!");
      return;
    }
    console.log(checkedQuestionIndex);
    checkQuestion(checkedQuestionIndex);
  } else if (question.type == "CODE") {
    const answer = document.getElementById("code-answer").value;

    console.log(answer);
    checkQuestion(answer);
  }
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

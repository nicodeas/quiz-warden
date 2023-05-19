// This file contains all the event handlers for the quiz pages

import { state, question } from "./index.js";
import { markQuestion } from "./markQuestion.js";

const checkButton = document.getElementById("check-button");

checkButton.addEventListener("click", async function (e) {
  e.preventDefault();
  let answer = null;
  if (question.type == "CHOICE") {
    const options = document.getElementsByName("option");
    // loop throught all multiple choice options to find the selected one
    for (let i = 0; i < options.length; i++) {
      if (options[i].checked) {
        answer = options[i].value;
        break;
      }
    }
  } else if (question.type == "CODE" || question.type == "IMAGE") {
    answer = document.getElementById("code-answer").value || null;
  }

  if (answer === null) {
    alert("Please input an answer!");
    return;
  }
  await markQuestion(answer);
});

document.getElementById("next-button").addEventListener("click", function (e) {
  e.preventDefault();
  window.location.href = "/quiz?question=" + (state.currentQuestion + 1);
});

document.getElementById("back-button").addEventListener("click", function (e) {
  e.preventDefault();
  window.location.href = "/quiz?question=" + (state.currentQuestion - 1);
});

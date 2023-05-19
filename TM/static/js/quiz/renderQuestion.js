import { getAnswer } from "./getAnswer.js";
import { getState } from "../utils/getState.js";

export const renderQuestion = async (data) => {
  const { text, language, type, attempts, correct, choices } = data;
  const state = await getState();
  const languageElement = document.getElementById("language");
  const quizQuestion = document.getElementById("quiz-question");
  const questionAttempt = document.getElementById("question-attempts");
  //const attemptText = document.getElementById("attempt-text");
  const hasActiveQB = state.activeQBs.length > 0;
  // Make sure we have an active QBs
  if (hasActiveQB) {
    languageElement.innerText = language == "CLANG" ? "C" : "Python";
    quizQuestion.innerText = text;
    questionAttempt.innerText = attempts;
  } else {
    languageElement.innerText = `Question bank for question ${state.currentQuestion} is offline!`;
    quizQuestion.innerText = "";
    //attemptText.style.display = "none";
    //attemptText.innerText = "";
  }

  const answerContainer = document.getElementById("quiz-choices");
  // Reset the container to blank
  answerContainer.innerText = "";

  if (correct != undefined) {
    // disable check button
    document.getElementById("check-button").disabled = true;

    document.getElementById(
      "question-result"
    ).innerHTML = `Result: <span id="result">${
      correct ? "Correct" : "Incorrect"
    }</span>`;

    // style red or green
    document.getElementById("result").style.color = correct ? "green" : "red";

    // mark is 0 if incorrect, otherwise based on attempts
    const mark = correct ? 4 - attempts : 0;
    document.getElementById("question-mark").innerText = `Mark: ${mark}`;

    if (correct != undefined) {
      await getAnswer(state.currentQuestion, type);
    }
  }

  if (type == "CHOICE") {
    // create multichoice fields
    choices.forEach((choice, idx) => {
      const radio = document.createElement("input");
      radio.type = "radio";
      radio.name = "option";
      radio.value = choice;
      radio.id = `choice-${idx}`;
      const label = document.createElement("label");
      label.innerText = choice;
      label.setAttribute("for", `choice-${idx}`);

      const choiceContainer = document.createElement("div");
      choiceContainer.appendChild(radio);
      choiceContainer.appendChild(label);
      answerContainer.appendChild(choiceContainer);
    });
  } else if (type == "CODE" || type == "IMAGE") {
    // Create a text area for code questions
    const textarea = document.createElement("textarea");
    textarea.id = "code-answer";
    answerContainer.append(textarea);
  }
};

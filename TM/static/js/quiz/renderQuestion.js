import { getAnswer } from "./getAnswer.js";
import { getState } from "../utils/getState.js";

export const renderQuestion = async (data) => {
  const state = await getState();
  const { text, language, type, attempts, correct, choices, errors } = data;

  const languageElement = document.getElementById("language");
  languageElement.innerText = language == "CLANG" ? "C" : "Python";

  const answerContainer = document.getElementById("quiz-choices");
  // Reset the container to blank
  answerContainer.innerText = "";

  document.getElementById("question-attempts").innerText = attempts;

  document.getElementById("errors").innerText = errors;

  if (attempts !== 1) {
    document.getElementById(
      "question-result"
    ).innerHTML = `Result: <span id="result">${
      correct ? "Correct" : "Incorrect"
    }</span>`;

    // style red or green
    document.getElementById("result").style.color = correct ? "green" : "red";
  }

  if (correct != undefined) {
    // disable check button
    document.getElementById("check-button").disabled = true;

    // mark is 0 if incorrect, otherwise based on attempts
    const mark = correct ? 4 - attempts : 0;
    document.getElementById("question-mark").innerText = `Mark: ${mark}`;
    console.log(type, data);

    if (correct != undefined) {
      await getAnswer(state.currentQuestion, type);
    }
  }

  document.getElementById("quiz-question").innerText = text;

  if (type == "CHOICE") {
    console.log(choices);
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

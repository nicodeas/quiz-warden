export const renderQuestion = (data) => {
  const { text, language, type, attempts, correct, choices } = data;

  const languageElement = document.getElementById("language");
  languageElement.innerHTML = language == "CLANG" ? "C" : "Python";

  const answerContainer = document.getElementById("quiz-choices");
  // Reset the container to blank
  answerContainer.innerHTML = "";

  document.getElementById("question-attempts").innerHTML = attempts;

  if (correct != undefined) {
    // disable check button
    document.getElementById("check-button").disabled = true;

    document.getElementById("question-result").innerHTML = `Result: <span id="result">${correct ? "Correct" : "Incorrect"}</span>`;

    // style red or green
    document.getElementById("result").style.color = correct ? "green" : "red";

    // mark is 0 if incorrect, otherwise based on attempts
    const mark = correct ? 4 - attempts : 0;
    document.getElementById("question-mark").innerHTML = `Mark: ${mark}`
  }

  document.getElementById("quiz-question").innerHTML = text;

  if (type == "CHOICE") {
    // create multichoice fields
    choices.forEach((choice, idx) => {
      const radio = document.createElement("input");
      radio.type = "radio";
      radio.name = "option";
      radio.value = choice;
      radio.id = `choice-${idx}`;
      const label = document.createElement("label");
      label.innerHTML = choice;
      label.setAttribute("for", `choice-${idx}`);

      const choiceContainer = document.createElement("div");
      choiceContainer.appendChild(radio);
      choiceContainer.appendChild(label);
      answerContainer.appendChild(choiceContainer);
    });
  } else if (type == "CODE") {
    const textarea = document.createElement("textarea");
    textarea.id = "code-answer";
    answerContainer.append(textarea);
  }
};

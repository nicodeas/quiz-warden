export const renderQuestion = (question, language, type, choices, image) => {
  const languageElement = document.getElementById("language");
  languageElement.innerHTML = language == "CLANG" ? "C" : "Python";
  const answerContainer = document.getElementById("quiz-choices");
  // Reset the container to blank
  answerContainer.innerHTML = "";
  document.getElementById("quiz-question").innerHTML = question;
  // Render question elements based on question type
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
  } else if (type == "CODE" || type == "IMAGE") {
    // Create a text area for code questions
    const textarea = document.createElement("textarea");
    textarea.id = "code-answer";
    answerContainer.append(textarea);
  }
};

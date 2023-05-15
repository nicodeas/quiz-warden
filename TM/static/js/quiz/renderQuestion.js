export const renderQuestion = (question, language, type, choices) => {
  const languageElement = document.getElementById("language");
  languageElement.innerHTML = language;
  const answerContainer = document.getElementById("quiz-choices");
  // Reset the container to blank
  answerContainer.innerHTML = "";
  document.getElementById("quiz-question").innerHTML = question;
  console.log("Question", question, language, type, choices);
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
  } else if (type == "IMAGE") {
    choices.forEach((choice, idx) => {
      console.log("choice ", choice);
      const radio = document.createElement("input");
      radio.type = "radio";
      radio.name = "option";
      radio.value = choice;
      radio.id = `choice-${idx}`;
      const image = document.createElement("img");
      image.src = choice;

      const choiceContainer = document.createElement("div");
      choiceContainer.appendChild(radio);
      choiceContainer.appendChild(image);
      answerContainer.appendChild(choiceContainer);
    });
  }
};

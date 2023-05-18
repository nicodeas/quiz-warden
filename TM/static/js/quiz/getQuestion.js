import { renderQuestion } from "./renderQuestion.js";

export const getQuestion = async (currentQuestionIndex) => {
  document.getElementById("question-result").innerHTML = "";

  let choices = null;
  try {
    const res = await fetch("/api/question?number=" + currentQuestionIndex, {
      method: "GET",
    });
    const data = await res.json();

    if (data.type == "CHOICE") {
      choices = data.choices;
    }
    // just added data.image until multiple images get sent as choices (?).
    renderQuestion(data.text, data.language, data.type, choices, data.image);
  } catch (err) {
    console.error(err);
  }
};

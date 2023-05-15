import { renderQuestion } from "./renderQuestion.js";

export const getQuestion = async (currentQuestionIndex) => {
  document.getElementById("question-result").innerHTML = "";

  let choices = null;
  try {
    const res = await fetch("/api/question?number=" + currentQuestionIndex, {
      method: "GET",
    });
    const data = await res.json();
    console.log("data type ", data.type);
    if (data.type == "CHOICE" || data.type == "IMAGE") {
      choices = data.choices;
    }
    renderQuestion(data.text, data.language, data.type, choices);
  } catch (err) {
    console.error(err);
  }
};

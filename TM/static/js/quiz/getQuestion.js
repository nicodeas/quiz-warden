import { renderQuestion } from "./renderQuestion.js";

export const getQuestion = async (currentQuestionIndex) => {
  document.getElementById("question-result").innerHTML = "";

  try {
    const res = await fetch("/api/question?number=" + currentQuestionIndex, {
      method: "GET",
    });
    const data = await res.json();
    renderQuestion(data);
  } catch (err) {
    console.error(err);
  }
};

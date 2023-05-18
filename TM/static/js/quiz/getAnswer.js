import { renderAnswer } from "./renderAnswer.js";

export const getAnswer = async (currentQuestionIndex) => {
  document.getElementById("question-result").innerText = "";

  try {
    const res = await fetch("/api/answer?number=" + currentQuestionIndex, {
      method: "GET",
    });
    const data = await res.json();
    renderAnswer(data);
    return data;
  } catch (err) {
    console.error(err);
  }
};

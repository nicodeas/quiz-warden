import { renderAnswer } from "./renderAnswer.js";

export const getAnswer = async (currentQuestionIndex, type) => {
  let data = null;
  try {
    const res = await fetch("/api/answer?number=" + currentQuestionIndex, {
      method: "GET",
    });
    data = await res.json();
  } catch (err) {
    console.error(err);
  }
  if (data != undefined) {
    renderAnswer(data, type);
  }
};

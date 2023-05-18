import { renderQuestion } from "./renderQuestion.js";

export const getQuestion = async (currentQuestionIndex) => {
  try {
    const res = await fetch("/api/question?number=" + currentQuestionIndex, {
      method: "GET",
    });
    const data = await res.json();
    await renderQuestion(data);
    return data;
  } catch (err) {
    console.error(err);
  }
};

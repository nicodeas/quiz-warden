import { state } from "./index.js";

// Makes a POST request with the index of the chosen answer
export const markQuestion = async (questionAnswer) => {
    const questionResultText = document.getElementById("question-result");
    const questionAttemptsText = document.getElementById("question-attempts");
    const questionMarkText = document.getElementById("question-attempts");
    const data = { answer: questionAnswer, number: state.currentQuestion };
    
    try {
      const res = await fetch("/api/mark", {
        method: "POST",
        headers: {
          "Content-Type": "application/json",
        },
        body: JSON.stringify(data),
      })

      console.log(res);
    } catch (err) {
      console.error(err);
    }
  };
  
import { state } from "./index.js";

// Makes a POST request with the index of the chosen answer
export const markQuestion = async (questionAnswer) => {
    const data = { answer: questionAnswer, number: state.currentQuestion };
    
    let res;
    try {
      res = await fetch("/api/mark", {
        method: "POST",
        headers: {
          "Content-Type": "application/json",
        },
        body: JSON.stringify(data),
      })

    } catch (err) {
      console.error(err);
    }

    if (res.status == 200) {
      location.reload();
    }
  };
  